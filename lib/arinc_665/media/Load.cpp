// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::Load.
 **/

#include "Load.hpp"

#include <arinc_665/media/MediaSet.hpp>
#include <arinc_665/media/RegularFile.hpp>

#include <arinc_665/Arinc665Exception.hpp>
#include <arinc_665/Logger.hpp>

#include <arinc_645/CheckValue.hpp>

#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <utility>

namespace Arinc665::Media {

Load::Load(
  const ContainerEntityPtr &parent,
  std::string name,
  const OptionalMediumNumber &mediumNumber,
  [[maybe_unused]] const CreateKey &createKey ) :
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

void Load::targetHardwareIdPositions( TargetHardwareIdPositions targetHardwareIdPositions )
{
  targetHardwareIdPositionsV = std::move( targetHardwareIdPositions );
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
  targetHardwareIdPositionsV.insert_or_assign( std::move( targetHardwareId ), std::move( positions ) );
}

ConstFilePtr Load::file( std::string_view filename ) const
{
  BOOST_LOG_FUNCTION()

  ConstFiles foundFiles{};

  for ( const auto &file : files() )
  {
    if ( file->name() == filename )
    {
      foundFiles.emplace_back( file );
    }
  }

  if ( 1U == foundFiles.size() )
  {
    return foundFiles.front();
  }

  if ( !foundFiles.empty() )
  {
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::info )
      << "More than one file found for given filename";
  }

  return {};
}

ConstFilePtr Load::file(
  const CheckValues &checkValues,
  std::string_view filename,
  const Arinc645::CheckValue &checkValue ) const
{
  BOOST_LOG_FUNCTION()

  ConstFiles foundFiles{};

  for ( const auto &file : files() )
  {
    if ( file->name() == filename )
    {
      if (
        auto fileCheckValues{ checkValues.find( file ) };
        ( fileCheckValues != checkValues.end() ) &&
        fileCheckValues->second.contains( checkValue ) )
      {
        foundFiles.emplace_back( file );
      }
    }
  }

  if ( 1U == foundFiles.size() )
  {
    return foundFiles.front();
  }

  if ( !foundFiles.empty() )
  {
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::info )
      << "More than one file found for given parameters";
  }

  return {};

}

ConstFiles Load::files() const
{
  BOOST_LOG_FUNCTION()

  // add load file itself
  ConstFiles files{ {
    std::dynamic_pointer_cast< const File >( shared_from_this() ) } };

  // add data files
  for ( const auto &[ file, filePartNumber, checkValueType ] : dataFilesV )
  {
    files.emplace_back( file );
  }

  // add support files
  for ( const auto &[ file, filePartNumber, checkValueType ] : supportFilesV )
  {
    files.emplace_back( file );
  }

  return files;
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

  supportFilesV.emplace_back( file, std::move( partNumber ), checkValueType );
}

Helper::ConstRawDataSpan Load::userDefinedData() const
{
  return userDefinedDataV;
}

Helper::RawData& Load::userDefinedData()
{
  return userDefinedDataV;
}

void Load::userDefinedData( Helper::RawData userDefinedData )
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

void Load::dataFilesCheckValueType( std::optional< Arinc645::CheckValueType > checkValueType )
{
  dataFilesCheckValueTypeV = checkValueType;
}

Arinc645::CheckValueType Load::effectiveSupportFilesCheckValueType() const
{
  return supportFilesCheckValueTypeV.value_or( mediaSet()->effectiveMediaSetCheckValueType() );
}

std::optional< Arinc645::CheckValueType > Load::supportFilesCheckValueType() const
{
  return supportFilesCheckValueTypeV;
}

void Load::supportFilesCheckValueType( std::optional< Arinc645::CheckValueType > checkValueType )
{
  supportFilesCheckValueTypeV = checkValueType;
}

ConstLoadPtr Loads_loadByPartNumber( const ConstLoads &loads, std::string_view partNumber )
{
  BOOST_LOG_FUNCTION()

  for ( const auto &load : loads )
  {
    if ( load->partNumber() == partNumber )
    {
      return load;
    }
  }

  return {};
}

ConstFilePtr Loads_file( const ConstLoads &loads, std::string_view filename, std::string_view loadPartNumber )
{
  BOOST_LOG_FUNCTION()

  if ( !loadPartNumber.empty() )
  {
    const auto load{ Loads_loadByPartNumber( loads, loadPartNumber ) };

    if ( !load )
    {
      BOOST_LOG_SEV( Logger::get(), Helper::Severity::error )
        << "No Load with given Part Number";
      return {};
    }

    return load->file( filename );
  }

  ConstFiles files{};

  for ( const auto &load : loads )
  {
    assert( load );
    if ( auto file{ load->file( filename ) }; file )
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
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::info )
      << "More than one file found for given parameters";
  }

  return {};
}

ConstFilePtr Loads_file(
  const ConstLoads &loads,
  const CheckValues &checkValues,
  std::string_view filename,
  std::string_view loadPartNumber,
  const Arinc645::CheckValue &checkValue )
{
  BOOST_LOG_FUNCTION()

  if ( !loadPartNumber.empty() )
  {
    const auto load{ Loads_loadByPartNumber( loads, loadPartNumber ) };

    if ( !load )
    {
      BOOST_LOG_SEV( Logger::get(), Helper::Severity::error )
        << "No Load with given Part Number";
      return {};
    }

    return load->file( checkValues, filename, checkValue );
  }

  ConstFiles files{};

  for ( const auto &load : loads )
  {
    assert( load );
    if ( auto file{ load->file( checkValues, filename, checkValue ) }; file )
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
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::info )
      << "More than one file found for given parameters";
  }

  // no, or more than one file
  return {};
}

}
