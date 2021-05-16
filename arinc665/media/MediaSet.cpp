/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::MediaSet.
 **/

#include "MediaSet.hpp"

#include <arinc665/media/Medium.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <boost/foreach.hpp>

#include <cassert>

namespace Arinc665::Media {

ConstMediaSetPtr MediaSet::mediaSet() const
{
  return std::dynamic_pointer_cast< const MediaSet>( shared_from_this());
}

MediaSetPtr MediaSet::mediaSet()
{
  return std::dynamic_pointer_cast< MediaSet>( shared_from_this());
}

MediaSet::Type MediaSet::type() const
{
  return Type::MediaSet;
}

std::string_view MediaSet::partNumber() const
{
  return partNumberV;
}

void MediaSet::partNumber( std::string_view partNumber)
{
  partNumberV = partNumber;
}

void MediaSet::partNumber( std::string &&partNumber)
{
  partNumberV = std::move( partNumber);
}

uint8_t MediaSet::numberOfMedia() const
{
  assert( mediaV.size() <= std::numeric_limits< uint8_t>::max());

  return static_cast< uint8_t>( mediaV.size());
}

void MediaSet::numberOfMedia(
  const uint8_t numberOfMedia,
  const bool deleteFiles)
{
  if (numberOfMedia == mediaV.size())
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info)
      << "No actions needed";
    return;
  }

  if (numberOfMedia > mediaV.size())
  {
    // Add media
    while (numberOfMedia > mediaV.size())
    {
      addMedium();
    }
  }
  else
  {
    // remove media
    while (numberOfMedia < mediaV.size())
    {
      removeMedium( deleteFiles);
    }
  }
}

ConstMedia MediaSet::media() const
{
  return ConstMedia( mediaV.begin(), mediaV.end());
}

Media MediaSet::media()
{
  return mediaV;
}

ConstMediumPtr MediaSet::medium( const uint8_t index) const
{
  auto medium{ mediaV.find( index)};

  if (medium == mediaV.end())
  {
    return {};
  }

  return medium->second;
}

MediumPtr MediaSet::medium( const uint8_t index)
{
  auto medium{ mediaV.find( index)};

  if (medium == mediaV.end())
  {
    return {};
  }

  return medium->second;
}

MediumPtr MediaSet::addMedium()
{
  if ( mediaV.size() >= 255 )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning)
      << "Maximum number of media reached";
    return {};
  }

  const auto newIndex{ static_cast< uint8_t>( mediaV.size() + 1U)};

  auto medium( std::make_shared< Medium>(
    std::dynamic_pointer_cast< MediaSet>( shared_from_this()),
    newIndex ));

  mediaV.insert( {newIndex, medium });

  return medium;
}

void MediaSet::removeMedium( const bool deleteFiles [[maybe_unused]])
{
  //! @todo implement
  BOOST_THROW_EXCEPTION( std::exception());
}

size_t MediaSet::numberOfFiles() const
{
  size_t numberOfFiles = 0;

  for ( const auto &medium : mediaV)
  {
    numberOfFiles += medium.second->numberOfFiles( true);
  }

  return numberOfFiles;
}

ConstFiles MediaSet::files() const
{
  ConstFiles files;

  // Iterate over all media and add their files to a complete list.
  for ( const auto &[mediumNumber,medium] : mediaV )
  {
    auto mediaFiles{ static_cast< const Medium&>( *medium).files( true) };
    files.insert( files.end(), mediaFiles.begin(), mediaFiles.end());
  }

  return files;
}

Files MediaSet::files()
{
  Files files;

  // Iterate over all media and add their files to a complete list.
  for ( auto &[mediumNumber,medium] : mediaV)
  {
    auto mediaFiles{ medium->files( true) };
    files.insert( files.end(), mediaFiles.begin(), mediaFiles.end() );
  }

  return files;
}

ConstFilePtr MediaSet::file( std::string_view filename) const
{
  for ( auto const &medium : mediaV)
  {
    ConstFilePtr file( medium.second->file( filename, true));

    if ( file)
    {
      return file;
    }
  }

  return ConstFilePtr();
}

FilePtr MediaSet::file( std::string_view filename)
{
  for ( auto &medium : mediaV)
  {
    FilePtr file( medium.second->file( filename, true));

    if ( file)
    {
      return file;
    }
  }

  return FilePtr();
}

size_t MediaSet::numberOfLoads() const
{
  size_t numberOfLoads = 0;

  for ( const auto &medium : mediaV)
  {
    numberOfLoads += medium.second->numberOfLoads( true);
  }

  return numberOfLoads;
}

ConstLoads MediaSet::loads() const
{
  ConstLoads loads;

  for (const auto & medium : mediaV)
  {
    ConstLoads mediaLoads( static_cast< const Medium&>(*medium.second).loads(
      true));
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

Loads MediaSet::loads()
{
  Loads loads;

  for (const auto & medium : mediaV)
  {
    Loads mediaLoads{ medium.second->loads( true)};
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

ConstLoadPtr MediaSet::load( std::string_view filename) const
{
  for (const auto & medium : mediaV)
  {
    auto load( medium.second->load( filename, true));

    if (load)
    {
      return load;
    }
  }

  return {};
}

LoadPtr MediaSet::load( std::string_view filename)
{
  for (const auto & medium : mediaV)
  {
    auto load( medium.second->load( filename, true));

    if (load)
    {
      return load;
    }
  }

  return {};
}

size_t MediaSet::numberOfBatches() const
{
  size_t numberOfBatches = 0;

  for ( const auto &medium : mediaV)
  {
    numberOfBatches += medium.second->numberOfBatches( true);
  }

  return numberOfBatches;
}

ConstBatches MediaSet::batches() const
{
  ConstBatches batches;

  for (const auto & medium : mediaV)
  {
    auto mediaBatches(
      std::const_pointer_cast< const Medium>( medium.second)->batches( true));
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

Batches MediaSet::batches()
{
  Batches batches;

  for (const auto & medium : mediaV)
  {
    auto mediaBatches( medium.second->batches( true));
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

ConstBatchPtr MediaSet::batch( std::string_view filename ) const
{
  for ( const auto & medium : mediaV)
  {
    auto batch( medium.second->batch( filename, true));

    if (batch)
    {
      return batch;
    }
  }

  return {};
}

BatchPtr MediaSet::batch( std::string_view filename)
{
  for ( const auto & medium : mediaV)
  {
    auto batch( medium.second->batch( filename, true));

    if ( batch )
    {
      return batch;
    }
  }

  return {};
}

const MediaSet::UserDefinedData& MediaSet::filesUserDefinedData() const
{
  return filesUserDefinedDataV;
}

MediaSet::UserDefinedData& MediaSet::filesUserDefinedData()
{
  return filesUserDefinedDataV;
}

void MediaSet::filesUserDefinedData( const UserDefinedData &userDefinedData )
{
  filesUserDefinedDataV = userDefinedData;
}

void MediaSet::filesUserDefinedData( UserDefinedData &&userDefinedData )
{
  filesUserDefinedDataV = std::move( userDefinedData );
}

const MediaSet::UserDefinedData& MediaSet::loadsUserDefinedData() const
{
  return loadsUserDefinedDataV;
}

MediaSet::UserDefinedData& MediaSet::loadsUserDefinedData()
{
  return loadsUserDefinedDataV;
}

void MediaSet::loadsUserDefinedData( const UserDefinedData &userDefinedData )
{
  loadsUserDefinedDataV = userDefinedData;
}

void MediaSet::loadsUserDefinedData( UserDefinedData &&userDefinedData )
{
  loadsUserDefinedDataV = std::move( userDefinedData);
}

const MediaSet::UserDefinedData& MediaSet::batchesUserDefinedData() const
{
  return batchesUserDefinedDataV;
}

MediaSet::UserDefinedData& MediaSet::batchesUserDefinedData()
{
  return batchesUserDefinedDataV;
}

void MediaSet::batchesUserDefinedData( const UserDefinedData &userDefinedData)
{
  batchesUserDefinedDataV = userDefinedData;
}

void MediaSet::batchesUserDefinedData( UserDefinedData &&userDefinedData)
{
  batchesUserDefinedDataV = std::move( userDefinedData);
}

}
