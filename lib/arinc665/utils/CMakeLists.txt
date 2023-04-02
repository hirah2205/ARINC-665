# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.25 )

target_sources(
  arinc665

  PUBLIC
    FILE_SET HEADERS
      FILES
        Arinc665Xml.hpp
        FileCreationPolicyDescription.hpp
        FilePrinter.hpp
        FilesystemMediaSetCompiler.hpp
        FilesystemMediaSetCopier.hpp
        FilesystemMediaSetImporter.hpp
        FilesystemMediaSetRemover.hpp
        MediaSetCompiler.hpp
        MediaSetImporter.hpp
        MediaSetManager.hpp
        MediaSetManagerConfiguration.hpp
        MediaSetPrinter.hpp
        MediaSetValidator.hpp
        Utils.hpp

  PRIVATE
    Arinc665Xml.cpp
    FileCreationPolicyDescription.cpp
    FilePrinter.cpp
    FilesystemMediaSetCompiler.cpp
    FilesystemMediaSetCopier.cpp
    FilesystemMediaSetImporter.cpp
    FilesystemMediaSetRemover.cpp
    MediaSetCompiler.cpp
    MediaSetImporter.cpp
    MediaSetManager.cpp
    MediaSetManagerConfiguration.cpp
    MediaSetPrinter.cpp
    MediaSetValidator.cpp
    Utils.cpp )

#target_sources(
#  arinc665_test
#
#  PRIVATE)

add_subdirectory( implementation )
