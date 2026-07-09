
# Applications ---------------------------------------------------------------

add_executable(test_application 
    test_application/test_application.c
    test_application/test_application_interfaces.c
)
target_include_directories(test_application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/c")
target_include_directories(test_application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(test_application PRIVATE psf sofa m)

add_executable(tlevision 
    tlevision/tlevision.c
    tlevision/tlevision_interfaces.c
    tlevision/tlevision_load_timespan.c
)
target_include_directories(tlevision PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/c")
target_include_directories(tlevision PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(tlevision PRIVATE psf sofa m sgp4)
