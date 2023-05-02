# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.25 )

# skip app when arinc665_qt is not available
if( NOT TARGET arinc665_qt )
  return()
endif()

add_executable( arinc665_media_set_viewer_gui WIN32 )

target_sources(
  arinc665_media_set_viewer_gui
  PRIVATE arinc665_media_set_viewer_gui.cpp )

target_compile_features( arinc665_media_set_viewer_gui PRIVATE cxx_std_20 )

target_compile_options(
  arinc665_media_set_viewer_gui

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W4>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries(
  arinc665_media_set_viewer_gui

  PRIVATE
    qt_icon_resources
    arinc665_qt )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

install(
  TARGETS arinc665_media_set_viewer_gui
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  COMPONENT runtime )
