# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

target_sources(
  arinc665

  PRIVATE
    Arinc665XmlImpl.hpp
    Arinc665XmlImpl.cpp
    MediaSetExporterImpl.hpp
    MediaSetExporterImpl.cpp
    MediaSetImporterImpl.hpp
    MediaSetImporterImpl.cpp
    MediaSetManagerImpl.hpp
    MediaSetManagerImpl.cpp
    MediaSetValidatorImpl.cpp
    MediaSetValidatorImpl.hpp )

#target_sources(
#  arinc665_test
#
#  PRIVATE)
