SET(ARCEMU_TOOLS_PATH "tools" CACHE PATH "The directory where you want the tools installed." )

if( BUILD_MAP_TOOLS )
#   dependencies
    add_subdirectory( bzip2 )
    add_subdirectory( libmpq_new )
    add_subdirectory( dbcfile )
    add_subdirectory( loadlib )

#   projects
	add_subdirectory( map_tools )
	add_subdirectory( vmap_tools )
else()
    message( WARNING, "BUILD_MAP_TOOLS option is set to false. M will not be built." )
endif( BUILD_MAP_TOOLS )

# This should be moved to external project
#add_subdirectory( creature_data )