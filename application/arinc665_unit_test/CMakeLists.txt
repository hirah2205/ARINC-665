# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

add_executable( arinc665_unit_test )

target_sources( arinc665_unit_test PRIVATE arinc665_unit_test.cpp )

target_link_libraries(
  arinc665_unit_test

  PRIVATE
    arinc665_test
    arinc645_test
    helper_test )

add_test( NAME arinc665_unit_test COMMAND arinc665_unit_test )

install( TARGETS arinc665_unit_test COMPONENT runtime )
