# $Date$
# $Revision$
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.8)

project ( "ARINC 665 Library" VERSION 1.0)

find_package( 
  Boost
  REQUIRED
  filesystem)

add_library( 
  arinc665
 
  Arinc665.hpp
  Arinc665.cpp
  Arinc665Crc.hpp
  Arinc665Exception.hpp
  Arinc665Logger.hpp
  CheckCode.hpp
  CheckCode.cpp
  FileTypeDescription.hpp
  FileTypeDescription.cpp
  ManufacturerCode.hpp
  ManufacturerCode.cpp
  PartNumber.hpp
  PartNumber.cpp
  ProductIdentifier.hpp
  ProductIdentifier.cpp
  TargetHardwareId.hpp
  TargetHardwareId.cpp

  file/Arinc665File.hpp
  file/Arinc665File.cpp
  file/FileFactory.hpp
  file/FileFactory.cpp
  file/StringHelper.hpp
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

  file/load/LoadHeaderFile.hpp
  file/load/LoadHeaderFile.cpp
  file/load/LoadFileInfo.hpp
  file/load/LoadFileInfo.cpp

  file/batch/BatchFile.hpp
  file/batch/BatchFile.cpp
  file/batch/BatchLoadInfo.hpp
  file/batch/BatchTargetInfo.hpp

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

  utils/Utils.hpp
  utils/Arinc665Utils.hpp
  utils/Arinc665Utils.cpp
  utils/Arinc665Xml.hpp
  utils/Arinc665Xml.cpp
  
  utils/implementation/Arinc665ImporterImpl.hpp
  utils/implementation/Arinc665ImporterImpl.cpp
  utils/implementation/Arinc665XmlPugiXmlImpl.hpp
  utils/implementation/Arinc665XmlPugiXmlImpl.cpp
  )

target_include_directories( arinc665 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

target_link_libraries( arinc665 helper pugixml Boost::filesystem)

target_compile_features( arinc665 PUBLIC cxx_std_14)


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

add_documentation( TARGET arinc665)

# INSTALL
include( GNUInstallDirs)

install(
  TARGETS arinc665
  COMPONENT runtime
  EXPORT arinc665-config 
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMPONENT development
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*Test.*" EXCLUDE
    PATTERN "implementation" EXCLUDE)

install(
  DIRECTORY $<TARGET_PROPERTY:arinc665_doc,DOCUMENTATION_OUTPUT_DIR>
  COMPONENT documentation
  DESTINATION ${CMAKE_INSTALL_DOCDIR})

install(
  FILES $<TARGET_PROPERTY:arinc665_doc,DOCUMENTATION_HTML_FILE>
  COMPONENT documentation
  DESTINATION ${CMAKE_INSTALL_DOCDIR})

install(
  FILES $<TARGET_PROPERTY:arinc665_doc,DOCUMENTATION_TAGFILE>
  COMPONENT documentation
  DESTINATION ${CMAKE_INSTALL_DOCDIR})

install( 
  EXPORT arinc665-config 
  COMPONENT development
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
