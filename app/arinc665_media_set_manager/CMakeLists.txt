# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.28 )

add_executable( arinc665_media_set_manager )

target_sources(
  arinc665_media_set_manager

  PRIVATE
    arinc665_media_set_manager.cpp )

target_compile_features( arinc665_media_set_manager PRIVATE cxx_std_23 )

target_compile_options(
  arinc665_media_set_manager

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries(
  arinc665_media_set_manager

  PRIVATE
    arinc665_commands
    commands
    helper )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY MAN_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/arinc665_media_set_manager.adoc
    ${CMAKE_CURRENT_SOURCE_DIR}/arinc665_media_set_manager-create.adoc
    ${CMAKE_CURRENT_SOURCE_DIR}/arinc665_media_set_manager-list-loads.adoc
    ${CMAKE_CURRENT_SOURCE_DIR}/arinc665_media_set_manager-list-media-sets.adoc )

install(
  TARGETS arinc665_media_set_manager
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  COMPONENT runtime )
