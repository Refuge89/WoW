#child CMakeLists use this variable to access the source file directories. 
#e.g ${SCRIPTS_PATH}/LuaEngine to access the source files for LuaEngine.

if( NOT BUILD_SCRIPTS )
  message( WARNING, "BUILD_SCRIPTS option is set to false. Script libraries will not be built." )
  return()
endif(NOT BUILD_SCRIPTS)
message("Configuring scripts :")
SET( SCRIPTS_PATH ${ROOT_PATH}/src/scripts/src )
#common include directories that script libraries might include.
SET( include_dirs ${GLOBAL_INCLUDE_DIRS} )
SET( dependancies shared world )
SET( linkages world)

#All script libraries have SCRIPTLIB defined.
add_definitions(-DSCRIPTLIB -DSKIP_ALLOCATOR_SHARING)
SET( BUILD_EVENTSSCRIPTS TRUE CACHE BOOL "Build arcEventsScripts.")
SET( BUILD_DEATHKNIGHT TRUE CACHE BOOL "Build arcDeathKnight.")
SET( BUILD_INSTANCESCRIPTS TRUE CACHE BOOL "Build arcInstanceScripts.")
SET( BUILD_RAIDSSCRIPTS TRUE CACHE BOOL "Build arcRaidsScripts." )
SET( BUILD_EXTRASCRIPTS TRUE CACHE BOOL "Build arcExtraScripts.")
SET( BUILD_GOSSIPSCRIPTS TRUE CACHE BOOL "Build arcGossipScripts.")
SET( BUILD_QUESTSCRIPTS TRUE CACHE BOOL "Build arcQuestScripts.")
SET( BUILD_MISCSCRIPTS TRUE CACHE BOOL "Build arcMiscScripts." )
SET( BUILD_WORLDPVPSCRIPTS TRUE CACHE BOOL "Build arcWorldPvPScripts." )
SET( BUILD_SERVERSTATUSPLUGIN TRUE CACHE BOOL "Build ServerStatusPlugin.")
SET( BUILD_LUAENGINE TRUE CACHE BOOL "Build  Lua Engine ( ALE ).")
SET( BUILD_LUABRIDGE FALSE CACHE BOOL "Build luabridge.")


add_subdirectory( eventsscripts)
add_subdirectory( deathknight )
add_subdirectory( instancescripts )
add_subdirectory( raidsscripts )
add_subdirectory( extrascripts )
add_subdirectory( gossipscripts )
add_subdirectory( questscripts )
add_subdirectory( miscscripts )
add_subdirectory( worldpvpscripts )
add_subdirectory( serverstatusplugin )
add_subdirectory( lualib ) #Used by luabridge and luaengine modules.
add_subdirectory( luabridge ) 
add_subdirectory( luaengine )

unset(SCRIPTS_PATH)
unset(include_dirs)