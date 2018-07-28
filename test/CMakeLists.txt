# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de

cmake_minimum_required( VERSION 3.11)

string( RANDOM LENGTH 2048 FILE1_CONTENT)
string( RANDOM LENGTH 4096 FILE2_CONTENT)

file( GENERATE OUTPUT File1 CONTENT ${FILE1_CONTENT})
file( GENERATE OUTPUT File2 CONTENT ${FILE1_CONTENT})
file( GENERATE OUTPUT Readme.txt CONTENT "Readme Content")

add_test(
  NAME arinc665_generate_media_set_rmoutdir
  COMMAND
    ${CMAKE_COMMAND} -E remove_directory MediaSet)

add_test(
  NAME arinc665_generate_media_set
  COMMAND
    arinc665_compiler
    --xml-file ${CMAKE_CURRENT_SOURCE_DIR}/ExampleMediaSet.xml
    --source-directory ${CMAKE_CURRENT_BINARY_DIR}
    --destination-directory MediaSet
    --create-batch-files All
    --create-load-header-files All)
