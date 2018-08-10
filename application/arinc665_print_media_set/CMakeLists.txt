# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.12)

add_executable(
  arinc665_print_media_set 
  arinc665_print_media_set.cpp
)

target_link_libraries( arinc665_print_media_set arinc665)
