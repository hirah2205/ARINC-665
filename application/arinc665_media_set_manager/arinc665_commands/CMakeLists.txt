# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.24 )

add_library( arinc665_commands STATIC )

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..> )

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
    commands )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

install(
  TARGETS arinc665_commands
  COMPONENT runtime
  ARCHIVE COMPONENT development )
