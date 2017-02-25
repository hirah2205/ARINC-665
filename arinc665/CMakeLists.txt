# $Date$
# $Revision$
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.7)

project ( "ARINC 665 Library")

find_package( 
  Boost
  REQUIRED
  filesystem)

add_library( 
  arinc665
 
  Arinc665.hpp
  Arinc665.cpp
  Arinc665Crc.hpp
  PartNumber.hpp
  PartNumber.cpp
  
  TargetHardwareId.hpp
  TargetHardwareId.cpp
  ManufacturerCode.hpp
  ManufacturerCode.cpp
  CheckCode.hpp
  CheckCode.cpp
  
  ProductIdentifier.hpp
  ProductIdentifier.cpp
 
  file/Arinc665File.cpp
  file/FileFactory.cpp
  file/StringHelper.cpp
 
  file/list/ListFile.hpp
  file/list/ListFile.cpp
  file/list/FileInfo.hpp
  file/list/FileInfo.cpp
  file/list/FileListFile.hpp
  file/list/FileListFile.cpp
  file/list/LoadInfo.hpp
  file/list/LoadInfo.cpp
  file/list/LoadListFile.hpp
  file/list/LoadListFile.cpp
  file/list/BatchInfo.hpp
  file/list/BatchInfo.cpp
  file/list/BatchListFile.hpp
  file/list/BatchListFile.cpp
 
  file/load/LoadHeaderFile.cpp
  file/load/LoadFileInfo.cpp
 
  file/batch/BatchFile.hpp
  file/batch/BatchFile.cpp
  file/batch/BatchLoadInfo.hpp
  file/batch/TargetHardwareLoad.hpp

  media/Base.hpp
  media/MediaSet.hpp
  media/MediaSet.cpp
  media/ContainerEntity.hpp
  media/ContainerEntity.cpp
  media/Medium.hpp
  media/Medium.cpp
  media/Directory.hpp
  media/Directory.cpp
  media/BaseFile.hpp
  media/BaseFile.cpp
  media/File.hpp
  media/File.cpp
  media/Load.hpp
  media/Load.cpp
  media/Batch.hpp
  media/Batch.cpp
  media/BatchInfo.hpp
  media/BatchInfo.cpp

  utils/Utils.hpp   
  utils/Arinc665Utils.hpp   
  utils/Arinc665Utils.cpp   
  utils/Arinc665Xml.cpp
  utils/Arinc665Xml.hpp
  utils/implementation/Arinc665ImporterImpl.hpp
  utils/implementation/Arinc665ImporterImpl.cpp
  utils/implementation/Arinc665XmlPugiXmlImpl.hpp
  utils/implementation/Arinc665XmlPugiXmlImpl.cpp
  )

target_include_directories( arinc665 SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories( arinc665 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries( arinc665 helper pugixml Boost::filesystem)


add_library( 
  arinc665_test 
  OBJECT
  
  Arinc665CrcTest.cpp
  CheckCodeTest.cpp
  ManufacturerCodeTest.cpp
  PartNumberTest.cpp
  ProductIdentifierTest.cpp
  TargetHardwareIdTest.cpp
  
  media/MediaSetTest.cpp
  )

target_include_directories( arinc665_test PRIVATE $<TARGET_PROPERTY:arinc665,INCLUDE_DIRECTORIES>)

GENERATE_DOCUMENTATION()