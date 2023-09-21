# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.26 )

target_sources(
  arinc665

  PUBLIC
    FILE_SET HEADERS
      FILES
        Base.hpp
        Batch.hpp
        ContainerEntity.hpp
        Directory.hpp
        File.hpp
        Load.hpp
        Media.hpp
        MediaSet.hpp
        RegularFile.hpp

  PRIVATE
    Batch.cpp
    ContainerEntity.cpp
    Directory.cpp
    File.cpp
    Load.cpp
    MediaSet.cpp
    RegularFile.cpp )

target_sources(
  arinc665_test

  PRIVATE
    test/LoadTest.cpp
    test/MediaSetTest.cpp )
