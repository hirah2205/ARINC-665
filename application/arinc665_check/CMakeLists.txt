# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

add_executable( arinc665_check )

target_sources( arinc665_check PRIVATE arinc665_check.cpp )

target_link_libraries( arinc665_check PRIVATE arinc665 )

install( TARGETS arinc665_check COMPONENT runtime )
