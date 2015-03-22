/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2010 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the AscentNG Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the AscentNG is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

//#define DEG2RAD (M_PI/180.0)
#ifdef M_PI
#undef M_PI
#endif

#define M_PI		3.14159265358979323846f
#define M_H_PI		1.57079632679489661923f
#define M_Q_PI		0.785398163397448309615f
#define M_PI_FLOAT 3.14159f

#define _GAME

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "../shared/Common.h"
#include "../shared/MersenneTwister.h"
#include "../shared/WorldPacket.h"
#include "../shared/Log.h"
#include "../shared/NGLog.h"
#include "../shared/ByteBuffer.h"
#include "../shared/StackBuffer.h"
#include "../shared/Config/ConfigEnv.h"
#include "../shared/crc32.h"
#include "../shared/LocationVector.h"
#include "../shared/hashmap.h"

#include "../shared/Collision/vmap/IVMapManager.h"
#include "../shared/Collision/vmap/VMapManager.h"

#include <zlib.h>

#include "../shared/Database/DatabaseEnv.h"
#include "../shared/Database/DBCStores.h"
#include "../shared/Database/dbcfile.h"

#include <Network/Network.h>

#include "../shared/Auth/MD5.h"
#include "../shared/Auth/BigNumber.h"
#include "../shared/Auth/Sha1.h"
#include "../shared/Auth/WowCrypt.h"
#include "../shared/CrashHandler.h"
#include "../shared/FastQueue.h"
#include "../shared/CircularQueue.h"
#include "../shared/Threading/RWLock.h"
#include "../shared/Threading/Condition.h"
#include "../shared/ascent_getopt.h"
#include "../shared/BufferPool.h"

#include "Const.h"
#include "NameTables.h"
#include "Player_Systems/UpdateFields.h"
#include "Player_Systems/UpdateMask.h"
#include "Client_Communications/Opcodes.h"
#include "Map_System/Environment/WorldStates.h"

#ifdef CLUSTERING
	#include "../realmserver/WorkerOpcodes.h"
#endif

#include "Client_Communications/Packet_Handlers/Packets.h"

#include "../shared/CallBack.h"
#include "Chat/WordFilter.h"
#include "Intersystem/Management/EventMgr.h"
#include "Objects/EventableObject.h"
#include "Objects/Object.h"
#include "Intersystem/Management/LootMgr.h"
#include "Objects/Units/Unit.h"

#include "Intersystem/Management/AddonMgr.h"
#include "AI/AIInterface.h"
#include "Map_System/Environment/AreaTrigger.h"
#include "Battlegrounds/BattlegroundMgr.h"
#include "Battlegrounds/Expansion0/AlteracValley.h"
#include "Battlegrounds/Expansion0/ArathiBasin.h"
#include "Battlegrounds/Expansion1/EyeOfTheStorm.h"
#include "Battlegrounds/Arenas.h"
#include "Map_System/Map_Interface/CellHandler.h"
#include "Chat/Commands/Chat.h"
#include "Objects/Corpse.h"
#include "Player_Systems/Quest.h"
#include "Intersystem/Management/QuestMgr.h"
#include "Objects/Units/Creature.h"
#include "Objects/DynamicObject.h"
#include "Objects/GameObject.h"
#include "Player_Systems/Group.h"
#include "Intersystem/Management/Guild/Guild.h"
#include "Client_Communications/Packet_Handlers/HonorHandler.h"
#include "Intersystem/Management/Items/ItemPrototype.h"
#include "Objects/Item.h"
#include "Objects/Container.h"
#include "Player_Systems/AuctionHouse.h"
#include "Intersystem/Management/AuctionMgr.h"
#include "Intersystem/Management/LfgMgr.h"
#include "Player_Systems/MailSystem.h"
#include "Map_System/Map_Interface/Map.h"
#include "Map_System/Map_Interface/MapCell.h"
#include "Map_System/Map_Interface/TerrainMgr.h"
#include "Client_Communications/Packet_Handlers/MiscHandler.h"
#include "Client_Communications/Packet_Handlers/NPCHandler.h"
#include "Objects/Units/Pet.h"
#include "Client_Communications/WorldSocket.h"
#include "Mainframe/World.h"
#include "Client_Communications/WorldSession.h"
#include "Map_System/Environment/WorldStateManager.h"
#include "Map_System/Map_Managers/MapMgr.h"
#include "Map_System/Map_Managers/MapScriptInterface.h"
#include "Battlegrounds/Expansion2/WintergraspInternal.h"
#include "Battlegrounds/Expansion2/Wintergrasp.h"
#include "Objects/Units/Player.h"
#include "Player_Systems/faction.h"
#include "Player_Systems/Skill.h"
#include "Intersystem/Management/SkillNameMgr.h"
#include "Spell/SpellNameHashes.h"
#include "Spell/Spell.h"
#include "Spell/SpellAuras.h"
#include "Intersystem/Management/TaxiMgr.h"
#include "Client_Communications/Packet_Handlers/TransporterHandler.h"
#include "Battlegrounds/Expansion0/WarsongGulch.h"
#include "Map_System/Environment/WeatherMgr.h"
#include "Intersystem/Management/Items/ItemInterface.h"
#include "Player_Systems/Stats.h"
#include "Mainframe/ObjectMgr.h"
#include "Map_System/Map_Managers/WorldCreator.h"
#include "ScriptInterface/ScriptMgr.h"
#include "Chat/Channel.h"
#include "Chat/ChannelMgr.h"
#include "Battlegrounds/ArenaTeam.h"
#include "Intersystem/Logon/LogonCommClient.h"
#include "Intersystem/Logon/LogonCommHandler.h"
#include "MainServerDefines.h"
#include "Map_System/WorldRunnable.h"
#include "../shared/Storage.h"
#include "Intersystem/ObjectStorage.h"
#include "Intersystem/DayWatcherThread.h"
#include "Client_Communications/Packet_Handlers/VoiceChatHandler.h"
#include "Objects/Units/Vehicle.h"
#include "Player_Systems/AchievementInterface.h"
#include "Battlegrounds/Expansion2/ChainAggroEntity.h"
#include "Battlegrounds/Expansion2/StrandOfTheAncients.h"
#include "Battlegrounds/Expansion2/TheIsleOfConquest.h"
#include "Battlegrounds/Expansion2/Wintergrasp.h"

#ifdef CLUSTERING
	#include "WorkerServerClient.h"
	#include "ClusterInterface.h"
#endif

#include "Map_System/Map_Tiles/CollideInterface.h"

#include "Mainframe/Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
//#define COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS 1

#endif
