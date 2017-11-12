/*
 * $Date: 2017-05-24 23:37:46 +0200 (Mi, 24. Mai 2017) $
 * $Revision: 2059 $
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
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/application.hpp>
#include <boost/format.hpp>

#include <cstdlib>
#include <memory>
#include <fstream>

Arinc665CompilerApplication::Arinc665CompilerApplication(
  boost::application::context &context) :
  context( context),
  optionsDescription( "ARINC 665 Media Set Compiler Options"),
  createBatchFiles( false),
  createLoadHeaderFiles( false),
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
    )
    (
      "create-batch-files",
      boost::program_options::bool_switch( &createBatchFiles)->default_value( false),
      "Create batch-files instead coping it."
    )
    (
      "create-load-header-files",
      boost::program_options::bool_switch( &createLoadHeaderFiles)->default_value( false),
      "Create load-headers-files instead coping it."
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

    // create media set directory
    boost::filesystem::create_directory( mediaSetDestinationDirectory);

    auto exporter( Arinc665::Utils::Arinc665Utils::createArinc665Exporter(
      std::get< 0>( result),
      std::bind(
        &Arinc665CompilerApplication::createMedium,
        this,
        std::placeholders::_1),
      std::bind(
        &Arinc665CompilerApplication::createDirectory,
        this,
        std::placeholders::_1),
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
        std::placeholders::_3),
      std::bind(
        &Arinc665CompilerApplication::readFile,
        this,
        std::placeholders::_1,
        std::placeholders::_2),
      Arinc665::Arinc665Version::ARINC_665_2,
      createBatchFiles,
      createLoadHeaderFiles));

    exporter();
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::cerr << "Arinc665Exception in compiler: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "BOOST Error in compiler: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cerr << "std Error in compiler: " << e.what() << std::endl;
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
    std::shared_ptr< boost::application::args> args(
      context.find< boost::application::args>());

    boost::program_options::variables_map options;

    boost::program_options::store(
      boost::program_options::parse_command_line(
        args->argc(),
        args->argv(),
        optionsDescription),
      options);

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return false;
    }

    boost::program_options::notify( options);
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl << optionsDescription << std::endl;
    return false;
  }

  return true;
}

Arinc665CompilerApplication::path Arinc665CompilerApplication::getMediumPath(
  const uint8_t mediumNumber) const
{
  return mediaSetDestinationDirectory
    / (boost::format("MEDIUM_%03u") % (unsigned int)mediumNumber).str();
}

void Arinc665CompilerApplication::createMedium(
  Arinc665::Media::ConstMediumPtr medium)
{
  BOOST_LOG_FUNCTION();

  auto mediumPath( getMediumPath( medium->getMediumNumber()));

  BOOST_LOG_TRIVIAL( severity_level::info) << "Create medium directory " <<
    mediumPath;

  boost::filesystem::create_directory( mediumPath);
}

void Arinc665CompilerApplication::createDirectory(
  Arinc665::Media::ConstDirectoryPtr directory)
{
  BOOST_LOG_FUNCTION();

  auto directoryPath(
    getMediumPath( directory->getMedium()->getMediumNumber()) /
    directory->getPath());

  BOOST_LOG_TRIVIAL( severity_level::info) << "Create directory " <<
    directoryPath;

  boost::filesystem::create_directory( directoryPath);
}

void Arinc665CompilerApplication::createFile(
  const Arinc665::Utils::Arinc665Xml::LoadXmlResult &mediaSetInfo,
  Arinc665::Media::ConstFilePtr file)
{
  BOOST_LOG_FUNCTION();

  // search for file
  auto fileIt( std::get< 1>( mediaSetInfo).find( file));

  if (fileIt == std::get< 1>( mediaSetInfo).end())
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "file mapping not found") <<
      boost::errinfo_file_name( file->getName()));
  }

  auto filePath(
    getMediumPath( file->getMedium()->getMediumNumber()) /
    file->getPath());

  BOOST_LOG_TRIVIAL( severity_level::info) << "Copy file " << filePath;

  // copy file
  boost::filesystem::copy(
    mediaSetSourceDirectory / fileIt->second,
    filePath);
}

void Arinc665CompilerApplication::writeFile(
  const uint8_t mediumNumber,
  const path &path,
  const Arinc665::File::RawFile &file)
{
  BOOST_LOG_FUNCTION();

  auto filePath( getMediumPath( mediumNumber) / path.relative_path());

  BOOST_LOG_TRIVIAL( severity_level::info) << "Write file " << filePath;

  // check existence of file
  if (boost::filesystem::exists( filePath))
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() <<
        AdditionalInfo( "File already exists") <<
        boost::errinfo_file_name( filePath.string()));
  }

  // save file
  std::ofstream fileStream(
    filePath.string(),
    std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

  if ( !fileStream.is_open())
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "Error opening files") <<
      boost::errinfo_file_name( filePath.string()));
  }

  // write the data to the buffer
  fileStream.write( (char*) &file.at( 0), file.size());
}

Arinc665::File::RawFile Arinc665CompilerApplication::readFile(
  const uint8_t mediumNumber,
  const path &path)
{
  BOOST_LOG_FUNCTION();

  // check medium number
  auto filePath( getMediumPath( mediumNumber) / path.relative_path());

  BOOST_LOG_TRIVIAL( severity_level::info) << "Read file " << filePath;

  // check existence of file
  if (!boost::filesystem::is_regular( filePath))
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File not found") <<
      boost::errinfo_file_name( filePath.string()));
  }

  Arinc665::File::RawFile data( boost::filesystem::file_size( filePath));

  // load file
  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in);

  if ( !file.is_open())
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "Error opening files") <<
      boost::errinfo_file_name( filePath.string()));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}

