# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.18 )

target_sources(
  arinc665_qt

  PRIVATE
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
    Media.hpp
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
    RegularFileWidget.ui )
