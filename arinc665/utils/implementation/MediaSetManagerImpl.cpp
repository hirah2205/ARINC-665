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

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>

#include <fstream>

namespace Arinc665 {
namespace Utils {

MediaSetManagerImpl::MediaSetManagerImpl(
  const MediaSetConfiguration &config):
  config( config)
{
  BOOST_LOG_FUNCTION();

  for ( auto const &mediaSetConfig : config.mediaSets)
  {
    // import media set
    auto importer( Arinc665Utils::createArinc665Importer(
      [&mediaSetConfig,&config]( const uint8_t mediumNumber, const path &path)->File::RawFile
      {
        if (mediumNumber > mediaSetConfig.second.size())
        {
          BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
            "More media then stored requested";

          return {};
        }

        auto filePath(
          config.mediaSetBase /
          mediaSetConfig.first /
          mediaSetConfig.second[ mediumNumber-1] /
          path.relative_path());

        File::RawFile data( boost::filesystem::file_size( filePath));

        std::ifstream file(
          filePath.string().c_str(),
          std::ifstream::binary | std::ifstream::in);

        if ( !file.is_open())
        {
          //! @throw Arinc665Exception
          BOOST_THROW_EXCEPTION(
            Arinc665Exception() << AdditionalInfo( "Error opening files"));
        }

        // read the data to the buffer
        file.read( (char*) &data.at( 0), data.size());

        // return the buffer
        return data;
      }));

    // execute importer
    auto mediaSet( importer( mediaSetConfig.first));

    // add media set
    mediaSets.push_back( mediaSet);

    // iterate over media
    for ( auto medium : mediaSet->getMedia())
    {
      // add path mapping
      this->mediaPaths.insert(
        { medium,
          mediaSetConfig.first /
          mediaSetConfig.second[ medium->getMediumNumber() - 1]});
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

  return config.mediaSetBase / mediumIt->second / file->getPath().relative_path();
}

}
}
