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
	PartNumberdEntity( partNumber)
{
}

uint8_t MediaSet::getNumberOfMedia( void) const
{
	return media.size();
}


const Media& MediaSet::getMedia( void) const
{
	return media;
}

ConstMediumPtr MediaSet::getMedium( uint8_t index) const
{
  assert( media.at( index - 1));

	return media.at( index - 1);
}

MediumPtr MediaSet::getMedium( uint8_t index)
{
  assert( media.at( index - 1));

	return media.at( index - 1);
}

unsigned int MediaSet::addMedium( void)
{
  if (media.size() >= 255)
  {
    return 0;
  }

  media.push_back( std::make_shared< Medium>( shared_from_this()));

	return media.size();
}

void MediaSet::setNumberOfMedia( uint8_t numberOfMedia, bool deleteFiles)
{
  if (numberOfMedia == media.size())
  {
    return;
  }

  if (numberOfMedia > media.size())
  {
    for (unsigned int i = 0; i < numberOfMedia - media.size(); ++i)
    {
      addMedium();
    }
  }
  else
  {
    //! todo
  }
}

ConstFiles MediaSet::getFiles( void) const
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

Files MediaSet::getFiles( void)
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

		if (file)
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

		if (file)
		{
			return file;
		}
	}

	return FilePtr();
}

ConstLoads MediaSet::getLoads( void) const
{
  ConstLoads loads;

  for (const auto & medium : media)
  {
    ConstLoads mediaLoads = static_cast< const Medium>(*medium).getLoads( true);
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

	return loads;
}

Loads MediaSet::getLoads( void)
{
  Loads loads;

  for (const auto & medium : media)
  {
    Loads mediaLoads =medium->getLoads( true);
    loads.insert( loads.end(), mediaLoads.begin(), mediaLoads.end());
  }

  return loads;
}

ConstBatches MediaSet::getBatches( void) const
{
  ConstBatches batches;

  for (const auto & medium : media)
  {
    ConstBatches mediaBatches = static_cast< const Medium>(*medium).getBatches( true);
    batches.insert( batches.end(), mediaBatches.begin(), mediaBatches.end());
  }

  return batches;
}

}
}
