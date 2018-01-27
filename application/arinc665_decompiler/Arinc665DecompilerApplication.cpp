/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665DecompilerApplication.
 **/

#include "Arinc665DecompilerApplication.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <cstdlib>
#include <memory>
#include <fstream>
#include <iostream>

Arinc665DecompilerApplication::Arinc665DecompilerApplication() :
  optionsDescription( "ARINC 665 Media Set Decompiler Options")
{
  optionsDescription.add_options()
    (
      "help",
      "print this help screen"
    )
    (
      "source-directory",
      boost::program_options::value(
        &mediaSourceDirectories)->required()->composing(),
      "ARINC 665 media source directories"
    )
    (
      "xml-file",
      boost::program_options::value( &mediaSetXmlFile)->required(),
      "Output ARINC 665 media set description XML"
    );
}

int Arinc665DecompilerApplication::operator()( int argc, char *argv[])
{
  try
  {
    std::cout << "ARINC 665 Media Set Decompiler" << std::endl;

    boost::program_options::variables_map options;
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, optionsDescription),
      options);
    boost::program_options::notify( options);

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return EXIT_FAILURE;
    }
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl << optionsDescription << std::endl;
    return EXIT_FAILURE;
  }

  try
  {
    // create importer
    auto importer( Arinc665::Utils::Arinc665Utils::createArinc665Importer(
      std::bind(
        &Arinc665DecompilerApplication::readFile,
        this,
        std::placeholders::_1,
        std::placeholders::_2)));

    // perform import
    auto result( importer());

    Arinc665::Utils::Arinc665Xml::FilePathMapping fileMapping;

    // iterate over files
    for (auto file : result->files())
    {
      boost::filesystem::path filePath(
        mediaSourceDirectories[ file->medium()->mediumNumber()-1] /
        file->path().relative_path());

      fileMapping.insert( {file, filePath});
    }

    // XML exporter
    auto xml( Arinc665::Utils::Arinc665Xml::createInstance());

    // export the XML file
    xml->saveToXml( result, fileMapping, mediaSetXmlFile);
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::string const * info = boost::get_error_info< AdditionalInfo>( e);

    std::cerr <<
      "decompiler failed: " <<
//      typid( e).name() << " - " <<
      ((nullptr==info) ? "Unknown" : *info) <<
      std::endl;

    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "Error in decompiler: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in decompiler: UNKNOWN EXCEPTION" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

Arinc665::File::RawFile Arinc665DecompilerApplication::readFile(
  const uint8_t mediumNumber,
  const path &path)
{
  // check medium number
  if (mediumNumber > this->mediaSourceDirectories.size())
  {
    return {};
  }

  auto filePath(
    this->mediaSourceDirectories[ mediumNumber-1] / path.relative_path());

  // check existence of file
  if (!boost::filesystem::is_regular( filePath))
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() <<
        boost::errinfo_file_name( filePath.string()) <<
        AdditionalInfo( "File not found"));
  }

  Arinc665::File::RawFile data( boost::filesystem::file_size( filePath));

  // load file
  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in);

  if ( !file.is_open())
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "Error opening files"));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}
