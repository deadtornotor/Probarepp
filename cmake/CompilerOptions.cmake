# CompilerOptions.cmake
# Centralized compiler flags for all configurations and platforms

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Message output
if(DEFINED MAIN_PROJECT AND MAIN_PROJECT)
  message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
  message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
  message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
endif()

# Per-target flags function
function(probare_setup_target_config_types TARGET_NAME)
  if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE
      $<$<CONFIG:Debug>:/W4 /Od /Zi>
      $<$<CONFIG:Release>:/O2>
      $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
      $<$<CONFIG:MinSizeRel>:/O1>
    )
  else()
    target_compile_options(${TARGET_NAME} PRIVATE
      $<$<CONFIG:Debug>:-Wall -Wextra -pedantic -Werror -O0 -g>
      $<$<CONFIG:Release>:-O3>
      $<$<CONFIG:RelWithDebInfo>:-O2 -g>
      $<$<CONFIG:MinSizeRel>:-Os>
    )

    target_compile_definitions(${TARGET_NAME} PRIVATE
      $<$<CONFIG:Release>:NDEBUG>
      $<$<CONFIG:RelWithDebInfo>:NDEBUG>
      $<$<CONFIG:MinSizeRel>:NDEBUG>
    )
  endif()
endfunction()
