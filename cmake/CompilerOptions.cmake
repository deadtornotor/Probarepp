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

get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(is_multi_config)
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

    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
else()
    message(STATUS "Using global CMAKE_CXX_FLAGS_<CONFIG> for single-config generator")

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
        message(STATUS "No CMAKE_BUILD_TYPE defined, defaulting to ${CMAKE_BUILD_TYPE}")
    endif()

    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /Od /Zi" CACHE STRING "Flags for Debug" FORCE)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2" CACHE STRING "Flags for Release" FORCE)
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /O2 /Zi" CACHE STRING "Flags for RelWithDebInfo" FORCE)
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /O1" CACHE STRING "Flags for MinSizeRel" FORCE)
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -Werror -O0 -g" CACHE STRING "Flags for Debug" FORCE)
        set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3" CACHE STRING "Flags for Release" FORCE)
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-DNDEBUG -O2 -g" CACHE STRING "Flags for RelWithDebInfo" FORCE)
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG -Os" CACHE STRING "Flags for MinSizeRel" FORCE)
    endif()

    message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
