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
        CompileMediaSet.hpp
        CompileMediaSetSettingsPage.hpp
        CompileMediaSetWizard.hpp

  PRIVATE
    CompileMediaSetSettingsPage.cpp
    CompileMediaSetSettingsPage.ui
    CompileMediaSetWizard.cpp
    CompileMediaSetWizard.ui )
