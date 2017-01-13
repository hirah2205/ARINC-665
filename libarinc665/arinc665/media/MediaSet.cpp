/*
 * $Date$
 * $Revision$
 */
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

#include <boost/foreach.hpp>

namespace Arinc665 {
namespace Media {

MediaSet::MediaSet( const string &partNumber):
  partNumber( partNumber)
{
}

ConstMediaSetPtr MediaSet::getMediaSet() const
{
  return shared_from_this();
}

MediaSetPtr MediaSet::getMediaSet()
{
  return shared_from_this();
}

MediaSet::Type MediaSet::getType() const
{
  return Type::MediaSet;
}

const MediaSet::string& MediaSet::getName() const
{
  return name;
}

void MediaSet::setName( const string& name)
{
  this->name = name;
}

const MediaSet::string& MediaSet::getPartNumber() const
{
  return partNumber;
}

void MediaSet::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

uint8_t MediaSet::getNumberOfMedia() const
{
  return media.size();
}


const Media& MediaSet::getMedia() const
{
  return media;
}

ConstMediumPtr MediaSet::getMedium( const uint8_t index) const
{
  if (0 == index)
  {
    return MediumPtr();
  }

  return media.at( index - 1);
}

MediumPtr MediaSet::getMedium( const uint8_t index)
{
  if (0 == index)
  {
    return MediumPtr();
  }

  return media.at( index - 1);
}

uint8_t MediaSet::addMedium()
{
  if (media.size() >= 255)
  {
    return 0U;
  }

  media.push_back( std::make_shared< Medium>( shared_from_this(), media.size()));

  return static_cast< uint8_t>( media.size());
}

void MediaSet::setNumberOfMedia( uint8_t numberOfMedia, bool deleteFiles)
{
  if (numberOfMedia == media.size())
  {
    return;
  }

  if (numberOfMedia > media.size())
  {
    while (numberOfMedia < media.size())
    {
      addMedium();
    }
  }
  else
  {
    while (numberOfMedia > media.size())
    {
      //! todo
    }
  }
}

size_t MediaSet::getNumberOfFiles() const
{
  size_t numberOfFiles = 0;

  for ( const auto &medium : media)
  {
    numberOfFiles += medium->getNumberOfFiles( true);
  }

  return numberOfFiles;
}

ConstFiles MediaSet::getFiles() const
{
  ConstFiles files;

  // Iterate over all medias and add their files to a complete list.
  for ( const auto &medium : media)
  {
    ConstFiles mediaFiles = static_cast< const Medium>(*medium).getFiles( true);
    files.insert( files.end(), mediaFiles.begin(), mediaFiles.end());
  }

  return files;
}

Files MediaSet::getFiles()
{
  Files files;

  // Iterate over all medias and add their files to a complete list.
  for ( auto &medium : media)
  {
    Files mediaFiles = medium->getFiles( true);
    files.insert( files.end(), mediaFiles.begin(), mediaFiles.end());
  }

  return files;
}

ConstFilePtr MediaSet::getFile( const string &filename) const
{
  for ( auto const &medium : media)
  {
    ConstFilePtr file( medium->getFile( filename));

    if ( file)
    {
      return file;
    }
  }

  return ConstFilePtr();
}

FilePtr MediaSet::getFile( const string &filename)
{
  for ( auto &medium : media)
  {
    FilePtr file( medium->getFile( filename));

    if ( file)
    {
      return file;
    }
  }

  return FilePtr();
}

size_t MediaSet::getNumberOfLoads() const
{
  size_t numberOfLoads = 0;

  for ( const auto &medium : media)
  {
    numberOfLoads += medium->getNumberOfLoads( true);
  }

  return numberOfLoads;
}

ConstLoads MediaSet::getLoads() const
{
  ConstLoads loads;

  for (const auto & medium : media)
  {
    ConstLoads mediaLoads = static_cast< const Medium>(*medium).getLoads( true);
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

Loads MediaSet::getLoads()
{
  Loads loads;

  for (const auto & medium : media)
  {
    Loads mediaLoads =medium->getLoads( true);
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

size_t MediaSet::getNumberOfBatches() const
{
  size_t numberOfBatches = 0;

  for ( const auto &medium : media)
  {
    numberOfBatches += medium->getNumberOfBatches( true);
  }

  return numberOfBatches;
}

ConstBatches MediaSet::getBatches() const
{
  ConstBatches batches;

  for (const auto & medium : media)
  {
    ConstBatches mediaBatches = static_cast< const Medium>(*medium).getBatches( true);
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

Batches MediaSet::getBatches()
{
  Batches batches;

  for (const auto & medium : media)
  {
    Batches mediaBatches = medium->getBatches( true);
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

}
}
