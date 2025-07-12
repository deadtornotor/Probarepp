cmake_minimum_required(VERSION 3.10)

set(MAIN_PROJECT OFF)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

project(probarepp LANGUAGES CXX VERSION 1.0.1)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectFunctions.cmake")

# Option to enable/disable test targets
option(PROBAREPP_TESTS "Build and enable tests" ON)
# Option to enable/disable precompiled header
option(PROBARE_USE_PCH "Enable precompiled headers for probare" ON)

file(GLOB_RECURSE PROBARE_SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[ch]pp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[ch]"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# --- Library target: probarepp ---
add_library(
    probarepp STATIC
    ${PROBARE_SRC_FILES}
    "${CMAKE_CURRENT_SOURCE_DIR}/include/probare"
)

probare_setup_target_config_types(probarepp)

if(PROBARE_USE_PCH)
    target_precompile_headers(probarepp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/probare")
endif()

# Mark the header as a public interface
target_include_directories(probarepp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(PROBAREPP_TESTS)
    # Enables ctest
    enable_testing()

    # Guard to ensure build_tests is only defined once
    if(NOT TARGET build_tests)
        add_custom_target(build_tests)
    endif()

    # Guard to ensure tests is only defined once
    if(NOT TARGET tests)
        add_custom_target(tests
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()

    add_probare_test(probarepp)
    probare_setup_target_config_types(probarepp_test)
    add_dependencies(tests build_tests)
endif()
