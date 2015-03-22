PROJECT(Battlegrounds)

SET( prefix ${ROOT_PATH}/src/scripts/${PROJECT_NAME} )

#sources -contains source files for our library.
SET( sources
Setup.cpp
ZoneSilithus.cpp
ZoneHellfirePeninsula.cpp
ZoneTerokkarForest.cpp
ZoneZangarmarsh.cpp
../Common/Instance_Base.cpp
../Common/Base.cpp
 )

SET( headers
Setup.h
../Common/Base.h
../Common/EasyFunctions.h
../Common/Instance_Base.h
 )
 
foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} ${HEADERS})
add_dependencies( ${PROJECT_NAME} shared world )
add_definitions( -DSCRIPTLIB -DSKIP_ALLOCATOR_SHARING )
target_link_libraries( ${PROJECT_NAME} world )

include(PCHSupport)
IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} Setup.h ${prefix}/Setup.cpp)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${SCRIPTLIB_PATH} LIBRARY DESTINATION ${SCRIPTLIB_PATH} )
#Cleap up our variables(Not required.)
unset( prefix )
unset( SRCS )
unset( HEADERS )
unset( headers )
unset( sources )
