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

MediaSetExporterImpl::MediaSetExporterImpl():
  arinc665VersionV{ SupportedArinc665Version::Supplement2 },
  createBatchFilesV{ FileCreationPolicy::None },
  createLoadHeaderFilesV{ FileCreationPolicy::None }
{
}

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
  SupportedArinc665Version arinc665Version )
{
  arinc665VersionV = std::move( arinc665Version );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createBatchFiles(
  FileCreationPolicy createBatchFiles )
{
  createBatchFilesV = createBatchFiles ;
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createLoadHeaderFiles(
  FileCreationPolicy createLoadHeaderFiles )
{
  createLoadHeaderFilesV = createLoadHeaderFiles;
  return *this;
}

void MediaSetExporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Media Set " << mediaSetV->partNumber();

  for ( const auto &[mediumNumber, medium] : mediaSetV->media() )
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
  for ( const auto &directory : medium->subDirectories() )
  {
    exportDirectory( directory );
  }

  // export files
  for ( const auto &file : medium->files() )
  {
    exportFile( file );
  }

  // export list of loads
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Loads";

  Arinc665::Files::LoadListFile loadListFile{ arinc665VersionV };

  loadListFile.mediaSequenceNumber( medium->mediumNumber() );
  loadListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  loadListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  /* add all loads to "list of loads" file  */
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

  // export "list of batches" file (if present)
  if ( medium->mediaSet()->numberOfBatches() != 0U )
  {
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

  // export medium info
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Files";

  auto filesCheckValueType{
    medium->mediaSet()->filesCheckValueType() ?
      medium->mediaSet()->filesCheckValueType() :
      medium->mediaSet()->mediaSetCheckValueType() };

  Arinc665::Files::FileListFile fileListFile{ arinc665VersionV };
  fileListFile.mediaSequenceNumber( medium->mediumNumber() );
  fileListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  fileListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  // add list of loads
  {
    std::optional< Arinc645::CheckValueGenerator > checkValueGenerator{
      filesCheckValueType ?
        Arinc645::CheckValueGenerator{ *filesCheckValueType } :
        std::optional< Arinc645::CheckValueGenerator >{} };

    const auto rawListOfLoadsFile{
      readFileHandlerV(
        medium->mediumNumber(),
        "/" + std::string{ ListOfLoadsName } ) };

    const auto listOfLoadsFileCrc{
      Files::Arinc665File::calculateChecksum( rawListOfLoadsFile ) };

    std::optional< Arinc645::CheckValue > listOfLoadsFileCheckValue{};
    if ( checkValueGenerator )
    {
      checkValueGenerator->process( rawListOfLoadsFile );
      listOfLoadsFileCheckValue = checkValueGenerator->checkValue();
    }

    fileListFile.file({
      std::string{ ListOfLoadsName },
      Files::Arinc665File::encodePath( "/" ),
      medium->mediumNumber(),
      listOfLoadsFileCrc,
      listOfLoadsFileCheckValue } );
  }

  // add list of batches - if present
  if ( medium->mediaSet()->numberOfBatches() != 0 )
  {
    std::optional< Arinc645::CheckValueGenerator > checkValueGenerator{
      filesCheckValueType ?
        Arinc645::CheckValueGenerator{ *filesCheckValueType } :
        std::optional< Arinc645::CheckValueGenerator >{} };

    const auto rawListOfBatchesFile{
      readFileHandlerV(
        medium->mediumNumber(),
        "/" + std::string{ ListOfBatchesName } ) };

    const auto listOfBatchesFileCrc{
      Files::Arinc665File::calculateChecksum( rawListOfBatchesFile ) };

    std::optional< Arinc645::CheckValue > listOfBatchesFileCheckValue{};
    if ( checkValueGenerator )
    {
      checkValueGenerator->process( rawListOfBatchesFile );
      listOfBatchesFileCheckValue = checkValueGenerator->checkValue();
    }

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
    const auto fileCheckValueType{ file->checkValueType() ?
      file->checkValueType() : filesCheckValueType };

    std::optional< Arinc645::CheckValueGenerator > checkValueGenerator{
      fileCheckValueType ?
        Arinc645::CheckValueGenerator{ *fileCheckValueType } :
        std::optional< Arinc645::CheckValueGenerator >{} };

    const auto rawFile{ readFileHandlerV( medium->mediumNumber(), file->path() ) };

    const auto crc{ Files::Arinc665File::calculateChecksum( rawFile ) };

    std::optional< Arinc645::CheckValue > fileCheckValue{};
    if ( checkValueGenerator )
    {
      checkValueGenerator->process( rawFile );
      fileCheckValue = checkValueGenerator->checkValue();
    }

    fileListFile.file( {
      std::string{ file->name() },
      Files::Arinc665File::encodePath( file->path().parent_path() ),
      file->medium()->mediumNumber(),
      crc,
      fileCheckValue } );
  }

  fileListFile.userDefinedData( mediaSetV->filesUserDefinedData() );

  writeFileHandlerV(
    medium->mediumNumber(),
    "/" + std::string{ ListOfFilesName },
    static_cast< Files::RawFile >( fileListFile ) );
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
  for ( const auto &subDirectory : directory->subDirectories() )
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
          if (checkFileExistenceHandlerV( file ) )
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

  const auto loadCheckValue{ load->loadCheckValueType() ?
    load->loadCheckValueType() :
    load->mediaSet()->mediaSetCheckValueType() };

  // calculate data files CRC and set data.
  const auto dataFilesCheckValue{ load->dataFilesCheckValueType() ?
    load->dataFilesCheckValueType() : loadCheckValue };
  for ( const auto &[ file, partNumber, checkValueType ] : load->dataFiles() )
  {
    const auto fileCheckValueType{ checkValueType ?
      checkValueType :
      dataFilesCheckValue };

    const auto rawDataFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    const auto fileCrc{
      Files::Arinc665File::calculateChecksum( rawDataFile ) };

    Arinc645::CheckValue checkValue{};

    if ( fileCheckValueType )
    {
      const auto fileCheckValue{ Arinc645::CheckValueGenerator::checkValue(
        *fileCheckValueType,
        rawDataFile ) };
    }

    loadHeaderFile.dataFile( Files::LoadFileInfo{
      std::string{ file->name() },
      partNumber,
      rawDataFile.size(),
      fileCrc,
      std::move( checkValue ) } );
  }

  // calculate support files CRC and set data.
  const std::optional< Arinc645::CheckValueType > supportFilesCheckValue{
    load->supportFilesCheckValueType() ?
      load->supportFilesCheckValueType() : loadCheckValue };
  for ( const auto &[ file, partNumber, checkValueType ] : load->supportFiles() )
  {
    const auto fileCheckValueType{ checkValueType ?
      checkValueType :
      supportFilesCheckValue };

    const auto rawSupportFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    const auto supportFileCrc{
      Files::Arinc665File::calculateChecksum( rawSupportFile ) };

    Arinc645::CheckValue checkValue{};

    if ( fileCheckValueType )
    {
      const auto fileCheckValue{ Arinc645::CheckValueGenerator::checkValue(
        *fileCheckValueType,
        rawSupportFile ) };
    }

    loadHeaderFile.supportFile( Files::LoadFileInfo{
      std::string{ file->name() },
      partNumber,
      rawSupportFile.size(),
      supportFileCrc,
      std::move( checkValue ) } );
  }

  // User Defined Data
  loadHeaderFile.userDefinedData( load->userDefinedData() );

  // calculate load CRC and Check Value
  Arinc645::Arinc645Crc32 loadCrc{};
  std::optional< Arinc645::CheckValueGenerator > checkValueGenerator{
    loadCheckValue ?
      Arinc645::CheckValueGenerator{ *loadCheckValue } :
      std::optional< Arinc645::CheckValueGenerator >{} };

  // load header load CRC calculation
  {
    Files::RawFile rawLoadHeader{ loadHeaderFile };

    loadCrc.process_bytes(
      std::data( rawLoadHeader ),
      rawLoadHeader.size() - sizeof( uint32_t ) );

    if ( checkValueGenerator )
    {
      checkValueGenerator->process( rawLoadHeader );
    }
  }

  // load data files for load CRC.
  for ( const auto &[ file, partNumber, checkValueType ] : load->dataFiles() )
  {
    auto rawDataFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    loadCrc.process_bytes( &(*rawDataFile.begin()), rawDataFile.size() );

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

    loadCrc.process_bytes( &(*rawSupportFile.begin()), rawSupportFile.size() );

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

  for ( const auto &[ targetHwId, loads ] : batch->targets() )
  {
    Files::BatchLoadsInfo batchLoadsInfo{};

    for ( const auto &load : loads )
    {
      batchLoadsInfo.emplace_back( Files::BatchLoadInfo{
        std::string{ load->name() },
        std::string{ load->partNumber() } } );
    }

    batchFile.targetHardware(
      Files::BatchTargetInfo{ targetHwId, batchLoadsInfo } );
  }

  writeFileHandlerV(
    batch->medium()->mediumNumber(),
    batch->path(),
    static_cast< Files::RawFile >( batchFile ) );
}

}
