/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665CompilerApplication.
 **/

#include "Arinc665CompilerApplication.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc665/media/Medium.hpp>
#include <arinc665/media/File.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/application.hpp>

#include <cstdlib>
#include <memory>
#include <fstream>

Arinc665CompilerApplication::Arinc665CompilerApplication(
  boost::application::context &context) :
  context( context),
  optionsDescription( "ARINC 665 Media Set Compiler Options"),
  xml( Arinc665::Utils::Arinc665Xml::createInstance())
{
  optionsDescription.add_options()
    (
      "help",
      "print this help screen"
    )
    (
      "xml-file",
      boost::program_options::value( &mediaSetXmlFile)->required(),
      "ARINC 665 media set description file"
    )
    (
      "source-directory",
      boost::program_options::value( &mediaSetSourceDirectory)->required(),
      "ARINC 665 source directory"
    )
    (
      "destination-directory",
      boost::program_options::value( &mediaSetDestinationDirectory)->required(),
      "Output directory for ARINC 665 media set"
    );
}

int Arinc665CompilerApplication::operator()()
{
  try
  {
    std::cout << "ARINC 665 Media Set Compiler" << std::endl;

    if ( !handleCommandLine())
    {
      return EXIT_FAILURE;
    }

    // load XML file
    auto result( xml->loadFromXml( mediaSetXmlFile));

    auto exporter( Arinc665::Utils::Arinc665Utils::createArinc665Exporter(
      std::get< 0>( result),
      std::bind(
        &Arinc665CompilerApplication::createFile,
        this,
        result,
        std::placeholders::_1),
      std::bind(
        &Arinc665CompilerApplication::writeFile,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)));

    exporter();
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::string const * info = boost::get_error_info< AdditionalInfo>( e);

    std::cerr <<
      "compiler failed: " <<
//    	typid( e).name() << " - " <<
      ((nullptr==info) ? "Unknown" : *info) <<
      std::endl;

    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "Error in compiler: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in compiler: UNKNOWN EXCEPTION" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

bool Arinc665CompilerApplication::handleCommandLine()
{
  try
  {
    std::shared_ptr< boost::application::args> args =
      context.find< boost::application::args>();

    boost::program_options::variables_map options;
    boost::program_options::store(
      boost::program_options::parse_command_line(
        args->argc(),
        args->argv(),
        optionsDescription),
      options);
    boost::program_options::notify( options);

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return false;
    }
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl << optionsDescription << std::endl;
    return false;
  }

  return true;
}

void Arinc665CompilerApplication::createFile(
  const Arinc665::Utils::Arinc665Xml::LoadXmlResult &mediaSetInfo,
  Arinc665::Media::ConstFilePtr file)
{
  // search for file
  auto fileIt( std::get< 1>( mediaSetInfo).find( file));

  if (fileIt == std::get< 1>( mediaSetInfo).end())
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "file mapping not found"));
  }

  auto filePath(
    (mediaSetDestinationDirectory /
    ("MEDIA_" + std::to_string( file->getMedium()->getMediumNumber())) /
    file->getPath()));

  // create directories (if necessary)
  boost::filesystem::create_directories( filePath.parent_path());

  // copy file
  boost::filesystem::copy(
    mediaSetSourceDirectory / fileIt->second,
    filePath);
}

void Arinc665CompilerApplication::writeFile(
  uint8_t mediumNumber,
  const path &path,
  Arinc665::File::RawFile file)
{
  auto filePath(
    mediaSetDestinationDirectory /
    ("MEDIA_" + std::to_string( mediumNumber)) /
    path.relative_path());

  BOOST_LOG_TRIVIAL( severity_level::info) << "Write file " << filePath;

  // check existence of file
  if (boost::filesystem::exists( filePath))
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() <<
        boost::errinfo_file_name( filePath.string()) <<
        AdditionalInfo( "File already exists"));
  }

  // create directories (if necessary)
  boost::filesystem::create_directories( filePath.parent_path());

  // save file
  std::ofstream fileStream(
    filePath.string(),
    std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

  if ( !fileStream.is_open())
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "Error opening files"));
  }

  // write the data to the buffer
  fileStream.write( (char*) &file.at( 0), file.size());
}

