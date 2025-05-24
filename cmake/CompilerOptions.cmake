# CompilerOptions.cmake
# Centralized compiler flags for all configurations and platforms

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(add_compile_flags_for_config CONFIG)
    foreach(FLAG IN LISTS ARGN)
        add_compile_options("$<$<CONFIG:${CONFIG}>:${FLAG}>")
    endforeach()
endfunction()

if(MSVC)
    set(COMMON_WARNINGS_LIST /W4)
    add_compile_flags_for_config(Debug ${COMMON_WARNINGS_LIST} /Od /Zi)
    add_compile_flags_for_config(Release /O2)
    add_compile_flags_for_config(RelWithDebInfo /O2 /Zi)
    add_compile_flags_for_config(MinSizeRel /O1)
else()
    string(REPLACE " " ";" COMMON_WARNINGS_LIST "-Wall -Wextra -pedantic -Werror")
    add_compile_flags_for_config(Debug ${COMMON_WARNINGS_LIST} -O0 -g)
    add_compile_flags_for_config(Release -O3)
    add_compile_flags_for_config(RelWithDebInfo -O2 -g)
    add_compile_flags_for_config(MinSizeRel -Os)
endif()

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

