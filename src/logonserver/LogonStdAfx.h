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

#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "Common.h"
#include <Network/Network.h>

#include "../shared/Log.h"
#include "../shared/Util.h"
#include "../shared/ByteBuffer.h"
#include "../shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../shared/Database/DatabaseEnv.h"
#include "../world/DBC/DBCStores.h"
#include "dbcfile/dbcfile.h"

#include "../shared/Auth/BigNumber.h"
#include "../shared/Auth/Sha1.h"
#include "../shared/Auth/WowCrypt.h"
#include "../shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../logonserver/Main.h"
#include "../world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../logonserver/AutoPatcher.h"
#include "../logonserver/AuthSocket.h"
#include "../logonserver/AuthStructs.h"
#include "../logonserver/LogonOpcodes.h"
#include "../logonserver/LogonCommServer.h"
#include "../logonserver/LogonConsole.h"
#include "../shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
