# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.16)

find_package( Boost REQUIRED)

# prepare Version.cpp
configure_file(
  Version.cpp.in
  Version.cpp)

add_library( arinc665)

target_sources(
  arinc665

  PRIVATE
    Arinc665.hpp
    Arinc665.cpp
    Arinc665Crc.cpp
    Arinc665Crc.hpp
    Arinc665Exception.hpp
    Arinc665Logger.hpp
    FileTypeDescription.hpp
    FileTypeDescription.cpp
    PartNumber.hpp
    PartNumber.cpp
    SupportedArinc665VersionDescription.cpp
    SupportedArinc665VersionDescription.hpp
    Version.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/Version.cpp)

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


add_library( arinc665_test OBJECT)

target_sources(
  arinc665_test

  PRIVATE
    Arinc665CrcTest.cpp
    PartNumberTest.cpp
    VersionTest.cpp)

target_compile_options(
  arinc665_test

  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:-W3>

    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:GNU>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU>:-Werror>

    $<$<CXX_COMPILER_ID:Clang>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Werror>)

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

target_link_libraries(
  arinc665_test
  PUBLIC arinc665)

add_subdirectory( file)
add_subdirectory( media)
add_subdirectory( utils)

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
