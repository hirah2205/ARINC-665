/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetExporterImpl.
 **/

#include "MediaSetExporterImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/files/LoadListFile.hpp>
#include <arinc665/files/BatchListFile.hpp>
#include <arinc665/files/FileListFile.hpp>
#include <arinc665/files/LoadHeaderFile.hpp>
#include <arinc665/files/BatchFile.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/Arinc645Crc.hpp>
#include <arinc645/CheckValueGenerator.hpp>

#include <utility>

namespace Arinc665::Utils {

MediaSetExporter& MediaSetExporterImpl::mediaSet(
  Media::ConstMediaSetPtr mediaSet )
{
  mediaSetV = std::move( mediaSet );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createMediumHandler(
  CreateMediumHandler createMediumHandler )
{
  createMediumHandlerV = std::move( createMediumHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createDirectoryHandler(
  CreateDirectoryHandler createDirectoryHandler )
{
  createDirectoryHandlerV = std::move( createDirectoryHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::checkFileExistenceHandler(
  CheckFileExistenceHandler checkFileExistenceHandler )
{
  checkFileExistenceHandlerV = std::move( checkFileExistenceHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createFileHandler(
  CreateFileHandler createFileHandler )
{
  createFileHandlerV = std::move( createFileHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::writeFileHandler(
  WriteFileHandler writeFileHandler )
{
  writeFileHandlerV = std::move( writeFileHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::readFileHandler(
  ReadFileHandler readFileHandler )
{
  readFileHandlerV = std::move( readFileHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::arinc665Version(
  const SupportedArinc665Version version )
{
  arinc665VersionV = version;
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createBatchFiles(
  const FileCreationPolicy createBatchFiles )
{
  createBatchFilesV = createBatchFiles ;
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createLoadHeaderFiles(
  const FileCreationPolicy createLoadHeaderFiles )
{
  createLoadHeaderFilesV = createLoadHeaderFiles;
  return *this;
}

void MediaSetExporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Media Set " << mediaSetV->partNumber();

  // first export medium (directories and regular files)
  for ( const auto &[mediumNumber, medium] : mediaSetV->media() )
  {
    // create the medium (i.e. create directory)
    createMediumHandlerV( medium );

    // export regular files
    for ( const auto &file : medium->regularFiles() )
    {
      exportRegularFile( file );
    }

    // export sub-directories
    for ( const auto &directory : medium->subdirectories() )
    {
      exportDirectory( directory );
    }
  }

  // export load headers
  for ( const auto &load : mediaSetV->loads() )
  {
    exportLoad( load );
  }

  // export batch files
  for ( const auto &batch : mediaSetV->batches() )
  {
    exportBatch( batch );
  }

  // export list of loads for all media
  exportListOfLoads();

  // export "list of batches" file (if present) for all media
  if ( mediaSetV->numberOfBatches() != 0U )
  {
    exportListOfBatches();
  }

  // export "list of files" for all media
  exportListOfFiles();
}

void MediaSetExporterImpl::exportDirectory(
  const Media::ConstDirectoryPtr &directory )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Directory to ["
    << (unsigned int)directory->medium()->mediumNumber()
    << "]:"
    << directory->path();

  createDirectoryHandlerV( directory );

  // export regular files
  for ( const auto &file : directory->regularFiles() )
  {
    exportRegularFile( file );
  }

  // export sub-directories
  for ( const auto &subDirectory : directory->subdirectories() )
  {
    exportDirectory( subDirectory );
  }
}

void MediaSetExporterImpl::exportRegularFile(
  const Media::ConstRegularFilePtr &file )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Regular File to ["
    << (unsigned int)file->medium()->mediumNumber()
    << "]:"
    << file->path();

  // regular file mus be created by callback
  createFileHandlerV( file );
}

void MediaSetExporterImpl::exportLoad( const Media::ConstLoadPtr &load )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Load to ["
    << (unsigned int)load->medium()->mediumNumber()
    << "]:"
    << load->path();

  // check load header creation policy
  switch ( createLoadHeaderFilesV )
  {
    case FileCreationPolicy::None:
      createFileHandlerV( load );
      break;

    case FileCreationPolicy::NoneExisting:
      if ( checkFileExistenceHandlerV( load ) )
      {
        createFileHandlerV( load );
      }
      else
      {
        createLoadHeaderFile( load );
      }
      break;

    case FileCreationPolicy::All:
      createLoadHeaderFile( load );
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid value of createLoadHeaderFiles" } );
  }
}

void MediaSetExporterImpl::exportBatch( const Media::ConstBatchPtr &batch )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Batch to ["
    << (unsigned int)batch->medium()->mediumNumber()
    << "]:"
    << batch->path();

  // check batch file creation policy
  switch ( createBatchFilesV )
  {
    case FileCreationPolicy::None:
      createFileHandlerV( batch );
      break;

    case FileCreationPolicy::NoneExisting:
      // check batch file creation policy
      if ( checkFileExistenceHandlerV( batch ) )
      {
        createFileHandlerV( batch );
      }
      else
      {
        createBatchFile( batch );
      }
      break;

    case FileCreationPolicy::All:
      createBatchFile( batch );
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid value of createBatchFiles" } );
  }
}

void MediaSetExporterImpl::exportListOfLoads() const
{
  BOOST_LOG_FUNCTION()

  Arinc665::Files::LoadListFile loadListFile{ arinc665VersionV };

  loadListFile.mediaSetPn( mediaSetV->partNumber() );
  loadListFile.numberOfMediaSetMembers( mediaSetV->numberOfMedia() );

  /* add all loads to "list of loads" file */
  for ( const auto &load : mediaSetV->loads() )
  {
    loadListFile.load( Files::LoadInfo{
      std::string{ load->partNumber() },
      std::string{ load->name() },
      load->medium()->mediumNumber(),
      load->targetHardwareIds() } );
  }

  loadListFile.userDefinedData( mediaSetV->loadsUserDefinedData() );

  for ( const auto &[mediumNumber, medium] : mediaSetV->media() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "Export List of Loads to ["
      << (unsigned int)mediumNumber
      << "]";

    loadListFile.mediaSequenceNumber( mediumNumber );

    writeFileHandlerV(
      mediumNumber,
      "/" + std::string{ ListOfLoadsName },
      static_cast< Files::RawFile >( loadListFile ) );
  }
}

void MediaSetExporterImpl::exportListOfBatches() const
{
  BOOST_LOG_FUNCTION()

  Arinc665::Files::BatchListFile batchListFile{ arinc665VersionV };
  batchListFile.mediaSetPn( mediaSetV->partNumber() );
  batchListFile.numberOfMediaSetMembers( mediaSetV->numberOfMedia() );

  /* add all batches to batches list */
  for ( const auto &batch : mediaSetV->batches() )
  {
    batchListFile.batch( Files::BatchInfo{
      std::string{ batch->partNumber() },
      std::string{ batch->name() },
      batch->medium()->mediumNumber() } );
  }

  batchListFile.userDefinedData( mediaSetV->batchesUserDefinedData() );

  for ( const auto &[mediumNumber, medium] : mediaSetV->media() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "Export List of Batches to [" << (unsigned int)mediumNumber
      << "]";

    batchListFile.mediaSequenceNumber( mediumNumber );

    writeFileHandlerV(
      mediumNumber,
      "/" + std::string{ ListOfBatchesName },
      static_cast< Files::RawFile >( batchListFile ) );
  }
}

void MediaSetExporterImpl::exportListOfFiles() const
{
  BOOST_LOG_FUNCTION()

  Files::FilesInfo filesInfo{};

  /* add all files, load header files, and batch files to file list */
  for ( const auto &file :mediaSetV->files() )
  {
    const auto&[ fileCrc, fileCheckValue ]{ fileCrcCheckValue(
      file->medium()->mediumNumber(),
      file->path(),
      file->effectiveCheckValueType() ) };

    filesInfo.emplace_back( Files::FileInfo{
      .filename = std::string{ file->name() },
      .pathName = Files::Arinc665File::encodePath( file->path().parent_path() ),
      .memberSequenceNumber = file->medium()->mediumNumber(),
      .crc = fileCrc,
      .checkValue = fileCheckValue } );
  }

  Arinc665::Files::FileListFile fileListFile{ arinc665VersionV };
  fileListFile.mediaSetPn( mediaSetV->partNumber() );
  fileListFile.numberOfMediaSetMembers( mediaSetV->numberOfMedia() );
  fileListFile.userDefinedData( mediaSetV->filesUserDefinedData() );
  fileListFile.checkValueType(
    mediaSetV->effectiveListOfFilesCheckValueType() );

  for ( const auto &[ mediumNumber, medium ] : mediaSetV->media() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "Export List of Files to ["
      << (unsigned int)mediumNumber
      << "]";

    fileListFile.mediaSequenceNumber( mediumNumber );

    // clear file list
    fileListFile.files().clear();

    // add list of loads
    const auto&[ listOfLoadsCrc, listOfLoadsCheckValue ]{ fileCrcCheckValue(
      medium->mediumNumber(),
      "/" + std::string{ ListOfLoadsName },
      medium->mediaSet()->effectiveListOfLoadsCheckValueType() ) };

    fileListFile.file( {
      std::string{ ListOfLoadsName },
      Files::Arinc665File::encodePath( "/" ),
      medium->mediumNumber(),
      listOfLoadsCrc,
      listOfLoadsCheckValue } );

    // add list of batches - if present
    if ( medium->mediaSet()->numberOfBatches() != 0 )
    {
      const auto&[ listOfBatchesFileCrc, listOfBatchesFileCheckValue ]{ fileCrcCheckValue(
        medium->mediumNumber(),
        "/" + std::string{ ListOfBatchesName },
        medium->mediaSet()->effectiveListOfBatchesCheckValueType() ) };

      fileListFile.file( {
        std::string{ ListOfBatchesName },
        Files::Arinc665File::encodePath( "/" ),
        medium->mediumNumber(),
        listOfBatchesFileCrc,
        listOfBatchesFileCheckValue } );
    }

    // add Files
    fileListFile.files().insert(
      fileListFile.files().end(),
      filesInfo.begin(),
      filesInfo.end() );

    writeFileHandlerV(
      mediumNumber,
      "/" + std::string{ ListOfFilesName },
      static_cast< Files::RawFile >( fileListFile ) );
  }
}

void MediaSetExporterImpl::createLoadHeaderFile(
  const Media::ConstFilePtr &loadFile ) const
{
  // up-cast load file
  auto load{ std::dynamic_pointer_cast< const Media::Load>( loadFile ) };

  if ( !load )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Cannot cast file to load" } );
  }

  Files::LoadHeaderFile loadHeaderFile{ arinc665VersionV };
  loadHeaderFile.partFlags( load->partFlags() );
  loadHeaderFile.partNumber( load->partNumber() );
  for ( const auto &[ thwId, positions ] : load->targetHardwareIdPositions() )
  {
    loadHeaderFile.targetHardwareId( thwId );
    loadHeaderFile.targetHardwareIdPositions(
      thwId,
      Files::LoadHeaderFile::Positions{ positions.begin(), positions.end() } );
  }
  loadHeaderFile.loadType( load->loadType() );

  // Process data files and add info to load header.
  for ( const auto &file : load->dataFiles( true ) )
  {
    loadHeaderFile.dataFile( loadFileInformation( file ) );
  }

  // Process support files and add info to load header.
  for ( const auto &file: load->supportFiles( true ) )
  {
    loadHeaderFile.supportFile( loadFileInformation( file ) );
  }

  // User Defined Data
  loadHeaderFile.userDefinedData( load->userDefinedData() );

  // Set Check Value Type before Raw File generation (for Space Reserving)
  loadHeaderFile.loadCheckValueType( load->effectiveLoadCheckValueType() );

  // RAW load header used for load Check Value and CRC calculation
  auto rawLoadHeader{ Files::RawFile( loadHeaderFile ) };

  // Calculate Load Check Value ( Only for supported version)
  if ( SupportedArinc665Version::Supplement345 == arinc665VersionV )
  {
    auto checkValueGenerator{
      Arinc645::CheckValueGenerator::create( load->effectiveLoadCheckValueType() ) };
    assert( checkValueGenerator );

    Files::LoadHeaderFile::processLoadCheckValue(
      rawLoadHeader,
      *checkValueGenerator );

    // load data files for Load Check Value.
    for ( const auto &[ file, partNumber, checkValueType ] : load->dataFiles() )
    {
      auto rawDataFile{ readFileHandlerV(
        file->medium()->mediumNumber(),
        file->path() ) };

      checkValueGenerator->process( rawDataFile );
    }

    // load support files for Load Check Value.
    for ( const auto &[ file, partNumber, checkValueType ] : load->supportFiles() )
    {
      auto rawSupportFile{ readFileHandlerV(
        file->medium()->mediumNumber(),
        file->path() ) };

      checkValueGenerator->process( rawSupportFile );
    }

    Files::LoadHeaderFile::encodeLoadCheckValue(
      rawLoadHeader,
      checkValueGenerator->checkValue() );
  }


  // Calculate load CRC
  Arinc645::Arinc645Crc32 loadCrc{};

  Files::LoadHeaderFile::processLoadCrc( rawLoadHeader, loadCrc );

  // load data files for load CRC.
  for ( const auto &[ file, partNumber, checkValueType ] : load->dataFiles() )
  {
    auto rawDataFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    loadCrc.process_bytes(
      std::to_address( rawDataFile.begin() ),
      rawDataFile.size() );
  }

  // load support files for load CRC.
  for ( const auto &[ file, partNumber, checkValueType ] : load->supportFiles() )
  {
    auto rawSupportFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    loadCrc.process_bytes(
      std::to_address( rawSupportFile.begin() ),
      rawSupportFile.size() );
  }

  // set load CRC
  Files::LoadHeaderFile::encodeLoadCrc( rawLoadHeader, loadCrc.checksum() );

  // Write Load Header File
  writeFileHandlerV(
    load->medium()->mediumNumber(),
    load->path(),
    rawLoadHeader );
}

Files::LoadFileInfo MediaSetExporterImpl::loadFileInformation(
  const Media::ConstLoadFile &loadFile ) const
{
  const auto &[ file, partNumber, checkValueType ] = loadFile;

  // read file
  const auto rawDataFile{ readFileHandlerV(
    file->medium()->mediumNumber(),
    file->path() ) };

  return Files::LoadFileInfo{
    .filename = std::string{ file->name() },
    .partNumber = partNumber,
    .length = rawDataFile.size(),
    .crc = Files::Arinc665File::calculateChecksum( rawDataFile ),
    .checkValue = Arinc645::CheckValueGenerator::checkValue(
      checkValueType.value_or( Arinc645::CheckValueType::NotUsed ),
      rawDataFile ) };
}

void MediaSetExporterImpl::createBatchFile(
  const Media::ConstFilePtr &file ) const
{
  // up-cast batch file
  const auto batch{ std::dynamic_pointer_cast< const Media::Batch>( file ) };

  if ( !batch )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Cannot cast file to batch" } );
  }

  Files::BatchFile batchFile{ arinc665VersionV };
  batchFile.partNumber( batch->partNumber() );
  batchFile.comment( batch->comment() );

  // iterate over targets
  for ( const auto &[ targetHwId, loads ] : batch->targets() )
  {
    Files::BatchLoadsInfo batchLoadsInfo{};

    // iterate over loads
    for ( const auto &load : loads )
    {
      // add load to target batch information
      batchLoadsInfo.emplace_back( Files::BatchLoadInfo{
        .headerFilename = std::string{ load->name() },
        .partNumber = std::string{ load->partNumber() } } );
    }

    // add target
    batchFile.targetHardware(
      Files::BatchTargetInfo{ targetHwId, std::move( batchLoadsInfo ) } );
  }

  writeFileHandlerV(
    batch->medium()->mediumNumber(),
    batch->path(),
    static_cast< Files::RawFile >( batchFile ) );
}

std::tuple< uint16_t, Arinc645::CheckValue >
MediaSetExporterImpl::fileCrcCheckValue(
  const uint8_t mediumNumber,
  const std::filesystem::path &filename,
  const Arinc645::CheckValueType checkValueType ) const
{
  auto checkValueGenerator{
    Arinc645::CheckValueGenerator::create( checkValueType ) };
  assert( checkValueGenerator );

  const auto rawFile{ readFileHandlerV( mediumNumber, filename ) };

  const auto crc{ Files::Arinc665File::calculateChecksum( rawFile ) };

  checkValueGenerator->process( rawFile );
  const auto checkValue = checkValueGenerator->checkValue();

  return { crc, checkValue };
}

}
