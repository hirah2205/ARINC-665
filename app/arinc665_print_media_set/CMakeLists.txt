# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.26 )

add_executable( arinc665_print_media_set )

target_sources( arinc665_print_media_set PRIVATE arinc665_print_media_set.cpp )

target_compile_features( arinc665_print_media_set PRIVATE cxx_std_23 )

target_compile_options(
  arinc665_print_media_set

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries( arinc665_print_media_set arinc665 )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY MAN_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/arinc665_print_media_set.adoc )

install(
  TARGETS arinc665_print_media_set
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  COMPONENT runtime )
