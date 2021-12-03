# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

# skip app when arinc665_qt is not available
if( NOT TARGET arinc665_qt )
  return()
endif()

add_executable( arinc665_media_set_view_gui WIN32 )

target_sources( arinc665_media_set_view_gui PRIVATE arinc665_media_set_view_gui.cpp )

target_link_libraries(
  arinc665_media_set_view_gui

  PRIVATE
    arinc665_qt )

install( TARGETS arinc665_media_set_view_gui COMPONENT runtime )
