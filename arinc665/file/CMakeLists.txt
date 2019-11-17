# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.15)

target_sources(
  arinc665

  PRIVATE
    Arinc665File.hpp
    Arinc665File.cpp
    BatchFile.hpp
    BatchFile.cpp
    BatchInfo.hpp
    BatchInfo.cpp
    BatchListFile.hpp
    BatchListFile.cpp
    BatchLoadInfo.hpp
    BatchLoadInfo.cpp
    BatchTargetInfo.hpp
    BatchTargetInfo.cpp
    CheckValueUtils.cpp
    CheckValueUtils.hpp
    File.hpp
    FileInfo.hpp
    FileInfo.cpp
    FileListFile.hpp
    FileListFile.cpp
    ListFile.hpp
    ListFile.cpp
    LoadFileInfo.hpp
    LoadFileInfo.cpp
    LoadHeaderFile.hpp
    LoadHeaderFile.cpp
    LoadInfo.hpp
    LoadInfo.cpp
    LoadListFile.hpp
    LoadListFile.cpp)

target_sources(
  arinc665_test

  PRIVATE
    BatchFileTest.cpp
    BatchListFileTest.cpp
    BatchLoadInfoTest.cpp
    CheckValueUtilsTest.cpp
    FileListFileTest.cpp
    LoadFileInfoTest.cpp
    LoadHeaderFileTest.cpp
    LoadListFileTest.cpp)
