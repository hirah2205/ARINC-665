# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.25 )

add_executable( arinc665_print_xml )

target_sources( arinc665_print_xml PRIVATE arinc665_print_xml.cpp )

target_link_libraries( arinc665_print_xml arinc665 )

set_property(
  DIRECTORY ${PROJECT_SOURCE_DIR}
  APPEND
  PROPERTY DOC_PATHS ${CMAKE_CURRENT_SOURCE_DIR} )

install(
  TARGETS arinc665_print_xml
  RUNTIME_DEPENDENCY_SET arinc665-runtime-deps
  COMPONENT runtime )
