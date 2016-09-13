# $Date$
# $Revision$
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.5)

project( "ARINC 665 unit test application")

find_package( Boost REQUIRED COMPONENTS unit_test_framework)

add_executable( arinc665_unit_test arinc665_unit_test.cpp)

target_link_libraries( 
  arinc665_unit_test 

  arinc665
  ${Boost_LIBRARIES}
  ${BOOST_THREAD_LIBRARY})
