# $Date: 2017-05-16 13:05:39 +0200 (Di, 16. Mai 2017) $
# $Revision: 2013 $
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.8)

add_executable( 
  arinc665_ls 
  arinc665_ls.cpp)

target_link_libraries( arinc665_ls arinc665)
