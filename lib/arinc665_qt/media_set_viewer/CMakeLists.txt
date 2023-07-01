# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.26 )

target_sources(
  arinc665_qt

  PUBLIC
    FILE_SET HEADERS
      FILES
        MediaSetViewer.hpp
        MediaSetViewerWindow.hpp

  PRIVATE
    MediaSetViewerWindow.cpp
    MediaSetViewerWindow.ui )
