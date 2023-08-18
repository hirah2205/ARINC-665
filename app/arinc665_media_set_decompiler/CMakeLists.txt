# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.26 )

find_package( fmt REQUIRED )

add_executable( arinc665_media_set_decompiler )

target_sources(
  arinc665_media_set_decompiler
  PRIVATE arinc665_media_set_decompiler.cpp )

target_compile_features( arinc665_media_set_decompiler PRIVATE cxx_std_23 )

target_compile_options(
  arinc665_media_set_decompiler

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries( arinc665_media_set_decompiler PRIVATE arinc665 fmt::fmt )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

install(
  TARGETS arinc665_media_set_decompiler
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  COMPONENT runtime )
