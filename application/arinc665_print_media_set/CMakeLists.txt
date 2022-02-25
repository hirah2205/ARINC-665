# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.21 )

add_executable( arinc665_print_media_set )

target_sources( arinc665_print_media_set PRIVATE arinc665_print_media_set.cpp )

target_link_libraries( arinc665_print_media_set arinc665 )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

install( TARGETS arinc665_print_media_set COMPONENT runtime )
