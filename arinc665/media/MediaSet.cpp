/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Media::MediaSet.
 **/

#include "MediaSet.hpp"

#include <arinc665/media/Medium.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <boost/foreach.hpp>

#include <cassert>

namespace Arinc665 {
namespace Media {

MediaSet::MediaSet( const string &name):
  nameValue( name)
{
}

MediaSet::MediaSet( string &&name):
  nameValue( std::move( name))
{
}

ConstMediaSetPtr MediaSet::mediaSet() const
{
  return shared_from_this();
}

MediaSetPtr MediaSet::mediaSet()
{
  return shared_from_this();
}

MediaSet::Type MediaSet::type() const
{
  return Type::MediaSet;
}

const MediaSet::string& MediaSet::name() const
{
  return nameValue;
}

void MediaSet::name( const string &name)
{
  nameValue = name;
}

void MediaSet::name( string &&name)
{
  nameValue = std::move( name);
}

MediaSet::string MediaSet::partNumber() const
{
  return partNumberValue;
}

void MediaSet::partNumber( const string &partNumber)
{
  partNumberValue = partNumber;
}

void MediaSet::partNumber( string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

uint8_t MediaSet::numberOfMedia() const
{
  assert( mediaValue.size() <= std::numeric_limits< uint8_t>::max());

  return static_cast< uint8_t>( mediaValue.size());
}

void MediaSet::numberOfMedia(
  const uint8_t numberOfMedia,
  const bool deleteFiles)
{
  if (numberOfMedia == mediaValue.size())
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "No actions needed";
    return;
  }

  if (numberOfMedia > mediaValue.size())
  {
    // Add media
    while (numberOfMedia > mediaValue.size())
    {
      addMedium();
    }
  }
  else
  {
    // remove media
    while (numberOfMedia < mediaValue.size())
    {
      removeMedium( deleteFiles);
    }
  }
}

ConstMedia MediaSet::media() const
{
  return ConstMedia( mediaValue.begin(), mediaValue.end());
}

Media MediaSet::media()
{
  return mediaValue;
}

ConstMediumPtr MediaSet::medium( const uint8_t index) const
{
  if (0 == index)
  {
    return {};
  }

  return mediaValue.at( index - 1);
}

MediumPtr MediaSet::medium( const uint8_t index)
{
  if (0 == index)
  {
    return {};
  }

  return mediaValue.at( index - 1);
}

MediumPtr MediaSet::addMedium()
{
  if (mediaValue.size() >= 255)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
      "Maximum number of media reached";
    return MediumPtr();
  }

  MediumPtr medium( std::make_shared< Medium>(
    shared_from_this(),
    static_cast< uint8_t>( mediaValue.size() + 1)));

  mediaValue.push_back( medium);

  return medium;
}

void MediaSet::removeMedium( const bool deleteFiles [[gnu::unused]])
{
  //! @todo implement
  BOOST_THROW_EXCEPTION( std::exception());
}

size_t MediaSet::numberOfFiles() const
{
  size_t numberOfFiles = 0;

  for ( const auto &medium : mediaValue)
  {
    numberOfFiles += medium->numberOfFiles( true);
  }

  return numberOfFiles;
}

ConstFiles MediaSet::files() const
{
  ConstFiles files;

  // Iterate over all medias and add their files to a complete list.
  for ( const auto &medium : mediaValue)
  {
    auto mediaFiles{ static_cast< const Medium&>(*medium).files( true)};
    files.insert( files.end(), mediaFiles.begin(), mediaFiles.end());
  }

  return files;
}

Files MediaSet::files()
{
  Files files;

  // Iterate over all medias and add their files to a complete list.
  for ( auto &medium : mediaValue)
  {
    auto mediaFiles{ medium->files( true)};
    files.insert( files.end(), mediaFiles.begin(), mediaFiles.end());
  }

  return files;
}

ConstFilePtr MediaSet::file( const string &filename) const
{
  for ( auto const &medium : mediaValue)
  {
    ConstFilePtr file( medium->file( filename, true));

    if ( file)
    {
      return file;
    }
  }

  return ConstFilePtr();
}

FilePtr MediaSet::file( const string &filename)
{
  for ( auto &medium : mediaValue)
  {
    FilePtr file( medium->file( filename, true));

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

  for ( const auto &medium : mediaValue)
  {
    numberOfLoads += medium->numberOfLoads( true);
  }

  return numberOfLoads;
}

ConstLoads MediaSet::loads() const
{
  ConstLoads loads;

  for (const auto & medium : mediaValue)
  {
    ConstLoads mediaLoads( static_cast< const Medium&>(*medium).loads( true));
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

Loads MediaSet::loads()
{
  Loads loads;

  for (const auto & medium : mediaValue)
  {
    Loads mediaLoads{ medium->loads( true)};
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

ConstLoadPtr MediaSet::load( const string &filename) const
{
  for (const auto & medium : mediaValue)
  {
    auto load( medium->load( filename, true));

    if (load)
    {
      return load;
    }
  }

  return {};
}

LoadPtr MediaSet::load( const string &filename)
{
  for (const auto & medium : mediaValue)
  {
    auto load( medium->load( filename, true));

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

  for ( const auto &medium : mediaValue)
  {
    numberOfBatches += medium->numberOfBatches( true);
  }

  return numberOfBatches;
}

ConstBatches MediaSet::batches() const
{
  ConstBatches batches;

  for (const auto & medium : mediaValue)
  {
    auto mediaBatches(
      std::const_pointer_cast< const Medium>(medium)->batches( true));
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

Batches MediaSet::batches()
{
  Batches batches;

  for (const auto & medium : mediaValue)
  {
    auto mediaBatches( medium->batches( true));
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

ConstBatchPtr MediaSet::batch( const string &filename) const
{
  for (const auto & medium : mediaValue)
  {
    auto batch( medium->batch( filename, true));

    if (batch)
    {
      return batch;
    }
  }

  return {};
}

BatchPtr MediaSet::batch( const string &filename)
{
  for (const auto & medium : mediaValue)
  {
    auto batch( medium->batch( filename, true));

    if (batch)
    {
      return batch;
    }
  }

  return {};
}

}
}
