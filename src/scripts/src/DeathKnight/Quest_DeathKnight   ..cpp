/*
Please read the copyright and license file in the root directory.
*/

#include "Setup.h"

/* The Endless Hunger script for both GO and Unworthy Initiate */

#define CN_INITIATE_1				29519
#define CN_INITIATE_2				29565
#define CN_INITIATE_3				29567
#define CN_INITIATE_4				29520
#define CN_INITIATE_5				29566


//Chapter One

#define SAY_START_1	"You have made a grave error, fiend!"
#define SAY_START_2 "I was a soldier of the Light once... Look at what I have become..."
#define SAY_START_3 "You are hopelessly outmatched, $R."
#define SAY_START_4 "They brand me unworthy? I will show them unmorthy!"
#define SAY_START_5 "You will allow me a weapon and armor, yes?"
#define SAY_START_6 "I will win my freedom and leave this cursed place!"
#define SAY_START_7 "I will dismantle this festering hellhole!"
#define SAY_START_8 "There can be only one survivor!"

class AcherusSoulPrison : GameObjectAIScript
{
	public:
	/*	ADD_GAMEOBJECT_FACTORY_FUNCTION(AcherusSoulPrison)
		AcherusSoulPrison(GameObject * goinstance) : GameObjectAIScript(goinstance) {}*/
		AcherusSoulPrison(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript * Create(GameObject* GO)
		{
			return new AcherusSoulPrison(GO);
		}


		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12848 );
		//	if( !en || en->GetMobCount(0) != 0 )
			if( !en )
				return; 

			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();

			Creature * pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ);

			if(!pCreature || !pCreature->isAlive())
				return;

			if(pCreature->GetEntry() == CN_INITIATE_1 || 
			   pCreature->GetEntry() == CN_INITIATE_2 ||
			   pCreature->GetEntry() == CN_INITIATE_3 || 
			   pCreature->GetEntry() == CN_INITIATE_4 ||
			   pCreature->GetEntry() == CN_INITIATE_5 )
			{
				// this way we have refference to our player
				pPlayer->SendChatMessage(CHAT_MSG_SAY, LANG_UNIVERSAL, "I give you the key to your salvation");
				pCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, pPlayer->GetGUID() );

			switch(rand()%8)
			{
			case 0: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_1); break;
			case 1: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_2); break;
			case 2: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_3); break;
			case 3: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_4); break;
			case 4: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_5); break;
			case 5: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_6); break;
			case 6: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_7); break;
			case 7: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_8); break;
			}

				// do all other things directly in creature's AI class, it's more comfortable
				pCreature->GetScript()->RegisterAIUpdateEvent(1000);
			}
		}
};

    //SPELL_CHAINED_PESANT_LH        54602,              // not used. possible it determine side, where to go get "weapon"
    //SPELL_CHAINED_PESANT_RH        54610,
#define    SPELL_CHAINED_PESANT_CHEST      54612
#define    SPELL_CHAINED_PESANT_BREATH     54613
#define    SPELL_INITIATE_VISUAL           51519

#define    SPELL_BLOOD_STRIKE              52374
#define    SPELL_DEATH_COIL                52375
#define    SPELL_ICY_TOUCH                 52372
#define    SPELL_PLAGUE_STRIKE             52373

#define    NPC_ANCHOR                      29521
#define    FACTION_MONSTER                 16

#define    PHASE_INACTIVE                  0
#define    PHASE_STANDUP                   1
#define    PHASE_REACH_ARMORY              2
#define    PHASE_ACTIVATE                  3
#define    PHASE_ATTACK_PLAYER             4
#define    PHASE_ATTACKING                 5

struct DisplayToSpell
{
	uint32 displayid;
	uint32 spellid;
};

#define SAY_ATTACK_1 "To battle!"
#define SAY_ATTACK_2 "Let your fears consume you!"
#define SAY_ATTACK_3 "HAH! You can barely hold a blade! Yours will be a quick death."
#define SAY_ATTACK_4 "And now you die"
#define SAY_ATTACK_5 "To battle!"
#define SAY_ATTACK_6 "There is no hope for our future..."
#define SAY_ATTACK_7 "Sate your hunger on cold steel, $R"
#define SAY_ATTACK_8 "It ends here!"
#define SAY_ATTACK_9 "Death is the only cure!"

DisplayToSpell m_DisplayToSpell[] =
{
    {25354, 51520},                                         // human M
    {25355, 51534},                                         // human F
    {25356, 51538},                                         // dwarf M
    {25357, 51541},                                         // draenei M
    {25358, 51535},                                         // nelf M
    {25359, 51539},                                         // gnome M
    {25360, 51536},                                         // nelf F
    {25361, 51537},                                         // dwarf F
    {25362, 51540},                                         // gnome F
    {25363, 51542},                                         // draenei F
    {25364, 51543},                                         // orc M
    {25365, 51546},                                         // troll M
    {25366, 51547},                                         // tauren M
    {25367, 51549},                                         // forsaken M
    {25368, 51544},                                         // orc F
    {25369, 51552},                                         // belf F
    {25370, 51545},                                         // troll F
    {25371, 51548},                                         // tauren F
    {25372, 51550},                                         // forsaken F
    {25373, 51551}                                          // belf M
};

static Location m_ArmoryLocations[] = 
{
	{ 2461.047363f, -5570.428223f, 415.664490f, 1.735095f },
	{ 2472.961670f, -5575.300781f, 415.429169f, 0.368502f },
	{ 2474.302246f, -5576.693848f, 415.394287f, 1.012529f },
	{ 2480.435547f, -5584.607422f, 415.663696f, 0.328443f },
	{ 2482.882324f, -5590.556152f, 415.663605f, 0.383420f },
	{ 2448.824219f, -5612.786621f, 415.664154f, 3.540719f },
	{ 2447.006104f, -5611.062500f, 415.664154f, 4.298627f },
	{ 2440.834229f, -5604.432617f, 415.664154f, 4.243647f },
	{ 2437.441895f, -5590.024414f, 415.663879f, 2.822077f },
	{ 2439.216797f, -5585.857422f, 415.663879f, 2.802442f },
};

class UnworthyInitiate : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(UnworthyInitiate, MoonScriptCreatureAI);
		UnworthyInitiate(Creature * c) : MoonScriptCreatureAI(c)
		{
			AddSpell(SPELL_BLOOD_STRIKE, Target_Current, 85.0f, 0.0f, 4000);
			AddSpell(SPELL_DEATH_COIL, Target_Current, 85.f, 0.0f, 6000);
			AddSpell(SPELL_ICY_TOUCH, Target_Current, 85.f, 0.0f, 2000);
			AddSpell(SPELL_PLAGUE_STRIKE, Target_Current, 85.f, 0.0f, 5000);
		}

		void OnLoad()
		{
			RegisterAIUpdateEvent(1000);
			_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 33024); // from database
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 7);
			_unit->SetStandState( uint8(STANDSTATE_KNEEL) );
			SetDisplayWeapon(false, false);
			state = -1;
			anchorGuid = 0;
			ParentClass::OnLoad();
		}

		void CheckForAnchor()
		{
			// attach nearest prison and cast spell on it
			Creature * anchor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), 0.0f, NPC_ANCHOR);
			if( anchor )
			{
				anchor->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID() );
				anchor->SetUInt32Value(UNIT_CHANNEL_SPELL,  SPELL_CHAINED_PESANT_BREATH );
				anchor->CastSpell(_unit, SPELL_CHAINED_PESANT_BREATH, false);
				anchorGuid = anchor->GetGUID();
			}
			
			_unit->CastSpell(_unit, SPELL_CHAINED_PESANT_CHEST, false);
		}

		void OnCombatStart(Unit* mTarget)
			{
				switch(rand()%8)
				{
				case 0: Emote(SAY_ATTACK_1); break;
				case 1: Emote(SAY_ATTACK_2); break;
				case 2: Emote(SAY_ATTACK_3); break;
				case 3: Emote(SAY_ATTACK_4); break;
				case 4: Emote(SAY_ATTACK_5); break;
				case 5: Emote(SAY_ATTACK_6); break;
				case 6: Emote(SAY_ATTACK_7); break;
				case 7: Emote(SAY_ATTACK_8); break;
				case 9: Emote(SAY_ATTACK_9); break;
				}
			 }

		void AIUpdate()
		{
			if( anchorGuid == 0 )
			{
				CheckForAnchor();
				if( anchorGuid != 0 )
				{
					state = PHASE_INACTIVE;
					RemoveAIUpdateEvent();
					return;
				}
			}

			if( state == PHASE_INACTIVE )
			{
				_unit->SetStandState( uint8(STANDSTATE_STAND) );
				_unit->RemoveAllAuras();

		//		Player * plr = _unit->GetMapMgrPlayer( _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY ) );
		//		if( plr )
		//			SetFacingToObject( plr );

				Creature * anchor = _unit->GetMapMgr()->GetCreature(GET_LOWGUID_PART( anchorGuid ) );
				if( anchor )
				{
					anchor->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0 );
					anchor->SetUInt32Value(UNIT_CHANNEL_SPELL,  0 );
					//anchor->GetCurrentSpell()->cancel();
				}
		
				timer = AddTimer(1000);
				state = PHASE_STANDUP;
			}
			else if( state == PHASE_STANDUP && IsTimerFinished(timer) )
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have committed a big mistake, demon");
				timer = AddTimer(5000);
				state = PHASE_REACH_ARMORY;
			}
			else if( state == PHASE_REACH_ARMORY && IsTimerFinished(timer) )
			{
				// test for nearest coords of armory
				uint32 nearest_entry = 0;
				uint32 nearest_distance = 1000;
				for(uint32 i = 0; i < sizeof(m_ArmoryLocations) / sizeof(Location); i++)
				{
					uint32 distance = _unit->CalcDistance(m_ArmoryLocations[i].mX, m_ArmoryLocations[i].mY, m_ArmoryLocations[i].mZ);

					if( distance < nearest_distance )
					{
						nearest_distance = distance;
						nearest_entry = i;
					}
				}
				
				// now go to nearest armory
				MoveTo(m_ArmoryLocations[nearest_entry].mX, m_ArmoryLocations[nearest_entry].mY, m_ArmoryLocations[nearest_entry].mZ, true);
				timer = AddTimer(2000);
				state = PHASE_ACTIVATE;
			}
			else if( state == PHASE_ACTIVATE && IsTimerFinished(timer) )
			{
				// face off the player
		//		Player * plr = _unit->GetMapMgrPlayer( _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY ) );
		//		if( plr )
		//			SetFacingToObject( plr );

				// select suitable spell
				uint32 spell_to_cast = 0;
				for(uint32 i = 0; i < sizeof(m_DisplayToSpell) / sizeof(DisplayToSpell); i++)
				{
					if( m_DisplayToSpell[i].displayid == _unit->GetUInt32Value(UNIT_FIELD_DISPLAYID) )
					{
						spell_to_cast = m_DisplayToSpell[i].spellid;
						break;
					}
				}

				// change look
				_unit->CastSpell(_unit, spell_to_cast, false);
				_unit->CastSpell(_unit, 48266, false); // blood presence
				timer = AddTimer(2000);
				state = PHASE_ATTACK_PLAYER;
				// he equips same weapon as normal DK?
				SetDisplayWeaponIds(38707, 0);
			}
			else if( state == PHASE_ATTACK_PLAYER && IsTimerFinished(timer) )
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "And now you die!");
				state = PHASE_ATTACKING; // we are fighting
				timer = AddTimer(1000);
			}
			else if( state == PHASE_ATTACKING && IsTimerFinished(timer) )
			{
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 16 );

				Player * plr = _unit->GetMapMgr()->GetPlayer(GET_LOWGUID_PART( _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY )) );
				if( plr )
				{
					SetCanEnterCombat( true );
					_unit->GetAIInterface()->AttackReaction( plr, 500, 0 );
					_unit->GetAIInterface()->SetNextTarget( plr );
					_unit->GetAIInterface()->SetAIState(STATE_ATTACKING);
				//	_unit->GetAIInterface()->EventEnterCombat( plr, 0 );
					_unit->GetAIInterface()->HandleEvent(EVENT_ENTERCOMBAT, plr, 0 );
				}

				state = -1;
				//_unit->SetUInt32Value( UNIT_FIELD_FLAGS, 0 );
			}

			ParentClass::AIUpdate();
		}

		void OnDied(Unit * mKiller)
		{
			if( !mKiller->IsPlayer() )
				return;

			// don't credit if any other player kills my initiate
			if( mKiller->GetGUID() != _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY ) )
				return;

			Player * plr = TO_PLAYER(mKiller);
			QuestLogEntry * qle = plr->GetQuestLogForEntry(12848);
			if( !qle )
				return;

			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
			qle->SendQuestComplete();

			ParentClass::OnDied(mKiller);
		}

		void OnCombatStop(Unit * pTarget)
		{
			_unit->Despawn(1000, 1000);
		}

		int32 timer;
		int8 state;
		uint64 anchorGuid;
};


void SetupDeathKnight(ScriptMgr* mgr)
{

	//CHAPTER 1

//	uint32 acherusSouldPrisonEntries[] = { 191588, 191577, 191580, 191581, 191582, 191583, 191584, 191585, 191586, 191587, 191589, 191590, 0 };
	mgr->register_gameobject_script(191588, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191577, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191580, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191581, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191582, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191583, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191584, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191585, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191586, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191587, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191589, &AcherusSoulPrison::Create);
	mgr->register_gameobject_script(191590, &AcherusSoulPrison::Create);

	mgr->register_creature_script(CN_INITIATE_1, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_2, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_3, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_4, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_5, &UnworthyInitiate::Create);

}