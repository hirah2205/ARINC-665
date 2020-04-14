# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.16 )

add_executable( arinc665_compiler)

target_sources( arinc665_compiler PRIVATE arinc665_compiler.cpp)

target_link_libraries(
  arinc665_compiler

  PRIVATE
    arinc665)
