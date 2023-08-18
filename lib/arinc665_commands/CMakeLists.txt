# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.26 )

find_package( fmt REQUIRED )

add_library( arinc665_commands )

include( GenerateExportHeader )
generate_export_header( arinc665_commands )

target_sources(
  arinc665_commands

  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}/..
      FILES
        Arinc665Commands.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/arinc665_commands_export.h

  PRIVATE
    Arinc665Commands.cpp )

target_compile_features( arinc665_commands PUBLIC cxx_std_23 )

target_compile_options(
  arinc665_commands

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries(
  arinc665_commands

  PUBLIC
    arinc665
    commands

  PRIVATE
    fmt::fmt )

add_subdirectory( media_set_manager )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )


install(
  TARGETS arinc665_commands
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  EXPORT arinc665_commands-config
  COMPONENT runtime
  ARCHIVE COMPONENT development
  FILE_SET HEADERS COMPONENT development )

install(
  EXPORT arinc665_commands-config
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
  COMPONENT development )
