# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.16)

find_package( Qt5 COMPONENTS Widgets QUIET)

if( TARGET Qt5::Widgets)

  set( CMAKE_AUTOMOC ON)
  set( CMAKE_AUTOUIC ON)

  add_library( arinc665_qt)

  target_sources(
    arinc665_qt

    PRIVATE
      Arinc665Qt.hpp
      Arinc665QtLogger.hpp
      BatchWidget.hpp
      BatchWidget.cpp
     BatchWidget.ui
      DirectoryWidget.hpp
      DirectoryWidget.cpp
      DirectoryWidget.ui
      FileWidget.hpp
      FileWidget.cpp
      FileWidget.ui
      #LoadsModel.hpp
      LoadWidget.hpp
      LoadWidget.cpp
      LoadWidget.ui
      MediaSetController.hpp
      MediaSetController.cpp
      MediaSetDialog.hpp
      MediaSetDialog.cpp
      MediaSetDialog.ui
      MediaSetModel.hpp
      MediaSetModel.cpp
      MediaSetWidget.hpp
      MediaSetWidget.cpp
      MediaSetWidget.ui
      MediumWidget.hpp
      MediumWidget.cpp
      MediumWidget.ui
      RegularFileWidget.hpp
      RegularFileWidget.cpp
      RegularFileWidget.ui)

  target_include_directories( arinc665_qt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

  target_link_libraries(
    arinc665_qt

    PUBLIC
      arinc665
      Qt5::Widgets)

endif( TARGET Qt5::Widgets)
