# ProjectFunctions.cmake
# Utility functions for Probare

function(_include_if_exists TARGET VISIBILITY DIR)
    if(EXISTS "${DIR}")
        target_include_directories(${TARGET} ${VISIBILITY} "${DIR}")
    endif()
endfunction()

function(add_probare_test TARGET_NAME)
    set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

    if(EXISTS "${TESTS_DIR}")
        file(GLOB_RECURSE TEST_SRC_FILES CONFIGURE_DEPENDS "${TESTS_DIR}/*.[ch]pp" "${TESTS_DIR}/*.[ch]")
        if(TEST_SRC_FILES)
            set(TEST_TARGET "${TARGET_NAME}_test")
            add_executable(${TEST_TARGET} ${TEST_SRC_FILES})
            target_link_libraries(${TEST_TARGET} PRIVATE ${TARGET_NAME} probarepp)

            _include_if_exists(${TEST_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
            _include_if_exists(${TEST_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
            _include_if_exists(${TEST_TARGET} PRIVATE "${TESTS_DIR}")

            add_test(NAME ${TARGET_NAME} COMMAND ${TEST_TARGET})
            add_dependencies(build_tests ${TEST_TARGET})
        endif()
    else()
        message(WARNING "${TARGET_NAME} does not have a tests directory in ${TESTS_DIR}")
    endif()
endfunction()
