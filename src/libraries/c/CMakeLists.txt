
##############################################################################
# EXTERNAL LIBRARIES ---------------------------------------------------------
##############################################################################

# SOFA
file(GLOB SOFA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/sofa/*.c")
add_library(sofa STATIC ${SOFA_SOURCES})
target_include_directories(sofa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(sofa PUBLIC c_std_23)

##############################################################################
# INTERNAL LIBRARIES ---------------------------------------------------------
##############################################################################

add_library(psf STATIC

    file-io/internal-products/parameter-evolution-file/parameter_evolution_file.c

    mathematics-library/linear-algebra/matrix-operations/matrix_operations.c
    mathematics-library/numerical-methods/runge-kutta-4/runge_kutta_4.c

    orbit-dynamics/equations-of-motion/two-body-problem/two_body_problem.c

    reference-systems/coordinate-systems/terrestrial-frames/terrestrial_frames.c
    reference-systems/coordinate-systems/orbital-frames/orbital_frames.c
    reference-systems/time-systems/time-formats/time_formats.c
    reference-systems/time-systems/universal-time/universal_time.c

    utilities/logging/log/log.c
)

target_link_libraries(psf PUBLIC sofa)

# Compiler settings ----------------------------------------------------------

target_compile_features(psf PUBLIC c_std_23)
target_compile_options(psf PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

# Linker settings ------------------------------------------------------------

target_include_directories(psf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
