# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.24 )

find_package( fmt REQUIRED )

add_library( arinc665_commands )

target_sources(
  arinc665_commands

  PRIVATE
    Arinc665Commands.hpp
    Arinc665Commands.cpp
    CreateMediaSetManagerCommand.hpp
    CreateMediaSetManagerCommand.cpp
    ImportMediaSetCommand.hpp
    ImportMediaSetCommand.cpp
    ImportMediaSetXmlCommand.hpp
    ImportMediaSetXmlCommand.cpp
    ListLoadsCommand.hpp
    ListLoadsCommand.cpp
    ListMediaSetsCommand.hpp
    ListMediaSetsCommand.cpp
    RemoveMediaSetCommand.hpp
    RemoveMediaSetCommand.cpp )

target_include_directories(
  arinc665_commands
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..> )

target_compile_features( arinc665_commands PUBLIC cxx_std_20 )

target_compile_options(
  arinc665_commands

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra> )

target_link_libraries(
  arinc665_commands

  PUBLIC
    arinc665
    commands

  PRIVATE
    fmt::fmt )

include( GenerateExportHeader )
generate_export_header( arinc665_commands )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

install(
  TARGETS arinc665_commands
  RUNTIME_DEPENDENCY_SET runtime-libs
  EXPORT arinc665_commands-config
  COMPONENT runtime
  ARCHIVE COMPONENT development )

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  TYPE INCLUDE
  COMPONENT development
  FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*Test.*" EXCLUDE
    PATTERN "implementation" EXCLUDE )

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/arinc665_commands_export.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/arinc665_commands
  COMPONENT development )

install(
  EXPORT arinc665_commands-config
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
  COMPONENT development )
