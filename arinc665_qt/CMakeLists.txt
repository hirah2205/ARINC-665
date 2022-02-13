# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

find_package( Qt5 COMPONENTS Widgets QUIET )

if( NOT TARGET Qt5::Widgets )
  return()
endif()

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

add_library( arinc665_qt )

target_sources(
  arinc665_qt

  PRIVATE
    Arinc665Qt.hpp
    Arinc665QtLogger.hpp
    MediaSetController.hpp
    MediaSetController.cpp
    MediaSetDialog.hpp
    MediaSetDialog.cpp
    MediaSetDialog.ui )

target_include_directories(
  arinc665_qt
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..> )

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
    helper_qt )

include( GenerateExportHeader )
generate_export_header( arinc665_qt )

add_subdirectory( media )
add_subdirectory( media_set_manager )
add_subdirectory( resources )

# disable clang tidy in binary dir by placing dummy .clang-tidy file
file(
  WRITE
  ${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy
  "Checks: '-*,llvm-twine-local'" )


install(
  TARGETS arinc665_qt
  RUNTIME_DEPENDENCY_SET runtime-libs
  EXPORT arinc665_qt-config
  COMPONENT runtime
  ARCHIVE COMPONENT development )

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  TYPE INCLUDE
  COMPONENT development
  FILES_MATCHING
    PATTERN "*.hpp" )

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/arinc665_qt_export.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/arinc665_qt
  COMPONENT development )

install(
  EXPORT arinc665_qt-config
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
  COMPONENT development )
