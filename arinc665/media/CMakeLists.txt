# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

target_sources(
  arinc665

  PRIVATE
    Base.hpp
    File.hpp
    File.cpp
    Batch.hpp
    Batch.cpp
    ContainerEntity.hpp
    ContainerEntity.cpp
    Directory.hpp
    Directory.cpp
    RegularFile.hpp
    RegularFile.cpp
    Load.hpp
    Load.cpp
    Media.hpp
    MediaSet.hpp
    MediaSet.cpp
    Medium.hpp
    Medium.cpp )

target_sources(
  arinc665_test

  PRIVATE
    MediaSetTest.cpp )
