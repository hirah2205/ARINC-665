# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.18 )

add_subdirectory( arinc665_compiler )
add_subdirectory( arinc665_decompiler )
add_subdirectory( arinc665_check )
add_subdirectory( arinc665_ls )

add_subdirectory( arinc665_unit_test )

add_subdirectory( arinc665_print_media_set )
add_subdirectory( arinc665_test_media_set_manager )
add_subdirectory( arinc665_xml_print )

add_subdirectory( arinc665_media_set_view_gui )
