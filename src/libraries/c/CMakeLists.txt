
##############################################################################
# EXTERNAL LIBRARIES ---------------------------------------------------------
##############################################################################

# SOFA
file(GLOB SOFA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/sofa/*.c")
add_library(sofa STATIC ${SOFA_SOURCES})
target_include_directories(sofa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(sofa PUBLIC c_std_23)

# CJSON
file(GLOB CJSON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/cjson/*.c")
add_library(cjson STATIC ${CJSON_SOURCES})
target_include_directories(cjson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(cjson PUBLIC c_std_23)

# SGP4
file(GLOB SGP4_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/sgp4/*.c")
add_library(sgp4 STATIC ${SGP4_SOURCES})
target_include_directories(sgp4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(sgp4 PUBLIC c_std_23)

##############################################################################
# INTERNAL LIBRARIES ---------------------------------------------------------
##############################################################################

add_library(psf STATIC

    file-io/internal-products/parameter-evolution-file/parameter_evolution_file.c
    file-io/internal-products/read-json/read_json.c
    file-io/internal-products/application_output/application_output.c

    mathematics-library/linear-algebra/matrix-operations/matrix_operations.c
    mathematics-library/numerical-methods/runge-kutta-4/runge_kutta_4.c
    mathematics-library/numerical-methods/interpolation/interpolation.c

    orbit-dynamics/equations-of-motion/two-body-problem/two_body_problem.c

    reference-systems/coordinate-systems/terrestrial-frames/terrestrial_frames.c
    reference-systems/coordinate-systems/orbital-frames/orbital_frames.c
    reference-systems/time-systems/time-formats/time_formats.c
    reference-systems/time-systems/epoch-transformations/epoch_transformations.c

    utilities/logging/log/log.c
    utilities/misc/parse-cmdline/parse_cmdline.c
    utilities/misc/paths/paths.c
)

target_link_libraries(psf PUBLIC sofa cjson)

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
