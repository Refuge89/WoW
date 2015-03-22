if(NOT BUILD_SERVERSTATUSPLUGIN )
  message("\tServerStatusPlugin was skipped because BUILD_SERVERSTATUSPLUGIN is unchecked.")
  return()
endif(NOT BUILD_SERVERSTATUSPLUGIN)

PROJECT(ServerStatusPlugin)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

SET( sources 
ServerStatusPlugin.cpp )

SET( headers
PerfCounters.h )

foreach(src IN ITEMS ${sources} )
  set( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} ${HEADERS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${SCRIPTLIB_PATH} LIBRARY DESTINATION ${SCRIPTLIB_PATH})
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
