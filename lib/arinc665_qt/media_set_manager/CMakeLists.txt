# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.25 )

target_sources(
  arinc665_qt

  PUBLIC
    FILE_SET HEADERS
      FILES
        ImportMediaSetWizard.hpp
        ImportMediaSetSettingsPage.hpp
        MediaSetManager.hpp
        MediaSetManagerController.hpp
        MediaSetManagerDialog.hpp
        RemoveMediaSetController.hpp
        ViewMediaSetDialog.hpp

  PRIVATE
    ImportMediaSetSettingsPage.cpp
    ImportMediaSetSettingsPage.ui
    ImportMediaSetWizard.cpp
    ImportMediaSetWizard.ui
    MediaSetManagerController.cpp
    MediaSetManagerDialog.cpp
    MediaSetManagerDialog.ui
    RemoveMediaSetController.cpp
    ViewMediaSetDialog.cpp
    ViewMediaSetDialog.ui )
