// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::Load.
 **/

#include "Load.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <utility>

namespace Arinc665::Media {

Load::Load(
  const ContainerEntityPtr &parent,
  std::string name,
  const OptionalMediumNumber &mediumNumber ) :
  File{ parent, std::move( name ), mediumNumber }
{
}

FileType Load::fileType() const
{
  return FileType::LoadFile;
}

uint16_t Load::partFlags() const
{
  return partFlagsV;
}

void Load::partFlags( const uint16_t partFlags )
{
  partFlagsV = partFlags;
}

std::string_view Load::partNumber() const
{
  return partNumberV;
}

void Load::partNumber( std::string partNumber )
{
  partNumberV = std::move( partNumber );
}

const Load::TargetHardwareIdPositions& Load::targetHardwareIdPositions() const
{
  return targetHardwareIdPositionsV;
}

Load::TargetHardwareIdPositions& Load::targetHardwareIdPositions()
{
  return targetHardwareIdPositionsV;
}

void Load::targetHardwareIdPositions(
  const TargetHardwareIdPositions &targetHardwareIdPositions)
{
  targetHardwareIdPositionsV = targetHardwareIdPositions;
}

void Load::targetHardwareIdPositions(
  TargetHardwareIdPositions &&targetHardwareIdPositions)
{
  targetHardwareIdPositionsV = std::move( targetHardwareIdPositions);
}

Load::TargetHardwareIds Load::targetHardwareIds() const
{
  TargetHardwareIds thwIds{};

  for ( const auto &[ thwId, positions ] : targetHardwareIdPositionsV )
  {
    thwIds.emplace( thwId );
  }

  return thwIds;
}

void Load::targetHardwareIds( const TargetHardwareIds &thwIds )
{
  for ( const auto &targetHardwareId : thwIds )
  {
    targetHardwareIdPositionsV.insert_or_assign( targetHardwareId, Positions{} );
  }
}

void Load::targetHardwareId( std::string targetHardwareId, Positions positions )
{
  targetHardwareIdPositionsV.insert_or_assign(
    std::move( targetHardwareId ),
    std::move( positions ) );
}

ConstRegularFilePtr Load::file(
  std::string_view filename,
  std::string_view partNumber ) const
{
  BOOST_LOG_FUNCTION()

  ConstRegularFiles files{};

  for ( const auto &[ file, filePartNumber, checkValueType ] : dataFilesV )
  {
    if ( auto regularFile{ file.lock() };
      regularFile && ( regularFile->name() == filename )
      && ( partNumber.empty() || ( partNumber == filePartNumber ) ) )
    {
      files.emplace_back( regularFile );
    }
  }

  for ( const auto &[ file, filePartNumber, checkValueType ] : supportFilesV )
  {
    if ( auto regularFile{ file.lock() };
         regularFile && ( regularFile->name() == filename )
         && ( partNumber.empty() || ( partNumber == filePartNumber ) ) )
    {
      files.emplace_back( regularFile );
    }
  }

  if ( 1U == files.size() )
  {
    return files.front();
  }

  if ( !files.empty() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "More loads found for given parameters";
  }

  return {};
}

ConstLoadFiles Load::dataFiles( const bool effective ) const
{
  BOOST_LOG_FUNCTION()

  ConstLoadFiles files{};

  for ( const auto &[ filePtr, partNumber, checkValueType ] : dataFilesV )
  {
    if ( effective )
    {
      files.emplace_back(
        filePtr.lock(),
        partNumber,
        checkValueType.value_or( effectiveDataFilesCheckValueType() ) );
    }
    else
    {
      files.emplace_back( filePtr.lock(), partNumber, checkValueType );
    }
  }

  return files;
}

void Load::dataFiles( const ConstLoadFiles &files )
{
  dataFilesV.assign( files.begin(), files.end() );
}

void Load::dataFile(
  const ConstRegularFilePtr &file,
  std::string partNumber,
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  BOOST_LOG_FUNCTION()

  if ( !file || ( file->mediaSet() != mediaSet() ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "invalid File" } );
  }

  dataFilesV.emplace_back(
    file,
    std::move( partNumber ),
    checkValueType );
}

ConstLoadFiles Load::supportFiles( const bool effective ) const
{
  BOOST_LOG_FUNCTION()

  ConstLoadFiles files{};

  for ( const auto &[filePtr, partNumber, checkValueType ] : supportFilesV )
  {
    if ( effective )
    {
      files.emplace_back(
        filePtr.lock(),
        partNumber,
        checkValueType.value_or( effectiveSupportFilesCheckValueType() ) );
    }
    else
    {
      files.emplace_back( filePtr.lock(), partNumber, checkValueType );
    }
  }

  return files;
}

void Load::supportFiles( const ConstLoadFiles &files )
{
  supportFilesV.assign( files.begin(), files.end() );
}

void Load::supportFile(
  const ConstRegularFilePtr &file,
  std::string partNumber,
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  BOOST_LOG_FUNCTION()

  if ( !file || ( file->mediaSet() != mediaSet() ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "invalid File" } );
  }

  supportFilesV.emplace_back(
    file,
    std::move( partNumber ),
    checkValueType );
}

ConstUserDefinedDataSpan Load::userDefinedData() const
{
  return userDefinedDataV;
}

UserDefinedData& Load::userDefinedData()
{
  return userDefinedDataV;
}

void Load::userDefinedData( UserDefinedData userDefinedData )
{
  userDefinedDataV = std::move( userDefinedData ) ;
}

const Load::Type& Load::loadType() const
{
  return typeV;
}

void Load::loadType( Type type )
{
  typeV = std::move( type );
}

Arinc645::CheckValueType Load::effectiveLoadCheckValueType() const
{
  return loadCheckValueTypeV.value_or(
    mediaSet()->mediaSetCheckValueType().value_or(
      Arinc645::CheckValueType::NotUsed ) );
}

std::optional< Arinc645::CheckValueType > Load::loadCheckValueType() const
{
  return loadCheckValueTypeV;
}

void Load::loadCheckValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  loadCheckValueTypeV = checkValueType;
}

Arinc645::CheckValueType Load::effectiveDataFilesCheckValueType() const
{
  return dataFilesCheckValueTypeV.value_or(
    mediaSet()->effectiveMediaSetCheckValueType() );
}

std::optional< Arinc645::CheckValueType > Load::dataFilesCheckValueType() const
{
  return dataFilesCheckValueTypeV;
}

void Load::dataFilesCheckValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  dataFilesCheckValueTypeV = checkValueType;
}

Arinc645::CheckValueType Load::effectiveSupportFilesCheckValueType() const
{
  return supportFilesCheckValueTypeV.value_or(
    mediaSet()->effectiveMediaSetCheckValueType() );
}

std::optional< Arinc645::CheckValueType > Load::supportFilesCheckValueType() const
{
  return supportFilesCheckValueTypeV;
}

void Load::supportFilesCheckValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  supportFilesCheckValueTypeV = checkValueType;
}

ConstFilePtr Load_file(
  const ConstLoadPtr &load,
  std::string_view filename,
  std::string_view partNumber )
{
  BOOST_LOG_FUNCTION()

  if ( !load )
  {
    return {};
  }

  // check if requested file is the load header file itself
  if ( ( filename == load->name() )
    && ( partNumber.empty() || ( partNumber == load->partNumber() ) ) )
  {
    return load;
  }

  return load->file( filename, partNumber );
}

ConstFilePtr Load_file(
  const ConstLoadPtr &load,
  const CheckValues &checkValues,
  std::string_view filename,
  const Arinc645::CheckValue &checkValue )
{
  BOOST_LOG_FUNCTION()

  if ( !load )
  {
    return {};
  }

  ConstRegularFiles files{};

  for ( const auto &[file, filePartNumber, checkValueType] : load->dataFiles() )
  {
    if ( file && ( file->name() == filename ) )
    {
      if (
        auto fileCheckValues{ checkValues.find( file ) };
        ( fileCheckValues != checkValues.end() ) &&
        fileCheckValues->second.contains( checkValue ) )
      {
        files.emplace_back( file );
      }
    }
  }

  for ( const auto &[file, filePartNumber, checkValueType] : load->supportFiles() )
  {
    if ( file && ( file->name() == filename ) )
    {
      auto fileCheckValues{ checkValues.find( file ) };
      if ( fileCheckValues->second.contains( checkValue ) )
      {
        files.emplace_back( file );
      }
    }
  }

  if ( 1U == files.size() )
  {
    return files.front();
  }

  if ( !files.empty() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "More files found for given parameters";
  }

  return {};
}

ConstFilePtr Loads_file(
  const ConstLoads &loads,
  std::string_view filename,
  std::string_view partNumber )
{
  BOOST_LOG_FUNCTION()

  ConstFiles files{};

  for ( const auto &load : loads )
  {
    assert( load );
    if ( auto file{ Load_file( load, filename, partNumber ) }; file )
    {
      files.emplace_back( std::move( file ) );
    }
  }

  if ( 1U == files.size() )
  {
    return files.front();
  }

  if ( !files.empty() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "More files found for given parameters";
  }

  return {};
}

ConstFilePtr Loads_file(
  const ConstLoads &loads,
  const CheckValues &checkValues,
  std::string_view filename,
  const Arinc645::CheckValue &checkValue )
{
  BOOST_LOG_FUNCTION()

  ConstFiles files{};

  for ( const auto &load : loads )
  {
    assert( load );
    if ( auto file{ Load_file( load, checkValues, filename, checkValue ) }; file )
    {
      files.emplace_back( std::move( file ) );
    }
  }

  if ( 1U == files.size() )
  {
    return files.front();
  }

  if ( !files.empty() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "More files found for given parameters";
  }

  // no, or more than one file
  return {};
}

}
