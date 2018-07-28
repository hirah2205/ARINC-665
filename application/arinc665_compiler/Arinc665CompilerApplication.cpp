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
#include <arinc665/utils/FileCreationPolicyDescription.hpp>

#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

#include <cstdlib>
#include <memory>
#include <fstream>
#include <iostream>

Arinc665CompilerApplication::Arinc665CompilerApplication() :
  optionsDescription( "ARINC 665 Media Set Compiler Options"),
  createBatchFiles( Arinc665::Utils::FileCreationPolicy::Invalid),
  createLoadHeaderFiles( Arinc665::Utils::FileCreationPolicy::Invalid),
  xml( Arinc665::Utils::Arinc665Xml::createInstance())
{
  auto desc{ Arinc665::Utils::FileCreationPolicyDescription::instance()};

  const std::string fileCreationPolicyValues{
    "* '" + desc.name( Arinc665::Utils::FileCreationPolicy::None) + "': Create never\n" +
    "* '" + desc.name( Arinc665::Utils::FileCreationPolicy::NoneExisting) + "': Create none-existing\n" +
    "* '" + desc.name( Arinc665::Utils::FileCreationPolicy::All) + "': Create all"};

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
    boost::program_options::value( &createBatchFiles)->default_value(
      Arinc665::Utils::FileCreationPolicy::None),
    (std::string( "batch-files creation policy:\n") + fileCreationPolicyValues).c_str()
  )
  (
    "create-load-header-files",
    boost::program_options::value( &createLoadHeaderFiles)->default_value(
      Arinc665::Utils::FileCreationPolicy::None),
    (std::string( "Load-headers-files creation policy:\n") + fileCreationPolicyValues).c_str()
  );
}

int Arinc665CompilerApplication::operator()( int argc, char *argv[])
{
  BOOST_LOG_FUNCTION();

  try
  {
    std::cout << "ARINC 665 Media Set Compiler\n";

    boost::program_options::variables_map options;

    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription),
      options);

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( options);

    // load XML file
    auto result( xml->loadFromXml( mediaSetXmlFile));

    // create media set directory
    std::filesystem::create_directory( mediaSetDestinationDirectory);

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
        &Arinc665CompilerApplication::checkFileExistance,
        this,
        result,
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
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
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

std::filesystem::path Arinc665CompilerApplication::mediumPath(
  const uint8_t mediumNumber) const
{
  return mediaSetDestinationDirectory
    / (boost::format("MEDIUM_%03u") % (unsigned int)mediumNumber).str();
}

void Arinc665CompilerApplication::createMedium(
  Arinc665::Media::ConstMediumPtr medium)
{
  BOOST_LOG_FUNCTION();

  auto mPath{ mediumPath( medium->mediumNumber())};

  BOOST_LOG_TRIVIAL( severity_level::info) << "Create medium directory " <<
    mPath;

  std::filesystem::create_directory( mPath);
}

void Arinc665CompilerApplication::createDirectory(
  Arinc665::Media::ConstDirectoryPtr directory)
{
  BOOST_LOG_FUNCTION();

  auto directoryPath{
    mediumPath( directory->medium()->mediumNumber()) /
      directory->path().relative_path()};

  BOOST_LOG_TRIVIAL( severity_level::info) << "Create directory " <<
    directoryPath;

  std::filesystem::create_directory( directoryPath);
}

bool Arinc665CompilerApplication::checkFileExistance(
  const Arinc665::Utils::Arinc665Xml::LoadXmlResult &mediaSetInfo,
  Arinc665::Media::ConstFilePtr file)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_TRIVIAL( severity_level::info) << "check existence of " <<
    file->path();

  // search for file
  auto fileIt( std::get< 1>( mediaSetInfo).find( file));

  if (fileIt == std::get< 1>( mediaSetInfo).end())
  {
    return false;
  }

  auto filePath{
    mediumPath( file->medium()->mediumNumber()) /
      file->path().relative_path()};

  return std::filesystem::is_regular_file( filePath);
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
      boost::errinfo_file_name( file->name()));
  }

  auto filePath{
    mediumPath( file->medium()->mediumNumber()) /
      file->path().relative_path()};

  BOOST_LOG_TRIVIAL( severity_level::info) << "Copy file " << filePath;

  // copy file
  std::filesystem::copy(
    mediaSetSourceDirectory / fileIt->second,
    filePath);
}

void Arinc665CompilerApplication::writeFile(
  const uint8_t mediumNumber,
  const std::filesystem::path &path,
  const Arinc665::File::RawFile &file)
{
  BOOST_LOG_FUNCTION();

  auto filePath{ mediumPath( mediumNumber) / path.relative_path()};

  BOOST_LOG_TRIVIAL( severity_level::info) << "Write file " << filePath;

  // check existence of file
  if (std::filesystem::exists( filePath))
  {
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
  const std::filesystem::path &path)
{
  BOOST_LOG_FUNCTION();

  // check medium number
  auto filePath{ mediumPath( mediumNumber) / path.relative_path()};

  BOOST_LOG_TRIVIAL( severity_level::info) << "Read file " << filePath;

  // check existence of file
  if (!std::filesystem::is_regular_file( filePath))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File not found") <<
      boost::errinfo_file_name( filePath.string()));
  }

  Arinc665::File::RawFile data( std::filesystem::file_size( filePath));

  // load file
  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in);

  if ( !file.is_open())
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "Error opening files") <<
      boost::errinfo_file_name( filePath.string()));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}

