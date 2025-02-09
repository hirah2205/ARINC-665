# SPDX-License-Identifier: MPL-2.0

# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
# If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required( VERSION 3.31 )

target_sources(
  arinc_665

  PRIVATE
    $<$<EQUAL:${LIBXMLPPVERSION},26>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlLoadImpl26.hpp>
    $<$<EQUAL:${LIBXMLPPVERSION},26>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlLoadImpl26.cpp>
    $<$<EQUAL:${LIBXMLPPVERSION},26>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlSaveImpl26.hpp>
    $<$<EQUAL:${LIBXMLPPVERSION},26>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlSaveImpl26.cpp>
    $<$<EQUAL:${LIBXMLPPVERSION},5>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlLoadImpl5.hpp>
    $<$<EQUAL:${LIBXMLPPVERSION},5>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlLoadImpl5.cpp>
    $<$<EQUAL:${LIBXMLPPVERSION},5>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlSaveImpl5.hpp>
    $<$<EQUAL:${LIBXMLPPVERSION},5>:${CMAKE_CURRENT_SOURCE_DIR}/Arinc665XmlSaveImpl5.cpp>
    FilesystemMediaSetCompilerImpl.hpp
    FilesystemMediaSetCompilerImpl.cpp
    FilesystemMediaSetCopierImpl.hpp
    FilesystemMediaSetCopierImpl.cpp
    FilesystemMediaSetDecompilerImpl.hpp
    FilesystemMediaSetDecompilerImpl.cpp
    FilesystemMediaSetRemoverImpl.hpp
    FilesystemMediaSetRemoverImpl.cpp
    MediaSetCompilerImpl.hpp
    MediaSetCompilerImpl.cpp
    MediaSetDecompilerImpl.hpp
    MediaSetDecompilerImpl.cpp
    MediaSetManagerImpl.hpp
    MediaSetManagerImpl.cpp
    MediaSetValidatorImpl.cpp
    MediaSetValidatorImpl.hpp )

#target_sources(
#  arinc_665_test
#
#  PRIVATE)
