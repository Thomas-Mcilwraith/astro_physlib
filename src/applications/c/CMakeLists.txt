
# Applications ---------------------------------------------------------------

add_executable(test_application
    test_application/test_application.c
)

# Linker settings ------------------------------------------------------------

target_link_libraries(test_application PRIVATE
    psf
)
