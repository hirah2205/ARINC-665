# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.11)

find_package( Boost REQUIRED)

add_library(
  arinc665

  Arinc665.hpp
  Arinc665.cpp
  Arinc665Crc.cpp
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
  file/BatchFile.hpp
  file/BatchFile.cpp
  file/BatchInfo.hpp
  file/BatchInfo.cpp
  file/BatchListFile.hpp
  file/BatchListFile.cpp
  file/BatchLoadInfo.hpp
  file/BatchLoadInfo.cpp
  file/BatchTargetInfo.hpp
  file/BatchTargetInfo.cpp
  file/CheckValueUtils.cpp
  file/CheckValueUtils.hpp
  file/File.hpp
  file/FileInfo.hpp
  file/FileInfo.cpp
  file/FileListFile.hpp
  file/FileListFile.cpp
  file/ListFile.hpp
  file/ListFile.cpp
  file/LoadFileInfo.hpp
  file/LoadFileInfo.cpp
  file/LoadHeaderFile.hpp
  file/LoadHeaderFile.cpp
  file/LoadInfo.hpp
  file/LoadInfo.cpp
  file/LoadListFile.hpp
  file/LoadListFile.cpp

  media/Base.hpp
  media/BaseFile.hpp
  media/BaseFile.cpp
  media/Batch.hpp
  media/Batch.cpp
  media/ContainerEntity.hpp
  media/ContainerEntity.cpp
  media/Directory.hpp
  media/Directory.cpp
  media/File.hpp
  media/File.cpp
  media/Load.hpp
  media/Load.cpp
  media/Media.hpp
  media/MediaSet.hpp
  media/MediaSet.cpp
  media/Medium.hpp
  media/Medium.cpp

  utils/Utils.hpp
  utils/Arinc665Utils.hpp
  utils/Arinc665Utils.cpp
  utils/Arinc665Xml.hpp
  utils/Arinc665Xml.cpp
  utils/FileCreationPolicyDescription.hpp
  utils/FileCreationPolicyDescription.cpp
  utils/MediaSetConfiguration.hpp
  utils/MediaSetConfiguration.cpp
  utils/MediaSetManager.hpp
  utils/MediaSetManager.cpp

  utils/implementation/Arinc665XmlPugiXmlImpl.hpp
  utils/implementation/Arinc665XmlPugiXmlImpl.cpp
  utils/implementation/MediaSetExporterImpl.hpp
  utils/implementation/MediaSetExporterImpl.cpp
  utils/implementation/MediaSetImporterImpl.hpp
  utils/implementation/MediaSetImporterImpl.cpp
  utils/implementation/MediaSetManagerImpl.hpp
  utils/implementation/MediaSetManagerImpl.cpp)

target_compile_options(
  arinc665

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra>)

target_compile_definitions(
  arinc665

  PRIVATE
    # Activate STL assertions
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:Debug>>:_GLIBCXX_ASSERTIONS>
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:Debug>>:_GLIBCXX_ASSERTIONS>)

target_include_directories(
  arinc665
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

target_link_libraries(
  arinc665

  PUBLIC
    helper
    pugixml
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,7.0>>:stdc++fs>
    Boost::boost)

target_compile_features( arinc665 PUBLIC cxx_std_17)


add_library(
  arinc665_test
  OBJECT

  Arinc665CrcTest.cpp
  CheckCodeTest.cpp
  ManufacturerCodeTest.cpp
  PartNumberTest.cpp
  ProductIdentifierTest.cpp
  TargetHardwareIdTest.cpp

  file/BatchFileTest.cpp
  file/BatchListFileTest.cpp
  file/CheckValueUtilsTest.cpp
  file/FileListFileTest.cpp
  file/LoadFileInfoTest.cpp
  file/LoadHeaderFileTest.cpp
  file/LoadListFileTest.cpp

  media/MediaSetTest.cpp)

target_compile_options( 
  arinc665_test

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra>)

target_compile_definitions(
  arinc665_test

  PRIVATE
    # Activate STL assertions
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:Debug>>:_GLIBCXX_ASSERTIONS>
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:Debug>>:_GLIBCXX_ASSERTIONS>)

target_compile_features( arinc665_test PUBLIC cxx_std_17)

target_include_directories(
  arinc665_test
  PRIVATE $<TARGET_PROPERTY:arinc665,INCLUDE_DIRECTORIES>)


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
  EXPORT arinc665-config
  COMPONENT development
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
