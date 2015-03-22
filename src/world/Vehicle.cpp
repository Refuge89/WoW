/*
 */

#include "StdAfx.h"

Vehicle::Vehicle(uint64 guid) : Creature(guid)
{
	m_vehicleEntry = 0;
	m_passengerCount = 0;
	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	memset( m_vehicleSeats, 0, sizeof(uint32)*8 );
	m_isVehicle = true;
	Initialised = false;
	m_CreatedFromSpell = false;
	m_mountSpell = 0;

	for(uint8 i = 0; i < 8; ++i)
	{
		seatisusable[i] = false;
		m_vehicleSeats[i] = NULL;
		m_passengers[i] = NULL;
	}
}

Vehicle::~Vehicle()
{
	m_passengerCount = 0;
	if( IsInWorld() )
		RemoveFromWorld(false, true);
}

void Vehicle::Destructor()
{
	delete this;
}

void Vehicle::Init()
{
	Creature::Init();
}

void Vehicle::InitSeats(uint32 vehicleEntry, Player* pRider)
{
	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	SetVehicleEntry(vehicleEntry);
	vehicleData = dbcVehicle.LookupEntry( vehicleEntry );
	if(!vehicleData)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.", vehicleEntry);
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( vehicleData->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( vehicleData->m_seatID[i] );
			m_seatSlotMax = i + 1; // i plus 1 so that we can count 0. Plus, no vehicle has 0 seats, so this ensures 1.

			if(m_vehicleSeats[i]->IsUsable())
			{
				seatisusable[i] = true;
				++m_maxPassengers;
			}
		}
	}

	Initialised = true;
	InstallAccessories();

	if( pRider != NULL)
		AddPassenger( pRider );
}

void Vehicle::InstallAccessories()
{
}

void Vehicle::Load(CreatureProto * proto_, float x, float y, float z, float o /* = 0.0f */)
{
	proto = proto_;
	if(!proto)
		return;

	if(proto->vehicle_entry != -1)
	{
		m_vehicleEntry = proto->vehicle_entry;
	}
	else
	{
		m_vehicleEntry = 124;
		OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.", proto->Id);
	}

	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	vehicleData = dbcVehicle.LookupEntry( m_vehicleEntry );
	if(!vehicleData)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.", GetVehicleEntry());
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( vehicleData->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( vehicleData->m_seatID[i] );
			m_seatSlotMax = (i = 0 ? i + 1 : i); // i plus 1 so that we can count 0. Plus, no vehicle has 0 seats, so this ensures 1.

			if(m_vehicleSeats[i]->IsUsable())
			{
				seatisusable[i] = true;
				++m_maxPassengers;
			}
		}
	}

	Initialised = true;

	Creature::Load(proto_, x, y, z, o);
}

bool Vehicle::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;

	if(proto->vehicle_entry != -1)
	{
		m_vehicleEntry = proto->vehicle_entry;
	}
	else
	{
		m_vehicleEntry = 124;
		OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.", proto->Id);
	}

	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	vehicleData = dbcVehicle.LookupEntry( m_vehicleEntry );
	if(!vehicleData)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.", GetVehicleEntry());
		return false;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( vehicleData->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( vehicleData->m_seatID[i] );
			m_seatSlotMax = i + 1; // i plus 1 so that we can count 0. Plus, no vehicle has 0 seats, so this ensures 1.

			if(m_vehicleSeats[i]->IsUsable())
			{
				seatisusable[i] = true;
				++m_maxPassengers;
			}
		}
	}

	Initialised = true;

	return Creature::Load(spawn, mode, info);
}


void Vehicle::OnPushToWorld()
{
	ChangePowerType();
//	InstallAccessories();
}

void Vehicle::SendSpells(uint32 entry, Player* plr)
{
	uint8 count = 0;
	//Vehicles spells are set in the AI_Agents table
	//in the world database these are cached in
	//AIInterface so we will set up an temporary
	//list and an iterator, then iterate over the
	//the AI spell list adding any spells that can
	//be cast by the controlling unit on the vehicle
	//to our temporary list. These are spells of
	//type 4 from enum AI_Agent::AGENT_SPELL
	list<uint32> avail_spells;
	for(list<AI_Spell*>::iterator itr = GetAIInterface()->m_spells.begin(); itr != GetAIInterface()->m_spells.end(); ++itr)
	{
		if((*itr)->agent == AGENT_SPELL)
			avail_spells.push_back((*itr)->spell->Id);
	}
	//set the iterator to our temporary list
	//and position it at the start
	list<uint32>::iterator itr = avail_spells.begin();

	//Now we can build the packet to send these spells
	//to the vehicle, this uses the pet spell packet
	WorldPacket data(SMSG_PET_SPELLS, 8+4+4+1+1+2+4*10+1+1);
	data << uint64(GetGUID());
	data << uint32(GetCreatureInfo()->Family);
	data << uint32(0x00000000);//unk
	data << uint16(0x0101);	//unk2

	//iterate over the temporary list
	//and add the spells to the packet
	for(uint8 i = 1; i < 10; ++i)
	{
		if(itr != avail_spells.end())
		{
			data << uint16((*itr)) << uint8(0) << uint8(i+8);
			++itr;
		}
		else
			data << uint16(0) << uint8(0) << uint8(i+8);
			++count;
	}

	//count of spells to follow, these are spells
	//which if we were sending to a pet would have
	//been added to the players spell book, so
	//obviously we don't want to do that for a
	//vehicle
	data << count;			
	data << uint8(0);			//count of spell cool down times to follow

	plr->GetSession()->SendPacket(&data);

	data.clear();
	data.SetOpcode(SMSG_PET_GUIDS);
	data << uint32(1); // count
	data << uint64(GetGUID());
	plr->GetSession()->SendPacket(&data);
}

void Vehicle::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::Despawn, (uint32)0, respawntime, EVENT_VEHICLE_RESPAWN, delay, 1,0);
		return;
	}

	if(!IsInWorld())
		return;

	if(respawntime)
	{
		/* get the cell with our SPAWN location. if we've moved cell this might break :P */
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(!pCell)
			pCell = m_mapCell;
	
		ASSERT(pCell);
		pCell->_respawnObjects.insert(TO_OBJECT(this));
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnVehicle, TO_VEHICLE(this), pCell, EVENT_VEHICLE_RESPAWN, respawntime, 1, 0);
		Unit::RemoveFromWorld(false);
		m_position = m_spawnLocation;
		m_respawnCell=pCell;
	}
	else
	{
		Unit::RemoveFromWorld(true);
		SafeDelete();
	}
}

void Vehicle::Update(uint32 time)
{
	Creature::Update(time);
}

void Vehicle::SafeDelete()
{
	for(int i = 0; i < 8; ++i)
	{
		if(!m_passengers[i])
			continue;

		// Remove any passengers
		RemovePassenger(m_passengers[i]);
	}

	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::DeleteMe, EVENT_VEHICLE_SAFE_DELETE, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Vehicle::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	Destructor();
}

void Vehicle::AddPassenger(Unit* pPassenger)
{
	if(!m_maxPassengers || !m_seatSlotMax) //how is this happening?
	{
		sLog.outColor(TRED, "Vehicle was not correctly initialised, retrying");
		sLog.outColor(TNORMAL, "\n");
		InitSeats(m_vehicleEntry);
	}

	OUT_DEBUG("AddPassenger1: Max Vehicle Slot: %u, Max Passengers: %u", m_seatSlotMax, m_maxPassengers);

	if(pPassenger->m_CurrentVehicle)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	// Find an available seat
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if(pPassenger->IsPlayer())
		{
			if(!m_passengers[i] && m_vehicleSeats[i] && (seatisusable[i] == true)) // Found a slot
			{
				_AddToSlot(pPassenger, i );
				break;
			}
		}
		else
		{
			if(!m_passengers[i] && m_vehicleSeats[i])
			{
				_AddToSlot(pPassenger, i );
				break;
			}
		}
	}
}

void Vehicle::AddPassenger(Unit* pPassenger, uint8 requestedseat, bool force /*= false*/)
{
	// Look at how fancy we are, we get to request a slot for ourselves!
	if(!m_maxPassengers || !m_seatSlotMax) //how is this happening?
	{
		sLog.outColor(TRED, "Vehicle was not correctly initialised, retrying");
		sLog.outColor(TNORMAL, "\n");
		InitSeats(m_vehicleEntry);
	}

	if(pPassenger->m_CurrentVehicle)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	OUT_DEBUG("AddPassenger2: Max Vehicle Slot: %u, Max Passengers: %u\n", m_seatSlotMax, m_maxPassengers);

	if(requestedseat > 0)
	{
		if(force)
		{
			if(m_vehicleSeats[requestedseat] && seatisusable[requestedseat] == true) // Slot available?
			{
				if(m_passengers[requestedseat])
					RemovePassenger(m_passengers[requestedseat]);

				_AddToSlot(pPassenger, requestedseat);
				return;
			}
		}
		else
		{
			if(!m_passengers[requestedseat] && m_vehicleSeats[requestedseat] && seatisusable[requestedseat] == true) // Slot available?
			{
				_AddToSlot(pPassenger, requestedseat );
				return;
			}
		}
	}
	else
	{ // Find us a slot!
		for(uint8 i = 0; i < m_seatSlotMax; ++i)
		{
			if(pPassenger->IsPlayer())
			{
				if(!m_passengers[i] && m_vehicleSeats[i] && (seatisusable[i] == true)) // Found a slot
				{
					_AddToSlot(pPassenger, i );
					break;
				}
			}
			else
			{
				if(!m_passengers[i] && m_vehicleSeats[i])
				{
					_AddToSlot(pPassenger, i );
					break;
				}
			}
		}
	}
}

uint8 Vehicle::GetPassengerSlot(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if( m_passengers[i] == pPassenger ) // Found a slot
		{
			return i;
			break;
		}
	}
	return 0xFF;
}

void Vehicle::RemovePassenger(Unit* pPassenger)
{
	if(!pPassenger) // We have enough problems that we need to do this :(
		return;

	Vehicle* pThis = TO_VEHICLE(this);

	uint8 slot = pPassenger->m_inVehicleSeatId;

	pPassenger->m_CurrentVehicle = NULL;
	pPassenger->m_inVehicleSeatId = 0xFF;

	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));
	if( pPassenger->IsPlayer() )
		pPassenger->RemoveAura(TO_PLAYER(pPassenger)->m_MountSpellId);

	if( m_mountSpell )
		pPassenger->RemoveAura( m_mountSpell );

	WorldPacket data(SMSG_MONSTER_MOVE, 85);
	data << pPassenger->GetNewGUID();			// PlayerGUID
	data << uint8(0);							// Unk - blizz uses 0x40
	data << pPassenger->GetPosition();			// Player Position xyz
	data << getMSTime();						// Timestamp
	data << uint8(0x4);							// Flags
	data << pPassenger->GetOrientation();		// Orientation
	data << uint32(MOVEFLAG_AIR_SUSPENSION);	// MovementFlags
	data << uint32(0);							// MovementTime
	data << uint32(1);							// Pointcount
	data << GetPosition();						// Vehicle Position xyz
	SendMessageToSet(&data, false);

	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->flags &= ~MOVEFLAG_TAXI;
	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->transX = 0;
	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->transY = 0;
	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->transZ = 0;
	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->transO = 0;
	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->transTime = 0;
	static_cast<Player*>(pPassenger)->GetSession()->GetMovementInfo()->transSeat = 0;

	if(pPassenger->IsPlayer())
	{
		Player* plr = TO_PLAYER(pPassenger);
		if(plr == GetControllingUnit())
		{
			plr->m_CurrentCharm = NULL;
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << (uint8)0;
			plr->GetSession()->SendPacket(&data);
		}
		RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_PLAYER_CONTROLLED_CREATURE | UNIT_FLAG_PLAYER_CONTROLLED));

		plr->SetPlayerStatus(TRANSFER_PENDING);
		plr->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
		plr->SetPosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());
		data.Initialize(MSG_MOVE_TELEPORT_ACK);
		data << plr->GetNewGUID();
		data << plr->m_teleportAckCounter;
		plr->m_teleportAckCounter++;
		data << uint32(MOVEFLAG_FLYING);
		data << uint16(0x40);
		data << getMSTime();
		data << GetPositionX();
		data << GetPositionY();
		data << GetPositionZ();
		data << GetOrientation();
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);

		plr->SetUInt64Value( PLAYER_FARSIGHT, 0 );

		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(m_vehicleSeats[slot]->m_exitUISoundID);
		data << plr->GetPosition();
		plr->GetSession()->SendPacket(&data);

		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);

/*		CreatureProtoVehicle* vehicleproto = CreatureProtoVehicleStorage.LookupEntry(GetEntry());
		if(vehicleproto && vehicleproto->healthfromdriver)
		{
			if(slot == 0)
			{
				uint32 health = GetUInt32Value(UNIT_FIELD_HEALTH);
				uint32 maxhealth = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
				uint32 healthdiff = maxhealth - health;

				SetUInt32Value(UNIT_FIELD_HEALTH, GetProto()->MaxHealth - (healthdiff/(((plr->GetTotalItemLevel())*(vehicleproto->healthunitfromitemlev)))));
				SetUInt32Value(UNIT_FIELD_MAXHEALTH, GetProto()->MaxHealth);
			}
		}*/
		plr->SetPlayerStatus(NONE);
	}

	if(slot == 0)
	{
		m_redirectSpellPackets = NULL;
		CombatStatus.Vanished();
		pPassenger->SetUInt64Value( UNIT_FIELD_CHARM, 0 );
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
		SetFaction( GetCharmTempVal());
		/* update target faction set */
		_setFaction();
		UpdateOppFactionSet();

		//GetAIInterface()->SetAIState(STATE_IDLE);
		GetAIInterface()->WipeHateList();
		GetAIInterface()->WipeTargetList();
		EnableAI();
		//Despawn(0,1000);
	}

	m_passengers[slot] = NULL;
	//note: this is not blizz like we should despawn
	//and respawn at spawn point.
	//Well actually this is how blizz wanted it
	//but they couldnt get it to work xD
	bool haspassengers = false;
	for(uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if(m_passengers[i] != NULL)
		{
			haspassengers = true;
			break;
		}
	}

	if(!haspassengers)
	{
		if( m_spawn )
			GetAIInterface()->MoveTo(m_spawn->x, m_spawn->y, m_spawn->z, m_spawn->o);
		else //we're a temp spawn
			SafeDelete();
	}

	// We need to null this out, its changed automatically.
//	if(m_TransporterGUID) // Vehicle has a transport guid?
//		pPassenger->m_TransporterGUID = m_TransporterGUID;
//	else
//		pPassenger->m_TransporterGUID = NULL;

	if(pPassenger->IsPlayer())
		--m_passengerCount;

	if(!IsFull())
		SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
}

bool Vehicle::HasPassenger(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if( m_passengers[i] == pPassenger )
			return true;
	}
	return false;
}

void Vehicle::_AddToSlot(Unit* pPassenger, uint8 slot)
{
	assert( slot < m_seatSlotMax );

	if(pPassenger->IsPlayer() && TO_PLAYER(pPassenger)->m_CurrentCharm)
		return;

	if(pPassenger->IsPlayer() && TO_PLAYER(pPassenger)->m_isGmInvisible)
	{
		sChatHandler.GreenSystemMessage(TO_PLAYER(pPassenger)->GetSession(), "Please turn of invis before entering vehicle.");
		return;
	}

	m_passengers[ slot ] = pPassenger;
	pPassenger->m_inVehicleSeatId = slot;
	//pPassenger->m_TransporterGUID = GetGUID();
	LocationVector v;
	v.x = m_vehicleSeats[slot]->m_attachmentOffsetX; /* pPassenger->m_TransporterX =*/
	v.y = m_vehicleSeats[slot]->m_attachmentOffsetY; /* pPassenger->m_TransporterY = */
	v.z = m_vehicleSeats[slot]->m_attachmentOffsetZ; /* pPassenger->m_TransporterZ = */
	v.o = 0; /* pPassenger->m_TransporterO = */

	if( m_mountSpell )
		pPassenger->CastSpell( pPassenger, m_mountSpell, true );

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SELF_RES);

	// This is where the real magic happens
	if( pPassenger->IsPlayer() )
	{
		Player* pPlayer = TO_PLAYER(pPassenger);

		//Dismount
		if(pPlayer->m_MountSpellId)
			pPlayer->RemoveAura(pPlayer->m_MountSpellId);
	
		//Remove morph spells
		if(pPlayer->GetUInt32Value(UNIT_FIELD_DISPLAYID)!= pPlayer->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
		{
			pPlayer->RemoveAllAurasOfType(SPELL_AURA_TRANSFORM);
			pPlayer->RemoveAllAurasOfType(SPELL_AURA_MOD_SHAPESHIFT);
		}

		//Dismiss any pets
		if(pPlayer->GetSummon())
		{
			if(pPlayer->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				pPlayer->GetSummon()->Dismiss(false);				// warlock summon -> dismiss
			else
				pPlayer->GetSummon()->Remove(false, true, true);	// hunter pet -> just remove for later re-call
		}

		pPlayer->m_CurrentVehicle = TO_VEHICLE(this);

		pPlayer->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION | UNIT_FLAG_NOT_SELECTABLE));

		pPlayer->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());

		pPlayer->SetPlayerStatus(TRANSFER_PENDING);
		pPlayer->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());

		WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 100);
		data << pPlayer->GetNewGUID();							// Passengerguid
		data << GetNewGUID();									// Transporterguid (vehicleguid)
		data << uint8(slot);									// Vehicle Seat ID
		data << uint8(0);										// Unknown
		data << GetPositionX() - pPlayer->GetPositionX();		// OffsetTransporterX
		data << GetPositionY() - pPlayer->GetPositionY();		// OffsetTransporterY
		data << GetPositionZ() - pPlayer->GetPositionZ();		// OffsetTransporterZ
		data << getMSTime();									// Timestamp
		data << uint8(0x04);									// Flags
		data << float(0);										// Orientation Offset
		data << uint32(MOVEFLAG_TB_MOVED);						// MovementFlags
		data << uint32(0);										// MoveTime
		data << uint32(1);										// Points
		data << v.x;											// GetTransOffsetX();
		data << v.y;											// GetTransOffsetY();
		data << v.z;											// GetTransOffsetZ();
		SendMessageToSet(&data, true);

		DisableAI();

		if(slot == 0)
		{
			SetControllingUnit(pPlayer);
			m_redirectSpellPackets = pPlayer;

			SetSpeed(RUN, m_runSpeed);
			SetSpeed(FLY, m_flySpeed);

			// send "switch mover" packet
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << uint8(1);
			pPlayer->GetSession()->SendPacket(&data);

			pPlayer->m_CurrentCharm = TO_VEHICLE(this);
			pPlayer->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());
			SetCharmTempVal(GetFaction());
			SetFaction( pPlayer->GetFaction());
			SetUInt64Value(UNIT_FIELD_CHARMEDBY, pPlayer->GetGUID());
			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

/*			CreatureProtoVehicle* vehicleproto = CreatureProtoVehicleStorage.LookupEntry(GetEntry());
			if(vehicleproto && vehicleproto->healthfromdriver)
			{
				uint32 health = GetUInt32Value(UNIT_FIELD_HEALTH);
				uint32 maxhealth = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
				uint32 healthdiff = maxhealth - health;

				SetUInt32Value(UNIT_FIELD_MAXHEALTH, (health+((pPlayer->GetTotalItemLevel())*(vehicleproto->healthunitfromitemlev))));
				SetUInt32Value(UNIT_FIELD_HEALTH, (health+((pPlayer->GetTotalItemLevel())*(vehicleproto->healthunitfromitemlev))) - healthdiff);
			}*/

			/* update target faction set */
			_setFaction();
			UpdateOppFactionSet();

			SendSpells(GetEntry(), pPlayer);
		}

		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(m_vehicleSeats[slot]->m_enterUISoundID);
		data << pPlayer->GetPosition();
		pPlayer->GetSession()->SendPacket(&data);
		pPlayer->SetPlayerStatus(NONE);
		++m_passengerCount;
	}
	else
	{
		WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 100);
		data << pPassenger->GetNewGUID();						// Passengerguid
		data << GetNewGUID();									// Transporterguid (vehicleguid)
		data << uint8(slot);									// Vehicle Seat ID
		data << uint8(0);										// Unknown
		data << GetPositionX() - pPassenger->GetPositionX();	// OffsetTransporterX
		data << GetPositionY() - pPassenger->GetPositionY();	// OffsetTransporterY
		data << GetPositionZ() - pPassenger->GetPositionZ();	// OffsetTransporterZ
		data << getMSTime();									// Timestamp
		data << uint8(0x04);									// Flags
		data << float(0);										// Orientation Offset
		data << uint32(MOVEFLAG_TB_MOVED);						// MovementFlags
		data << uint32(0);										// MoveTime
		data << uint32(1);										// Points
		data << v.x;											// GetTransOffsetX();
		data << v.y;											// GetTransOffsetY();
		data << v.z;											// GetTransOffsetZ();
		SendMessageToSet(&data, false);
	}

	if(IsFull())
		RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
}

/* This function changes a vehicles position server side to
keep us in sync with the client, so that the vehicle doesn't
get dismissed because the server thinks its gone out of range
of its passengers*/
void Vehicle::MoveVehicle(float x, float y, float z, float o) //thanks andy
{
	SetPosition(x, y, z, o, false);
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if(m_passengers[i] != NULL)
		{
			m_passengers[i]->SetPosition(x,y,z,o,false);
		}
	}
}

void Vehicle::setDeathState(DeathState s)
{
	Creature::setDeathState(s);

	for (uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if (m_passengers[i] != NULL)
			RemovePassenger(m_passengers[i]);
	}

	if( s == JUST_DIED && m_CreatedFromSpell)
		SafeDelete();
}

void Vehicle::SetSpeed(uint8 SpeedType, float value)
{
	if( value < 0.1f )
		value = 0.1f;

	uint8 buf[200];
	StackPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, buf, 200);

	if( SpeedType != SWIMBACK )
	{
		data << GetNewGUID();
		data << uint32(0);
		if( SpeedType == RUN )
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << m_position.x;
		data << m_position.y;
		data << m_position.z;
		data << m_position.o;
		data << uint32(0);
		data << value;
	}
	
	switch(SpeedType)
	{
	case RUN:
		{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			m_runSpeed = value;
		}break;
	case RUNBACK:
		{
			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
		}break;
	case SWIM:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
		}break;
	case SWIMBACK:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			m_backSwimSpeed = value;
		}break;
	case FLY:
		{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
		}break;
	default:return;
	}
	
	SendMessageToSet(&data , true);

}

void Vehicle::ChangePowerType()
{
	if(vehicleData == NULL)
		return;
	switch(vehicleData->m_powerType)
	{
	case POWER_TYPE_MANA:
		{
		SetPowerType( POWER_TYPE_ENERGY );
		SetUInt32Value(UNIT_FIELD_POWER1, 100);
		SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
		SetPower( POWER_TYPE_ENERGY, 100 );
		}break;
	case POWER_TYPE_ENERGY:
		{
		SetPowerType( POWER_TYPE_ENERGY );
		SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
		SetPower( POWER_TYPE_ENERGY, 100 );
		}break;
	case POWER_TYPE_STEAM:
	case POWER_TYPE_HEAT:
	case POWER_TYPE_BLOOD:
	case POWER_TYPE_OOZE:
	case POWER_TYPE_WRATH:
		{
		SetPowerType( POWER_TYPE_ENERGY );
		SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
		SetPower( POWER_TYPE_ENERGY, 100 );
		}break;
	case POWER_TYPE_PYRITE:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY,50);
			SetPower( POWER_TYPE_ENERGY, 50 );
			m_interruptRegen = true;
		}break;
	default:
		{
			sLog.outError("Vehicle %u, Vehicle Entry %u has an unknown powertype %u.", GetEntry(), GetVehicleEntry(), vehicleData->m_powerType);
		}break;
	}
	Root();
}


void Vehicle::ChangeSeats(Unit* unit, uint8 seatid)
{
//	unit->m_TransporterGUID = GetGUID();
	// TODO!
}

/* This function handles the packet sent from the client when we
leave a vehicle, it removes us server side from our current
vehicle*/
void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	if(GetPlayer() == NULL || !GetPlayer()->m_CurrentVehicle)
		return;

	if(recv_data.rpos() != recv_data.wpos())
		HandleMovementOpcodes(recv_data);

	GetPlayer()->m_CurrentVehicle->RemovePassenger(GetPlayer());
}

/* This function handles the packet from the client which is
sent when we click on an npc with the flag UNIT_FLAG_SPELLCLICK
and checks if there is room for us then adds us as a passenger
to that vehicle
void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{
	if (GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle)
		return;

	CHECK_PACKET_SIZE(recv_data, 8);

	uint64 guid;
	recv_data >> guid;

	Vehicle* pVehicle = NULL;
	Unit* unit = GetPlayer()->GetMapMgr()->GetUnit(guid);
	Unit* pPlayer = TO_UNIT(GetPlayer());

	if(!unit)
		return;

	if(!unit->IsVehicle())
	{
		if(unit->IsCreature())
		{
			Creature* ctr = TO_CREATURE(unit);
			if(ctr->GetProto()->SpellClickid)
				ctr->CastSpell(pPlayer, ctr->GetProto()->SpellClickid, true);
			else
				OUT_DEBUG("[SPELL][CLICK]: Unknown spell click spell on creature %u", ctr->GetEntry());
		}
		return;
	}
	else
	{
		pVehicle = TO_VEHICLE(unit);
	}

	if(!pVehicle->GetMaxPassengerCount())
		return;

	if(!pVehicle->GetMaxSeat())
		return;

	// just in case.
	if( sEventMgr.HasEvent( pVehicle, EVENT_VEHICLE_SAFE_DELETE ) )
		return;

	if(pVehicle->HasPassenger(pPlayer))
		pVehicle->RemovePassenger(pPlayer);

	pVehicle->AddPassenger(pPlayer);
}
void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{

	if(GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle || GetPlayer()->GetMapMgr() == NULL)
		return;

	CHECK_PACKET_SIZE(recv_data, 8);

	uint64 guid = 0;
	recv_data >> guid;

	Vehicle* pVehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));
	if(!pVehicle)
		return;

	if(pVehicle->HasPassenger(_player))
		pVehicle->RemovePassenger(_player);
	pVehicle->AddPassenger(_player);
}
*/
void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{
	if (GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle)
		return;

	CHECK_PACKET_SIZE(recv_data, 8);

	uint64 target_guid;
	recv_data >> target_guid;

	Vehicle* pVehicle = NULL;
	Unit* target_unit = GetPlayer()->GetMapMgr()->GetUnit(target_guid);
	Unit* pPlayer = TO_UNIT(GetPlayer());

	if(!target_unit)
		return;

	if(!target_unit->IsVehicle())
	{
		uint32 creature_id = target_unit->GetEntry();
		uint32 cast_spell_id = 0;

		if(!_player->GetAuraSpellIDWithNameHash(SPELL_HASH_LIGHTWELL_RENEW) && target_unit->RemoveAura(59907))
		{
			SpellClickSpell *sp = SpellClickSpellStorage.LookupEntry( creature_id );
			if( sp == NULL ){
				if( target_unit->IsCreature() ){
					Creature *c = static_cast< Creature* >( target_unit );
				
					sChatHandler.BlueSystemMessage( this, "NPC Id %u ( %s ) has no spellclick spell associated with it.", c->GetProto()->Id, c->GetCreatureInfo()->Name  );
					sLog.outError("Spellclick packet received for creature %u but there is no spell associated with it.", creature_id );
					return;
				}
			}

			cast_spell_id = sp->SpellID;

			target_unit->CastSpell(_player, cast_spell_id, true);

			if(!target_unit->HasAura(59907))
				TO_CREATURE(target_unit)->Despawn(0, 0); //IsCreature() check is not needed, refer to r2387 and r3230

			return;
		}

		SpellClickSpell *sp = SpellClickSpellStorage.LookupEntry( creature_id );
		if( sp == NULL ){
			if( target_unit->IsCreature() ){
				Creature *c = static_cast< Creature* >( target_unit );

				sChatHandler.BlueSystemMessage( this, "NPC Id %u ( %s ) has no spellclick spell associated with it.", c->GetProto()->Id, c->GetCreatureInfo()->Name  );
				sLog.outError("Spellclick packet received for creature %u but there is no spell associated with it.", creature_id );
				return;
			}
		}

		cast_spell_id = sp->SpellID;

		if(cast_spell_id == 0)
			return;

		SpellEntry* spellInfo = dbcSpell.LookupEntryForced(cast_spell_id);
		if(spellInfo == NULL)
		return;
		Spell* spell = new Spell(_player, spellInfo, false, NULL);
		SpellCastTargets targets(target_guid);
		spell->prepare(&targets);
	}
	else
	{
		pVehicle = TO_VEHICLE(target_unit);
	}

	if(!pVehicle->GetMaxPassengerCount())
		return;

	if(!pVehicle->GetMaxSeat())
		return;

	// just in case.
	if( sEventMgr.HasEvent( pVehicle, EVENT_VEHICLE_SAFE_DELETE ) )
		return;

	if(pVehicle->HasPassenger(pPlayer))
		pVehicle->RemovePassenger(pPlayer);

	pVehicle->AddPassenger(pPlayer);
}