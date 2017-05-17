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
 * @brief Definition of class Arinc665::Utils::MediaSetManagerImpl.
 **/

#include "MediaSetManagerImpl.hpp"

#include <arinc665/Arinc665Logger.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>

#include <boost/property_tree/ptree.hpp>

namespace Arinc665 {
namespace Utils {

MediaSetManagerImpl::MediaSetManagerImpl(
  const boost::property_tree::ptree &config,
  const path &mediaSetBase)
{
  BOOST_LOG_FUNCTION();

  // iterate over media sets
  for ( auto &mediaSetConfig : config.get_child( "media_set"))
  {
    std::vector< path> mediaPaths;

    // iterate over media
    for ( auto &mediumConfig : mediaSetConfig.second.get_child( "medium"))
    {
      mediaPaths.push_back( mediumConfig.second.get_value< path>());
    }

    // import media set
    auto importer( Arinc665Utils::createArinc665Importer(
      [&mediaPaths,&mediaSetBase]( const uint8_t mediumNumber)
      {
        if (mediumNumber > mediaPaths.size())
        {
          BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
            "More media then stored requested";

          return path();
        }

        const auto &mediumPath( mediaPaths[ mediumNumber-1]);

        if (mediumPath.is_absolute())
        {
          return mediumPath;
        }

        return mediaSetBase / mediumPath;
      }));

    // execute importer
    auto mediaSet( importer( mediaSetConfig.second.get< string>( "name", {})));

    // add media set
    mediaSets.push_back( mediaSet);

    // iterate over media
    for ( auto medium : mediaSet->getMedia())
    {
      // add path mapping
      this->mediaPaths.insert(
        { medium, mediaPaths[ medium->getMediumNumber() - 1]});
    }
  }
}

Media::MediaSetPtr MediaSetManagerImpl::getMediaSet(
  const string &partNumber)
{
  for (const auto &mediaSet : mediaSets)
  {
    if (mediaSet->getPartNumber() == partNumber)
    {
      return mediaSet;
    }
  }
  return {};
}

const MediaSetManagerImpl::MediaSetList& MediaSetManagerImpl::getMediaSets() const
{
  return mediaSets;
}

MediaSetManagerImpl::MediaSetList& MediaSetManagerImpl::getMediaSets()
{
  return mediaSets;
}

Media::ConstLoads MediaSetManagerImpl::getLoads() const
{
  Media::ConstLoads loads;

  for (const auto &mediaSet : mediaSets)
  {
    auto mediaSetLoads( mediaSet->getLoads());

    loads.insert( loads.end(), mediaSetLoads.begin(), mediaSetLoads.end());
  }

  return loads;
}

MediaSetManagerImpl::path MediaSetManagerImpl::getFilePath(
  Media::ConstBaseFilePtr file) const
{
  auto mediumIt( mediaPaths.find( file->getParent()->getMedium()));

  if (mediumIt == mediaPaths.end())
  {
    return {};
  }

  return mediumIt->second / file->getPathname().relative_path();
}

}
}
