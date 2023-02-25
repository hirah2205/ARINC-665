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

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/MediaSetExporter.hpp>
#include <arinc665/utils/FileCreationPolicyDescription.hpp>

#include <arinc665/media/Directory.hpp>
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/SupportedArinc665VersionDescription.hpp>
#include <arinc665/Version.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <fmt/format.h>

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
  const Arinc665::MediumNumber &mediumNumber );

/**
 * @brief Creates the Directory for the given Medium.
 *
 * @param[in] medium
 *   Medium to Create.
 **/
static void createMedium(
  const std::filesystem::path &base,
  const Arinc665::MediumNumber &mediumNumber );

/**
 * @brief Creates the given Directory.
 *
 * @param[in] directory
 *   Directory to Create.
 **/
static void createDirectory(
  const std::filesystem::path &mediaSetBase,
  const Arinc665::MediumNumber &mediumNumber,
  const Arinc665::Media::ConstDirectoryPtr &directory );

/**
 * @brief Check File Existence Handler.
 *
 * @param[in] file
 *   File to Check
 *
 * @return If file exists
 **/
static bool checkFileExistence(
  const std::filesystem::path &sourceBase,
  const Arinc665::Utils::FilePathMapping &filePathMapping,
  const Arinc665::Media::ConstFilePtr &file );

/**
 * @brief Create File Handler.
 *
 * @param[in] file
 *   File to Create
 **/
static void createFile(
  const std::filesystem::path &sourceBase,
  const std::filesystem::path &mediaSetBase,
  const Arinc665::Utils::FilePathMapping &filePathMapping,
  const Arinc665::Media::ConstFilePtr &file );

/**
 * @brief Write File Handler
 *
 * @param[in] mediumNumber
 *   Medium Number
 * @param[in] path
 *   File Path
 * @param[in] file
 *   File Content
 **/
static void writeFile(
  const std::filesystem::path &mediaSetBase,
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path,
  const Arinc665::Files::ConstRawFileSpan &file );

/**
 * @brief Read File Handler
 *
 * @param[in] mediumNumber
 *   Medium number.
 * @param[in] path
 *   File Path
 *
 * @return File Content
 **/
static Arinc665::Files::RawFile readFile(
  const std::filesystem::path &mediaSetBase,
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path );

int main( int argc, char * argv[] )
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  try
  {
    std::cout
      << "ARINC 665 Media Set Compiler - "
      << Arinc665::Version::VersionInformation << "\n";

    const auto &fileCreatePolDesc{
      Arinc665::Utils::FileCreationPolicyDescription::instance() };

    const std::string fileCreationPolicyValues{
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::None)} + "': Create never\n" +
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::NoneExisting)} + "': Create none-existing\n" +
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::All)} + "': Create all" };

    const auto &versionDesc{ Arinc665::SupportedArinc665VersionDescription::instance() };

    const std::string versionValues{
      "* '" + std::string{ versionDesc.name( Arinc665::SupportedArinc665Version::Supplement2 ) } +   "': ARINC 665-2\n" +
      "* '" + std::string{ versionDesc.name( Arinc665::SupportedArinc665Version::Supplement345 ) } +  "': ARINC 665-3/4/5" };

    // Media Set XML file
    std::filesystem::path mediaSetXmlFile;
    // Media Set source directory
    std::filesystem::path mediaSetSourceDirectory;
    // Media Set destination directory
    std::filesystem::path mediaSetDestinationDirectory;
    // Create batch file policy
    Arinc665::Utils::FileCreationPolicy createBatchFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    // Create Load Header file policy
    Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    // ARINC 665 Version
    Arinc665::SupportedArinc665Version version{
      Arinc665::SupportedArinc665Version::Supplement2 };

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
      "ARINC 665 media set description XML"
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

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription ),
      vm );

    if ( 0U != vm.count( "help" ) )
    {
      std::cout
        << "Compiles an ARINC 665 Media Set\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    // load ARINC 665 XML file
    auto [ mediaSet, fileMapping ] =
      Arinc665::Utils::Arinc665Xml_load( mediaSetXmlFile );

    // Add Media Set Part Number to Output Path
    mediaSetDestinationDirectory /= mediaSet->partNumber();

    if ( std::filesystem::exists( mediaSetDestinationDirectory ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set Directory already exist" } );
    }

    // create media set directory
    std::filesystem::create_directories( mediaSetDestinationDirectory );

    auto exporter{ Arinc665::Utils::MediaSetExporter::create() };

    // set exporter parameters
    exporter->mediaSet( mediaSet )
      .createMediumHandler( std::bind_front(
        &createMedium,
        mediaSetDestinationDirectory ) )
      .createDirectoryHandler( std::bind_front(
        &createDirectory,
        mediaSetDestinationDirectory ) )
      .checkFileExistenceHandler( std::bind_front(
        &checkFileExistence,
        mediaSetSourceDirectory,
        fileMapping ) )
      .createFileHandler( std::bind_front(
        &createFile,
        mediaSetSourceDirectory,
        mediaSetDestinationDirectory,
        fileMapping ) )
      .writeFileHandler( std::bind_front(
        &writeFile,
        mediaSetDestinationDirectory ) )
      .readFileHandler( std::bind_front(
        &readFile,
        mediaSetDestinationDirectory ) )
      .arinc665Version( version )
      .createBatchFiles( createBatchFiles )
      .createLoadHeaderFiles( createLoadHeaderFiles );

    (*exporter)();
  }
  catch ( const boost::program_options::error &e )
  {
    std::cerr
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description\n";
    return EXIT_FAILURE;
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    std::cerr
      << "Arinc665Exception in compiler: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << "Error: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cerr
      << "Error: "
      << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "unknown exception occurred\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static std::filesystem::path mediumPath(
  const std::filesystem::path &base,
  const Arinc665::MediumNumber &mediumNumber )
{
  return base / fmt::format( "MEDIUM_{:03d}", static_cast< uint8_t >( mediumNumber ) );
}

static void createMedium(
  const std::filesystem::path &base,
  const Arinc665::MediumNumber &mediumNumber )
{
  BOOST_LOG_FUNCTION()

  auto mPath{ mediumPath( base, mediumNumber ) };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Create medium directory " << mPath;

  std::filesystem::create_directory( mPath );
}

static void createDirectory(
  const std::filesystem::path &mediaSetBase,
  const Arinc665::MediumNumber &mediumNumber,
  const Arinc665::Media::ConstDirectoryPtr &directory )
{
  BOOST_LOG_FUNCTION()

  auto directoryPath{
    mediumPath( mediaSetBase, mediumNumber )
      / directory->path().relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Create directory " << "[" << mediumNumber << "] " << directoryPath;

  std::filesystem::create_directory( directoryPath );
}

static bool checkFileExistence(
  const std::filesystem::path &sourceBase,
  const Arinc665::Utils::FilePathMapping &filePathMapping,
  const Arinc665::Media::ConstFilePtr &file )
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
  const Arinc665::Utils::FilePathMapping &filePathMapping,
  const Arinc665::Media::ConstFilePtr &file )
{
  BOOST_LOG_FUNCTION()

  // search for file
  auto fileIt{ filePathMapping.find( file ) };

  if ( fileIt == filePathMapping.end() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "file mapping not found" }
      << boost::errinfo_file_name{ std::string{ file->name() } } );
  }

  auto filePath{
    mediumPath( mediaSetBase, file->effectiveMediumNumber() )
      / file->path().relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Copy file " << filePath;

  // copy file
  std::filesystem::copy( sourceBase / fileIt->second, filePath );
}

static void writeFile(
  const std::filesystem::path &mediaSetBase,
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path,
  const Arinc665::Files::ConstRawFileSpan &file )
{
  BOOST_LOG_FUNCTION()

  auto filePath{
    mediumPath( mediaSetBase, mediumNumber) / path.relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Write file " << "[" << mediumNumber << "] " << filePath;

  // check existence of file
  if ( std::filesystem::exists( filePath) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File already exists" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // save file
  std::ofstream fileStream(
    filePath.string(),
    std::ofstream::binary | std::ofstream::out | std::ofstream::trunc );

  if ( !fileStream.is_open() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // write the data to the buffer
  fileStream.write(
    (const char*) file.data(),
    static_cast< std::streamsize >( file.size() ) );
}

static Arinc665::Files::RawFile readFile(
  const std::filesystem::path &mediaSetBase,
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path )
{
  BOOST_LOG_FUNCTION()

  // check medium number
  auto filePath{
    mediumPath( mediaSetBase, mediumNumber ) / path.relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Read file " << "[" << mediumNumber << "] " << filePath;

  // check existence of file
  if ( !std::filesystem::is_regular_file( filePath ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << boost::errinfo_file_name{ filePath.string() }
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  Arinc665::Files::RawFile data( std::filesystem::file_size( filePath ) );

  // load file
  std::ifstream file{
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in };

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // read the data to the buffer
  file.read(
    (char *) &data.at( 0 ),
    static_cast< std::streamsize >( data.size() ) );

  // return the buffer
  return data;
}
