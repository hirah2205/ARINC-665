# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt, Thomas@Thomas-Vogt.de
#
# CMAKE configuration

cmake_minimum_required( VERSION 3.13)

find_package( Doxygen)

if ( NOT TARGET doc)
  add_custom_target( doc COMMENT "Generate Documentation")
endif()

if (DOXYGEN_FOUND)
  set( DOXYGEN_PROJECT_NAME "ARINC 665 Tool Suite")
  set( DOXYGEN_BUILTIN_STL_SUPPORT YES)
  set( DOXYGEN_EXTRACT_PRIVATE YES)
  set( DOXYGEN_EXTRACT_PACKAGE YES)
  set( DOXYGEN_QUIET YES)
#  set( DOXYGEN_WARN_NO_PARAMDOC YES)
  set( DOXYGEN_HTML_DYNAMIC_SECTIONS YES)
  set( DOXYGEN_OUTPUT_DIRECTORY api_doc)
  set( DOXYGEN_GENERATE_DOCBOOK YES)

# Add own CMake scripts to mod
  set( DOXYGEN_STRIP_FROM_PATH
    ${helper_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/application)

  set( DOXYGEN_EXCLUDE_PATTERNS
    "*Test.*")

  doxygen_add_docs(
    arinc665_api_doc

    Mainpage.md
    ${helper_SOURCE_DIR}/helper
    ${CMAKE_SOURCE_DIR}/arinc665
    ${CMAKE_SOURCE_DIR}/arinc665_qt
    ${CMAKE_SOURCE_DIR}/application/arinc665_check
    ${CMAKE_SOURCE_DIR}/application/arinc665_compiler
    ${CMAKE_SOURCE_DIR}/application/arinc665_decompiler
    ${CMAKE_SOURCE_DIR}/application/arinc665_ls
    ${CMAKE_SOURCE_DIR}/application/arinc665_print_media_set
    ${CMAKE_SOURCE_DIR}/application/arinc665_test_media_set_manager
    ${CMAKE_SOURCE_DIR}/application/arinc665_unit_test
    ${CMAKE_SOURCE_DIR}/application/arinc665_xml_print)

  add_dependencies( doc arinc665_api_doc)

  # add index.html redirection
  configure_file(
    ${cmake_common_SOURCE_DIR}/doxygen_index.html.in
    ${DOXYGEN_OUTPUT_DIRECTORY}/index.html
    @ONLY)

  install(
    DIRECTORY ${CMAKE_BINARY_DIR}/doc/${DOXYGEN_OUTPUT_DIRECTORY}
    COMPONENT documentation
    DESTINATION doc
    OPTIONAL)

  file(
    GENERATE
    OUTPUT ${CMAKE_BINARY_DIR}/CMakeGraphVizOptions.cmake
    CONTENT "\
cmake_minimum_required( VERSION 3.12)
set( GRAPHVIZ_EXTERNAL_LIBS FALSE)
set( GRAPHVIZ_GENERATE_PER_TARGET FALSE)
set( GRAPHVIZ_GENERATE_DEPENDERS FALSE)")

  add_custom_target(
    doc_dep
    ${CMAKE_COMMAND} --graphviz=${CMAKE_CURRENT_BINARY_DIR}/doc_dep.dot ${CMAKE_SOURCE_DIR}
    COMMAND ${DOXYGEN_DOT_EXECUTABLE} -Tpdf -O ${CMAKE_CURRENT_BINARY_DIR}/doc_dep.dot)

  #add_dependencies( doc doc_dep)
endif()
