
# Applications ---------------------------------------------------------------

add_executable(test_application 
    test_application/test_application.c
    test_application/test_application_interfaces.c
)
target_include_directories(test_application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/c")
target_include_directories(test_application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(test_application PRIVATE psf sofa m)
