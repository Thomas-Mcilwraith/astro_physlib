
# Applications ---------------------------------------------------------------

add_executable(test_application
    test_application/test_application.c
)

# Linker settings ------------------------------------------------------------

target_include_directories(test_application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/c")
target_link_libraries(test_application PRIVATE psf sofa m)
