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
    << "Media Set " << mediaSetV->partNumber();

  for ( const auto &[ mediumNumber, medium ] : mediaSetV->media() )
  {
    exportMedium( medium );
  }
}

void MediaSetExporterImpl::exportMedium( const Media::ConstMediumPtr &medium )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Medium #" << (unsigned int)medium->mediumNumber();

  // create the medium (i.e. create directory)
  createMediumHandlerV( medium );

  // export sub-directories
  for ( const auto &directory : medium->subdirectories() )
  {
    exportDirectory( directory );
  }

  // export files
  for ( const auto &file : medium->files() )
  {
    exportFile( file );
  }

  // export list of loads
  exportListOfLoads( medium );

  // export "list of batches" file (if present)
  if ( medium->mediaSet()->numberOfBatches() != 0U )
  {
    exportListOfBatches( medium );
  }

  // export "list of files"
  exportListOfFiles( medium );
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

  // export sub-directories
  for ( const auto &subDirectory : directory->subdirectories() )
  {
    exportDirectory( subDirectory );
  }

  // export files
  for ( const auto &file : directory->files() )
  {
    exportFile( file );
  }
}

void MediaSetExporterImpl::exportFile( const Media::ConstFilePtr &file ) const
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export File to ["
    << (unsigned int)file->medium()->mediumNumber()
    << "]:"
    << file->path();

  switch (file->fileType())
  {
    case Media::RegularFile::FileType::RegularFile:
      createFileHandlerV( file );
      break;

    case Media::RegularFile::FileType::LoadFile:
      switch ( createLoadHeaderFilesV )
      {
        case FileCreationPolicy::None:
          createFileHandlerV( file );
          break;

        case FileCreationPolicy::NoneExisting:
          if ( checkFileExistenceHandlerV( file ) )
          {
            createFileHandlerV( file );
          }
          else
          {
            createLoadHeaderFile( file );
          }
          break;

        case FileCreationPolicy::All:
          createLoadHeaderFile( file );
          break;

        default:
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "Invalid value of createLoadHeaderFiles" } );
      }
      break;

    case Media::RegularFile::FileType::BatchFile:
      switch ( createBatchFilesV )
      {
        case FileCreationPolicy::None:
          createFileHandlerV( file );
          break;

        case FileCreationPolicy::NoneExisting:
          if ( checkFileExistenceHandlerV( file ) )
          {
            createFileHandlerV( file );
          }
          else
          {
            createBatchFile( file );
          }
          break;

        case FileCreationPolicy::All:
          createBatchFile( file );
          break;

        default:
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "Invalid value of createBatchFiles" } );
      }
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid file type" } );
        /* no break: because THROW */
  }
}

void MediaSetExporterImpl::exportListOfLoads(
  const Media::ConstMediumPtr &medium ) const
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Loads";

  Arinc665::Files::LoadListFile loadListFile{ arinc665VersionV };

  loadListFile.mediaSequenceNumber( medium->mediumNumber() );
  loadListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  loadListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  /* add all loads to "list of loads" file */
  for ( const auto &load : medium->mediaSet()->loads() )
  {
    loadListFile.load( Files::LoadInfo{
      std::string{ load->partNumber() },
      std::string{ load->name() },
      load->medium()->mediumNumber(),
      load->targetHardwareIds() } );
  }

  loadListFile.userDefinedData( mediaSetV->loadsUserDefinedData() );

  writeFileHandlerV(
    medium->mediumNumber(),
    "/" + std::string{ ListOfLoadsName },
    static_cast< Files::RawFile >( loadListFile ) );
}

void MediaSetExporterImpl::exportListOfBatches(
  const Media::ConstMediumPtr &medium ) const
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Batches";

  Arinc665::Files::BatchListFile batchListFile{ arinc665VersionV };
  batchListFile.mediaSequenceNumber( medium->mediumNumber() );
  batchListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  batchListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  /* add all batches to batches list */
  for ( const auto &batch : medium->mediaSet()->batches() )
  {
    batchListFile.batch( Files::BatchInfo{
      std::string{ batch->partNumber() },
      std::string{ batch->name() },
      batch->medium()->mediumNumber() } );
  }

  batchListFile.userDefinedData( mediaSetV->batchesUserDefinedData() );

  writeFileHandlerV(
    medium->mediumNumber(),
    "/" + std::string{ ListOfBatchesName },
    static_cast< Files::RawFile >( batchListFile ) );
}

void MediaSetExporterImpl::exportListOfFiles(
  const Media::ConstMediumPtr &medium ) const
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Files";

  Arinc665::Files::FileListFile fileListFile{ arinc665VersionV };
  fileListFile.mediaSequenceNumber( medium->mediumNumber() );
  fileListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  fileListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  // add list of loads
  const auto listOfLoadsCheckValueType{
    medium->mediaSet()->listOfLoadsCheckValueType( true ) };

  const auto&[  listOfLoadsCrc, listOfLoadsCheckValue ]{ fileCrcCheckValue(
    medium,
    listOfLoadsCheckValueType,
    "/" + std::string{ ListOfLoadsName } ) };

  fileListFile.file( {
    std::string{ ListOfLoadsName },
    Files::Arinc665File::encodePath( "/" ),
    medium->mediumNumber(),
    listOfLoadsCrc,
    listOfLoadsCheckValue } );

  // add list of batches - if present
  if ( medium->mediaSet()->numberOfBatches() != 0 )
  {
    const auto listOfBatchesCheckValueType{
      medium->mediaSet()->listOfBatchesCheckValueType( true ) };

    const auto&[ listOfBatchesFileCrc, listOfBatchesFileCheckValue ]{ fileCrcCheckValue(
      medium,
      listOfBatchesCheckValueType,
      "/" + std::string{ ListOfBatchesName } ) };

    fileListFile.file( {
      std::string{ ListOfBatchesName },
      Files::Arinc665File::encodePath( "/" ),
      medium->mediumNumber(),
      listOfBatchesFileCrc,
      listOfBatchesFileCheckValue } );
  }

  /* add all files, load header files, and batch files to file list */
  for ( const auto &file : medium->mediaSet()->files() )
  {
    const auto fileCheckValueType{ file->checkValueType( true ) };

    const auto&[ fileCrc, fileCheckValue ]{ fileCrcCheckValue(
      medium,
      fileCheckValueType,
      file->path() ) };

    fileListFile.file( {
      std::string{ file->name() },
      Files::Arinc665File::encodePath( file->path().parent_path() ),
      file->medium()->mediumNumber(),
      fileCrc,
      fileCheckValue } );
  }

  fileListFile.userDefinedData( mediaSetV->filesUserDefinedData() );
  fileListFile.checkValueType(
    medium->mediaSet()->listOfFilesCheckValueType( true ) );

  writeFileHandlerV(
    medium->mediumNumber(),
    "/" + std::string{ ListOfFilesName },
    static_cast< Files::RawFile >( fileListFile ) );
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
  loadHeaderFile.targetHardwareIdPositions( load->targetHardwareIdPositions() );
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

  // calculate load CRC and Check Value
  Arinc645::Arinc645Crc32 loadCrc{};
  const auto loadCheckValueType{ load->loadCheckValueType( true ) };
  auto checkValueGenerator{
    loadCheckValueType ?
      Arinc645::CheckValueGenerator::create( *loadCheckValueType ) :
      Arinc645::CheckValueGeneratorPtr{} };

  // load header load CRC calculation
  const auto rawLoadHeader{ Files::RawFile( loadHeaderFile ) };

  loadCrc.process_bytes(
    std::data( rawLoadHeader ),
    rawLoadHeader.size() - sizeof( uint32_t ) );

  if ( checkValueGenerator )
  {
    checkValueGenerator->process( rawLoadHeader );
  }

  // load data files for load CRC.
  for ( const auto &[ file, partNumber, checkValueType ] : load->dataFiles() )
  {
    auto rawDataFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    loadCrc.process_bytes(
      std::to_address( rawDataFile.begin() ),
      rawDataFile.size() );

    if ( checkValueGenerator )
    {
      checkValueGenerator->process( rawDataFile );
    }
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

    if ( checkValueGenerator )
    {
      checkValueGenerator->process( rawSupportFile );
    }
  }

  // set load CRC + Check Value
  loadHeaderFile.loadCrc( loadCrc.checksum() );
  if ( checkValueGenerator )
  {
    loadHeaderFile.loadCheckValue( checkValueGenerator->checkValue() );
  }

  writeFileHandlerV(
    load->medium()->mediumNumber(),
    load->path(),
    static_cast< Files::RawFile >( loadHeaderFile ) );
}

Files::LoadFileInfo MediaSetExporterImpl::loadFileInformation(
  const Media::ConstLoadFile &loadFile ) const
{
  const auto &[ file, partNumber, checkValueType ] = loadFile;

  // read file
  const auto rawDataFile{ readFileHandlerV(
    file->medium()->mediumNumber(),
    file->path() ) };

  // Calculate CRC 16
  const auto fileCrc{
    Files::Arinc665File::calculateChecksum( rawDataFile ) };

  // Calculate Check Value if required
  Arinc645::CheckValue checkValue{};

  if ( checkValueType )
  {
    checkValue = { Arinc645::CheckValueGenerator::checkValue(
      *checkValueType,
      rawDataFile ) };
  }

  return Files::LoadFileInfo{
    std::string{ file->name() },
    partNumber,
    rawDataFile.size(),
    fileCrc,
    std::move( checkValue ) };
}

void MediaSetExporterImpl::createBatchFile(
  const Media::ConstFilePtr &file ) const
{
  // up-cast batch file
  auto batch{ std::dynamic_pointer_cast< const Media::Batch>( file ) };

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
        std::string{ load->name() },
        std::string{ load->partNumber() } } );
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

std::tuple< uint16_t, Arinc645::CheckValue > MediaSetExporterImpl::fileCrcCheckValue(
  const Media::ConstMediumPtr &medium,
  std::optional< Arinc645::CheckValueType > checkValueType,
  const std::filesystem::path &filename ) const
{
  auto checkValueGenerator{ checkValueType ?
    Arinc645::CheckValueGenerator::create( *checkValueType ) :
    Arinc645::CheckValueGeneratorPtr{} };

  const auto rawFile{
    readFileHandlerV(
      medium->mediumNumber(),
      filename ) };

  const auto crc{ Files::Arinc665File::calculateChecksum( rawFile ) };

  Arinc645::CheckValue checkValue{ Arinc645::CheckValueType::NotUsed, {} };
  if ( checkValueGenerator )
  {
    checkValueGenerator->process( rawFile );
    checkValue = checkValueGenerator->checkValue();
  }

  return { crc, checkValue };
}

}
