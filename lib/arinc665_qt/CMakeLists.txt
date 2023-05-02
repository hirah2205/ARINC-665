# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.25 )

find_package( Qt5 COMPONENTS Widgets QUIET )

if( NOT TARGET Qt5::Widgets )
  return()
endif()

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

add_library( arinc665_qt )

include( GenerateExportHeader )
generate_export_header( arinc665_qt )

target_sources(
  arinc665_qt

  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}/..
      FILES
        Arinc665Qt.hpp
        Arinc665QtLogger.hpp
        ExportMediaSetSettingsWidget.hpp
        FileCreationPolicyModel.hpp
        FilePathMappingModel.hpp
        ImportMediaSetXmlSettingsWidget.hpp
        MediaPathsModel.hpp
        MediaPathsWidget.hpp
        SupportedArinc665VersionModel.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/arinc665_qt_export.h

  PRIVATE
    ExportMediaSetSettingsWidget.cpp
    ExportMediaSetSettingsWidget.ui
    FileCreationPolicyModel.cpp
    FilePathMappingModel.cpp
    ImportMediaSetXmlSettingsWidget.cpp
    ImportMediaSetXmlSettingsWidget.ui
    MediaPathsModel.cpp
    MediaPathsWidget.cpp
    MediaPathsWidget.ui
    SupportedArinc665VersionModel.cpp )

target_compile_features( arinc665_qt PUBLIC cxx_std_20 )

target_compile_options(
  arinc665_qt

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries(
  arinc665_qt

  PUBLIC
    arinc665
    Qt5::Widgets

  PRIVATE
    helper_qt
    qt_icon_resources
    arinc645_qt )

add_subdirectory( compile_media_set )
add_subdirectory( decompile_media_set )
add_subdirectory( media )
add_subdirectory( media_set_manager )
add_subdirectory( media_set_viewer )
add_subdirectory( resources )

# disable clang tidy in binary dir by placing dummy .clang-tidy file
file(
  WRITE
  ${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy
  "Checks: '-*,llvm-twine-local'" )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )


install(
  TARGETS arinc665_qt
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  EXPORT arinc665_qt-config
  COMPONENT runtime
  ARCHIVE COMPONENT development
  FILE_SET HEADERS COMPONENT development )

install(
  EXPORT arinc665_qt-config
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
  COMPONENT development )
