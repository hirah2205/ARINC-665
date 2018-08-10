# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.12)

project( 
  arinc665 
  VERSION 1.0.0.0
  DESCRIPTION "ARINC 665")

add_subdirectory( arinc665)
add_subdirectory( arinc665_qt)

add_subdirectory( application)
add_subdirectory( test)
