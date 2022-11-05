# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.24 )

project(
  arinc665
  VERSION 0.0.0.0
  DESCRIPTION "ARINC 665 Tool Suite"
  HOMEPAGE_URL "https://git.thomas-vogt.de/thomas-vogt/arinc665" )

set( PROJECT_VERSION_ADDITION "dev" )

find_package( Git )

if( GIT_FOUND )
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --verify --short HEAD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE )

  set( PROJECT_VERSION_ADDITION "${PROJECT_VERSION_ADDITION}-${GIT_HASH}" )
endif()


set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# find clang-tidy and add options to execute
find_program( CLANG_TIDY_EXE NAMES clang-tidy )

if( CLANG_TIDY_EXE )
  set( CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} )
endif()

enable_testing()

include( GNUInstallDirs )

include( FetchContent )

FetchContent_Declare(
  cmake_common
  GIT_REPOSITORY https://git.thomas-vogt.de/thomas-vogt/cmake_common.git
  GIT_TAG        main )

if( IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake_common )
  set( FETCHCONTENT_SOURCE_DIR_CMAKE_COMMON ${CMAKE_SOURCE_DIR}/cmake_common )
endif()

FetchContent_GetProperties( cmake_common )
if( NOT cmake_common_POPULATED )
  FetchContent_Populate( cmake_common )
  # Add own CMake scripts to module path
  list( APPEND CMAKE_MODULE_PATH "${cmake_common_SOURCE_DIR}" )
endif()


FetchContent_Declare(
  helper
  GIT_REPOSITORY https://git.thomas-vogt.de/thomas-vogt/helper.git
  GIT_TAG        main )

if( IS_DIRECTORY ${CMAKE_SOURCE_DIR}/helper )
  set( FETCHCONTENT_SOURCE_DIR_HELPER ${CMAKE_SOURCE_DIR}/helper )
endif()


FetchContent_Declare(
  arinc645
  GIT_REPOSITORY https://git.thomas-vogt.de/thomas-vogt/arinc645.git
  GIT_TAG        main )

if( IS_DIRECTORY ${CMAKE_SOURCE_DIR}/arinc645 )
  set( FETCHCONTENT_SOURCE_DIR_ARINC645 ${CMAKE_SOURCE_DIR}/arinc645 )
endif()


FetchContent_Declare(
  command_shell
  GIT_REPOSITORY https://git.thomas-vogt.de/thomas-vogt/command_shell.git
  GIT_TAG        main )

if( IS_DIRECTORY ${CMAKE_SOURCE_DIR}/command_shell )
  set( FETCHCONTENT_SOURCE_DIR_COMMAND_SHELL ${CMAKE_SOURCE_DIR}/command_shell )
endif()

FetchContent_MakeAvailable( helper arinc645 command_shell )


add_subdirectory( arinc665 )
add_subdirectory( arinc665_commands )
add_subdirectory( arinc665_qt )
add_subdirectory( arinc665_xml )

add_subdirectory( application )

add_subdirectory( arinc665_doc )

add_subdirectory( test )

install(
  FILES LICENSE
  RENAME ${PROJECT_NAME}.LICENSE
  TYPE DOC
  COMPONENT runtime )

set( CPACK_GENERATOR ZIP )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION} )
set( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
include( CPack )
