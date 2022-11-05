# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.24 )

target_sources(
  arinc665

  PRIVATE
    Arinc665Xml.hpp
    Arinc665Xml.cpp
    FileCreationPolicyDescription.hpp
    FileCreationPolicyDescription.cpp
    FilePrinter.hpp
    FilePrinter.cpp
    JsonMediaSetManager.hpp
    JsonMediaSetManager.cpp
    MediaSetExporter.hpp
    MediaSetExporter.cpp
    MediaSetImporter.hpp
    MediaSetImporter.cpp
    MediaSetManager.hpp
    MediaSetManager.cpp
    MediaSetManagerConfiguration.hpp
    MediaSetManagerConfiguration.cpp
    MediaSetPrinter.hpp
    MediaSetPrinter.cpp
    MediaSetValidator.hpp
    MediaSetValidator.cpp
    Utils.hpp
    Utils.cpp )

#target_sources(
#  arinc665_test
#
#  PRIVATE)

add_subdirectory( implementation )
