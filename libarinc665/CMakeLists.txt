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
 
  arinc665/Arinc665.hpp
  arinc665/Arinc665.cpp
  arinc665/Arinc665Crc.hpp
  arinc665/PartNumber.hpp
  arinc665/PartNumber.cpp
  
  arinc665/TargetHardwareId.hpp
  arinc665/TargetHardwareId.cpp
  arinc665/ManufacturerCode.hpp
  arinc665/ManufacturerCode.cpp
  arinc665/CheckCode.hpp
  arinc665/CheckCode.cpp
  
  arinc665/ProductIdentifier.hpp
  arinc665/ProductIdentifier.cpp
 
  arinc665/file/Arinc665File.cpp
  arinc665/file/FileFactory.cpp
  arinc665/file/StringHelper.cpp
 
  arinc665/file/list/ListFile.hpp
  arinc665/file/list/ListFile.cpp
  arinc665/file/list/FileInfo.hpp
  arinc665/file/list/FileInfo.cpp
  arinc665/file/list/FileListFile.hpp
  arinc665/file/list/FileListFile.cpp
  arinc665/file/list/LoadInfo.hpp
  arinc665/file/list/LoadInfo.cpp
  arinc665/file/list/LoadListFile.hpp
  arinc665/file/list/LoadListFile.cpp
  arinc665/file/list/BatchInfo.hpp
  arinc665/file/list/BatchInfo.cpp
  arinc665/file/list/BatchListFile.hpp
  arinc665/file/list/BatchListFile.cpp
 
  arinc665/file/load/LoadHeaderFile.cpp
  arinc665/file/load/LoadFileInfo.cpp
 
  arinc665/file/batch/BatchFile.hpp
  arinc665/file/batch/BatchFile.cpp
  arinc665/file/batch/BatchLoadInfo.hpp
  arinc665/file/batch/TargetHardwareLoad.hpp

  arinc665/media/Base.hpp
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

  arinc665/utils/Utils.hpp   
  arinc665/utils/Arinc665Utils.hpp   
  arinc665/utils/Arinc665Utils.cpp   
  arinc665/utils/Arinc665Xml.cpp
  arinc665/utils/Arinc665Xml.hpp
  arinc665/utils/implementation/Arinc665ImporterImpl.hpp
  arinc665/utils/implementation/Arinc665ImporterImpl.cpp
  arinc665/utils/implementation/Arinc665XmlPugiXmlImpl.hpp
  arinc665/utils/implementation/Arinc665XmlPugiXmlImpl.cpp
  )

target_include_directories( arinc665 SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories( arinc665 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( arinc665 helper pugixml ${Boost_LIBRARIES})


add_library( 
  arinc665_test 
  OBJECT
  
  arinc665/Arinc665CrcTest.cpp
  arinc665/CheckCodeTest.cpp
  arinc665/ManufacturerCodeTest.cpp
  arinc665/PartNumberTest.cpp
  arinc665/ProductIdentifierTest.cpp
  
  arinc665/media/MediaSetTest.cpp
  )

target_include_directories( arinc665_test PRIVATE $<TARGET_PROPERTY:arinc665,INCLUDE_DIRECTORIES>)

GENERATE_DOCUMENTATION()