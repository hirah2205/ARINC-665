/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set Compiler Application.
 **/

/**
 * @dir
 * @brief ARINC 665 Media Set Compiler Application.
 **/

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/MediaSetExporter.hpp>
#include <arinc665/utils/FileCreationPolicyDescription.hpp>

#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/SupportedArinc665VersionDescription.hpp>
#include <arinc665/Version.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>

/**
 * @brief Program entry point
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   Arguments
 *
 * @return Success state of this operation.
 **/
int main( int argc, char * argv[] );

/**
 * @brief Returns the medium path.
 *
 * @param[in] mediumNumber
 *   Medium Number.
 *
 * @return Medium Path.
 **/
static std::filesystem::path mediumPath(
  const std::filesystem::path &base,
  uint8_t mediumNumber );

/**
 * @brief Creates the directory for the given medium.
 *
 * @param[in] medium
 *   Medium.
 **/
static void createMedium(
  const std::filesystem::path &base,
  Arinc665::Media::ConstMediumPtr medium );

/**
 * @brief Creates the directory for the given directory.
 *
 * @param[in] directory
 *   The directory.
 **/
static void createDirectory(
  const std::filesystem::path &mediaSetBase,
  Arinc665::Media::ConstDirectoryPtr directory );

static bool checkFileExistance(
  const std::filesystem::path &sourceBase,
  const Arinc665::Utils::Arinc665Xml::FilePathMapping &filePathMapping,
  Arinc665::Media::ConstFilePtr file );

static void createFile(
  const std::filesystem::path &sourceBase,
  const std::filesystem::path &mediaSetBase,
  const Arinc665::Utils::Arinc665Xml::FilePathMapping &filePathMapping,
  Arinc665::Media::ConstFilePtr file );

static void writeFile(
  const std::filesystem::path &mediaSetBase,
  uint8_t mediumNumber,
  const std::filesystem::path &path,
  const Arinc665::Files::ConstRawFileSpan &file );

static Arinc665::Files::RawFile readFile(
  const std::filesystem::path &mediaSetBase,
  uint8_t mediumNumber,
  const std::filesystem::path &path );

int main( int argc, char * argv[] )
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  auto fileCreatPolDes{
    Arinc665::Utils::FileCreationPolicyDescription::instance() };

  const std::string fileCreationPolicyValues{
    "* '" + std::string{ fileCreatPolDes.name( Arinc665::Utils::FileCreationPolicy::None)} + "': Create never\n" +
    "* '" + std::string{ fileCreatPolDes.name( Arinc665::Utils::FileCreationPolicy::NoneExisting)} + "': Create none-existing\n" +
    "* '" + std::string{ fileCreatPolDes.name( Arinc665::Utils::FileCreationPolicy::All)} + "': Create all" };

  auto versionDes{ Arinc665::SupportedArinc665VersionDescription::instance() };

  const std::string versionValues{
    "* '" + std::string{ versionDes.name( Arinc665::SupportedArinc665Version::Supplement2 ) } +   "': ARINC 665-2\n" +
    "* '" + std::string{ versionDes.name( Arinc665::SupportedArinc665Version::Supplement345 ) } +  "': ARINC 665-3/4/5" };

  // Media Set XML file
  std::filesystem::path mediaSetXmlFile;
  // Media Set source directory
  std::filesystem::path mediaSetSourceDirectory;
  // Media Set destination directory
  std::filesystem::path mediaSetDestinationDirectory;
  // Create batch file policy
  Arinc665::Utils::FileCreationPolicy createBatchFiles{
    Arinc665::Utils::FileCreationPolicy::Invalid};
  // Create Load Header file policy
  Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles{
    Arinc665::Utils::FileCreationPolicy::Invalid};
  // ARINC 665 Version
  Arinc665::SupportedArinc665Version version{
    Arinc665::SupportedArinc665Version::Invalid};

  boost::program_options::options_description optionsDescription{
    "ARINC 665 Media Set Compiler Options" };

  optionsDescription.add_options()
  (
    "help",
    "print this help screen"
  )
  (
    "xml-file",
    boost::program_options::value( &mediaSetXmlFile )->required(),
    "ARINC 665 media set description file"
  )
  (
    "source-directory",
    boost::program_options::value( &mediaSetSourceDirectory )->required(),
    "ARINC 665 source directory"
  )
  (
    "destination-directory",
    boost::program_options::value( &mediaSetDestinationDirectory )->required(),
    "Output directory for ARINC 665 media set"
  )
  (
    "create-batch-files",
    boost::program_options::value(
      &createBatchFiles )->default_value( Arinc665::Utils::FileCreationPolicy::None ),
    ( std::string( "batch-files creation policy:\n" ) + fileCreationPolicyValues ).c_str()
  )
  (
    "create-load-header-files",
    boost::program_options::value( &createLoadHeaderFiles)->default_value(
      Arinc665::Utils::FileCreationPolicy::None ),
    ( std::string( "Load-headers-files creation policy:\n" ) + fileCreationPolicyValues).c_str()
  )
  (
    "version",
    boost::program_options::value( &version )->default_value(
      Arinc665::SupportedArinc665Version::Supplement2 ),
    ( std::string( "ARINC 665 Version:\n" ) + versionValues ).c_str()
  );

  try
  {
    std::cout
      << "ARINC 665 Media Set Compiler - "
      << Arinc665::Version::VersionInformation << "\n";

    boost::program_options::variables_map options{};

    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription ),
      options );

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( options);

    // ARINC 665 XML instance
    auto xml{ Arinc665::Utils::Arinc665Xml::instance()};

    // load XML file
    auto [ mediaSet, fileMapping ] = xml->loadFromXml( mediaSetXmlFile );

    // Add Media Set Part Number to Output Path
    mediaSetDestinationDirectory /= mediaSet->partNumber();

    // create media set directory
    std::filesystem::create_directories( mediaSetDestinationDirectory );

    auto exporter{ Arinc665::Utils::MediaSetExporter::create() };

    // set exporter parameters
    exporter->mediaSet( mediaSet )
      .createMediumHandler( std::bind(
        &createMedium,
        mediaSetDestinationDirectory,
        std::placeholders::_1 ) )
      .createDirectoryHandler( std::bind(
        &createDirectory,
        mediaSetDestinationDirectory,
        std::placeholders::_1 ) )
      .checkFileExistenceHandler( std::bind(
        &checkFileExistance,
        mediaSetSourceDirectory,
        fileMapping,
        std::placeholders::_1 ) )
      .createFileHandler( std::bind(
        &createFile,
        mediaSetSourceDirectory,
        mediaSetDestinationDirectory,
        fileMapping,
        std::placeholders::_1 ) )
      .writeFileHandler( std::bind(
        &writeFile,
        mediaSetDestinationDirectory,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3 ) )
      .readFileHandler( std::bind(
        &readFile,
        mediaSetDestinationDirectory,
        std::placeholders::_1,
        std::placeholders::_2 ) )
      .arinc665Version( version )
      .createBatchFiles( createBatchFiles )
      .createLoadHeaderFiles( createLoadHeaderFiles );

    (*exporter)();
  }
  catch ( boost::program_options::error &e )
  {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( Arinc665::Arinc665Exception &e )
  {
    std::cerr
      << "Arinc665Exception in compiler: "
      << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e )
  {
    std::cerr
      << "BOOST Error in compiler: "
      << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cerr << "std Error in compiler: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in compiler: UNKNOWN EXCEPTION\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static std::filesystem::path mediumPath(
  const std::filesystem::path &base,
  const uint8_t mediumNumber )
{
  return base
    / (boost::format("MEDIUM_%03u" ) % (unsigned int)mediumNumber).str();
}

static void createMedium(
  const std::filesystem::path &base,
  Arinc665::Media::ConstMediumPtr medium )
{
  BOOST_LOG_FUNCTION()

  auto mPath{ mediumPath( base, medium->mediumNumber() ) };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Create medium directory " << mPath;

  std::filesystem::create_directory( mPath);
}

static void createDirectory(
  const std::filesystem::path &mediaSetBase,
  Arinc665::Media::ConstDirectoryPtr directory )
{
  BOOST_LOG_FUNCTION()

  auto directoryPath{
    mediumPath( mediaSetBase, directory->medium()->mediumNumber() )
      / directory->path().relative_path()};

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Create directory " << directoryPath;

  std::filesystem::create_directory( directoryPath);
}

static bool checkFileExistance(
  const std::filesystem::path &sourceBase,
  const Arinc665::Utils::Arinc665Xml::FilePathMapping &filePathMapping,
  Arinc665::Media::ConstFilePtr file )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "check existence of " << file->path();

  // search for file
  auto fileIt{ filePathMapping.find( file ) };

  if ( fileIt == filePathMapping.end() )
  {
    return false;
  }

  return std::filesystem::is_regular_file( sourceBase / fileIt->second );
}

static void createFile(
  const std::filesystem::path &sourceBase,
  const std::filesystem::path &mediaSetBase,
  const Arinc665::Utils::Arinc665Xml::FilePathMapping &filePathMapping,
  Arinc665::Media::ConstFilePtr file )
{
  BOOST_LOG_FUNCTION()

  // search for file
  auto fileIt{ filePathMapping.find( file ) };

  if ( fileIt == filePathMapping.end() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "file mapping not found")
      << boost::errinfo_file_name( std::string{ file->name() } ) );
  }

  auto filePath{
    mediumPath( mediaSetBase, file->medium()->mediumNumber() )
      / file->path().relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Copy file " << filePath;

  // copy file
  std::filesystem::copy(
    sourceBase / fileIt->second,
    filePath );
}

static void writeFile(
  const std::filesystem::path &mediaSetBase,
  const uint8_t mediumNumber,
  const std::filesystem::path &path,
  const Arinc665::Files::ConstRawFileSpan &file )
{
  BOOST_LOG_FUNCTION()

  auto filePath{
    mediumPath( mediaSetBase, mediumNumber) / path.relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Write file " << filePath;

  // check existence of file
  if (std::filesystem::exists( filePath))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "File already exists" )
      << boost::errinfo_file_name( filePath.string() ) );
  }

  // save file
  std::ofstream fileStream(
    filePath.string(),
    std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

  if ( !fileStream.is_open())
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // write the data to the buffer
  fileStream.write(
    (char*) file.data(),
    static_cast< std::streamsize >( file.size() ) );
}

static Arinc665::Files::RawFile readFile(
  const std::filesystem::path &mediaSetBase,
  const uint8_t mediumNumber,
  const std::filesystem::path &path)
{
  BOOST_LOG_FUNCTION()

  // check medium number
  auto filePath{
    mediumPath( mediaSetBase, mediumNumber ) / path.relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Read file " << filePath;

  // check existence of file
  if ( !std::filesystem::is_regular_file( filePath ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  Arinc665::Files::RawFile data( std::filesystem::file_size( filePath ) );

  // load file
  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in );

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // read the data to the buffer
  file.read(
    (char*) &data.at( 0),
    static_cast< std::streamsize >( data.size() ) );

  // return the buffer
  return data;
}
