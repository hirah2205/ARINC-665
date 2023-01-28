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
        BatchesModel.hpp
        BatchInfoModel.hpp
        BatchWidget.hpp
        DirectoryWidget.hpp
        FileWidget.hpp
        LoadFilesModel.hpp
        LoadsModel.hpp
        LoadWidget.hpp
        Media.hpp
        MediaSetModel.hpp
        MediaSetsModel.hpp
        MediaSetWidget.hpp
        MediumWidget.hpp
        RegularFileWidget.hpp
        TargetHardwareIdsPositionsModel.hpp

  PRIVATE
    BatchesModel.cpp
    BatchInfoModel.cpp
    BatchWidget.cpp
    BatchWidget.ui
    DirectoryWidget.cpp
    DirectoryWidget.ui
    FileWidget.cpp
    FileWidget.ui
    LoadFilesModel.cpp
    LoadsModel.cpp
    LoadWidget.cpp
    LoadWidget.ui
    MediaSetModel.cpp
    MediaSetsModel.cpp
    MediaSetWidget.cpp
    MediaSetWidget.ui
    MediumWidget.cpp
    MediumWidget.ui
    RegularFileWidget.cpp
    RegularFileWidget.ui
    TargetHardwareIdsPositionsModel.cpp )
