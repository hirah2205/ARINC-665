# $Date$
# $Revision$
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.7)

project( "ARINC 665 QT Library")

find_package( Qt5Widgets)

if( DEFINED Qt5Widgets_FOUND)

#QT5_WRAP_UI(
#  bhm_qt_ui
#  
#  bhm_qt/AsfMessagesDialog.ui
#  )

set( CMAKE_AUTOMOC ON)

add_library(
  arinc665_qt
  
  arinc665_qt/Arinc665Qt.hpp
  arinc665_qt/QMediaSetModel.hpp
  arinc665_qt/QMediaSetModel.cpp
#  ${bhm_qt_ui}
  )

target_include_directories( arinc665_qt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories( arinc665_qt PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries( 
  arinc665_qt 
  arinc665
  Qt5::Widgets)

set( 
  DOXYGEN_TAGFILES 
  "libhelper.TAGFILE=../../libhelper/html libarinc665.TAGFILE=../../libarinc665/html")

GENERATE_DOCUMENTATION()

endif()