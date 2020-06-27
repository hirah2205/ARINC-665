# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.16 )

find_package( Qt5Widgets QUIET )

if( Qt5Widgets_FOUND )

  set( CMAKE_AUTOMOC ON )

  add_executable( arinc665_media_set_view_qt WIN32 )

  target_sources( arinc665_media_set_view_qt PRIVATE arinc665_media_set_view_gui.cpp )

  target_link_libraries(
    arinc665_media_set_view_qt

    PRIVATE
      arinc665_qt
      Qt5::Widgets )

  install(
    TARGETS arinc665_media_set_view_qt
    COMPONENT runtime )

endif( Qt5Widgets_FOUND )
