# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, thomas@thomas-vogt.de

cmake_minimum_required( VERSION 3.18 )

string( RANDOM LENGTH 2048 FILE1_CONTENT )
string( RANDOM LENGTH 4096 FILE2_CONTENT )
string( RANDOM LENGTH 8192 FILE3_CONTENT )
string( RANDOM LENGTH 16384 FILE4_CONTENT )

file( GENERATE OUTPUT File1 CONTENT ${FILE1_CONTENT} )
file( GENERATE OUTPUT File2 CONTENT ${FILE2_CONTENT} )
file( GENERATE OUTPUT File3 CONTENT ${FILE3_CONTENT} )
file( GENERATE OUTPUT File4 CONTENT ${FILE4_CONTENT} )
file( GENERATE OUTPUT Readme.txt CONTENT "Readme Content" )

set( MEDIA_SET_OUTPUT MediaSet_V2 )
configure_file(
  MediaSetConfig.json.in
  MediaSetConfig_v2.json )
set( MEDIA_SET_OUTPUT MediaSet_V3 )
configure_file(
  MediaSetConfig.json.in
  MediaSetConfig_v3.json )

add_test(
  NAME arinc665_generate_media_set_rmoutdir_v2
  COMMAND
  ${CMAKE_COMMAND} -E remove_directory MediaSet_V2 )
add_test(
  NAME arinc665_generate_media_set_rmoutdir_v3
  COMMAND
  ${CMAKE_COMMAND} -E remove_directory MediaSet_V3 )

add_test(
  NAME arinc665_generate_media_set_v2
  COMMAND
    arinc665_compiler
    --xml-file ${CMAKE_CURRENT_SOURCE_DIR}/ExampleMediaSet.xml
    --source-directory ${CMAKE_CURRENT_BINARY_DIR}
    --destination-directory MediaSet_V2
    --create-batch-files All
    --create-load-header-files All )

add_test(
  NAME arinc665_decompile_media_set_v2
  COMMAND
    arinc665_decompiler
    --xml-file ${CMAKE_CURRENT_BINARY_DIR}/DecompiledMediaSet.xml
    --source-directory ${CMAKE_CURRENT_BINARY_DIR}/MediaSet_V2/CCC/MEDIUM_001 )


add_test(
  NAME arinc665_generate_media_set_v3
  COMMAND
  arinc665_compiler
  --xml-file ${CMAKE_CURRENT_SOURCE_DIR}/ExampleMediaSet.xml
  --source-directory ${CMAKE_CURRENT_BINARY_DIR}
  --destination-directory MediaSet_V3
  --create-batch-files All
  --create-load-header-files All
  --version Supplement345 )

add_test(
  NAME arinc665_decompile_media_set_v3
  COMMAND
  arinc665_decompiler
  --xml-file ${CMAKE_CURRENT_BINARY_DIR}/DecompiledMediaSetV3.xml
  --source-directory ${CMAKE_CURRENT_BINARY_DIR}/MediaSet_V3/CCC/MEDIUM_001 )
