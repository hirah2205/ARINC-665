# $Date$
# $Revision$
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.5)

project ( "ARINC 665 Library")

find_package( 
  Boost
  REQUIRED
  filesystem
  unit_test_framework)
	
add_library( 
  arinc665
 
  arinc665/Arinc665.cpp

  arinc665/file/Arinc665File.cpp
  arinc665/file/FileFactory.cpp
  arinc665/file/StringHelper.cpp
 
  arinc665/file/list/LoadListFile.cpp
  arinc665/file/list/FileInfo.cpp
  arinc665/file/list/FileListFile.cpp
  arinc665/file/list/LoadInfo.cpp
  arinc665/file/list/ListFile.cpp
 
  arinc665/file/load/LoadHeaderFile.cpp
  arinc665/file/load/LoadFileInfo.cpp
 
  arinc665/partnumber/PartNumber.cpp
  arinc665/partnumber/TargetHardwareId.cpp
  arinc665/partnumber/ManufacturerCode.cpp
  arinc665/partnumber/CheckCode.cpp
  arinc665/partnumber/ProductIdentifier.cpp
 
  arinc665/partnumber/test/PartNumberTest.cpp
  arinc665/partnumber/test/CheckCodeTest.cpp
 
  arinc665/media/PartNumberdEntity.hpp
  arinc665/media/PartNumberdEntity.cpp
  arinc665/media/MediaSet.hpp
  arinc665/media/MediaSet.cpp
  arinc665/media/ContainerEntity.hpp
  arinc665/media/ContainerEntity.cpp
  arinc665/media/Medium.hpp
  arinc665/media/Medium.cpp
  arinc665/media/Directory.hpp
  arinc665/media/Directory.cpp
  arinc665/media/BaseFile.hpp
  arinc665/media/BaseFile.cpp
  arinc665/media/File.hpp
  arinc665/media/File.cpp
  arinc665/media/Load.hpp
  arinc665/media/Load.cpp
  arinc665/media/Batch.hpp
  arinc665/media/Batch.cpp
  arinc665/media/BatchInfo.hpp
  arinc665/media/BatchInfo.cpp

  arinc665/importer/Arinc665Importer.cpp   

  arinc665/media/test/MediaSetTest.cpp)

target_include_directories( arinc665 SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories( arinc665 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( arinc665 helper)

GENERATE_DOCUMENTATION()