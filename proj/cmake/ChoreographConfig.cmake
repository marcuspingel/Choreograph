if( NOT TARGET Choreograph )
  get_filename_component( CHOREOGRAPH_BLOCK_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE )
  get_filename_component( CHOREOGRAPH_INCLUDE_PATH "${CHOREOGRAPH_BLOCK_PATH}/src" ABSOLUTE )
  
  if( NOT EXISTS ${CINDER_PATH} )
    get_filename_component( CINDER_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE )
  endif()

  add_library( Choreograph
    ${CHOREOGRAPH_INCLUDE_PATH}/choreograph/Cue.cpp
    ${CHOREOGRAPH_INCLUDE_PATH}/choreograph/Timeline.cpp
    ${CHOREOGRAPH_INCLUDE_PATH}/choreograph/TimelineItem.cpp
    ${CHOREOGRAPH_INCLUDE_PATH}/choreograph/UnitBezier.cpp
  )

  #target_compile_options( Choreograph PUBLIC "-std=c++11" )

  target_include_directories( Choreograph PUBLIC "${CHOREOGRAPH_INCLUDE_PATH}" )
  target_include_directories( Choreograph PUBLIC "${CHOREOGRAPH_INCLUDE_PATH}/choreograph" )

  if( NOT TARGET cinder )
        include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
        find_package( cinder REQUIRED PATHS
            "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
            "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
  endif()
  target_link_libraries( Choreograph PRIVATE cinder )
  
endif()
