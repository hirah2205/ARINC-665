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

  for ( auto const &mediaSet : config.mediaSets)
  {
    // import media set
    auto importer( Arinc665Utils::createArinc665Importer(
      [&mediaSet,&config]( const uint8_t mediumNumber, const path &path)->File::RawFile
      {
        auto medium{ mediaSet.second.find( mediumNumber)};


        if (mediaSet.second.end() == medium)
        {
          BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
            "Medium not found";

          return {};
        }

        // concatenate file path
        auto filePath(
          config.mediaSetBase /
          mediaSet.first /
          medium->second /
          path.relative_path());

        // read file

        File::RawFile data( boost::filesystem::file_size( filePath));

        std::ifstream file(
          filePath.string().c_str(),
          std::ifstream::binary | std::ifstream::in);

        if ( !file.is_open())
        {
          BOOST_THROW_EXCEPTION(
            Arinc665Exception() << AdditionalInfo( "Error opening files"));
        }

        // read the data to the buffer
        file.read( (char*) &data.at( 0), data.size());

        // return the buffer
        return data;
      }));

    // execute importer
    auto impMediaSet( importer( mediaSet.first));

    // add media set
    mediaSetsValue.push_back( impMediaSet);

    // iterate over media
    for ( auto medium : impMediaSet->media())
    {
      // add path mapping
      this->mediaPaths.insert(
        { medium,
          mediaSet.first /
          mediaSet.second.at( medium->mediumNumber())}); // should never fail
    }
  }
}

Media::MediaSetPtr MediaSetManagerImpl::mediaSet(
  const string &partNumber)
{
  for (const auto &mediaSet : mediaSetsValue)
  {
    if (mediaSet->partNumber() == partNumber)
    {
      return mediaSet;
    }
  }
  return {};
}

const MediaSetManagerImpl::MediaSets& MediaSetManagerImpl::mediaSets() const
{
  return mediaSetsValue;
}

MediaSetManagerImpl::MediaSets& MediaSetManagerImpl::mediaSets()
{
  return mediaSetsValue;
}

Media::ConstLoads MediaSetManagerImpl::loads() const
{
  Media::ConstLoads loads;

  for (const auto &mediaSet : mediaSetsValue)
  {
    auto mediaSetLoads( mediaSet->loads());

    loads.insert( loads.end(), mediaSetLoads.begin(), mediaSetLoads.end());
  }

  return loads;
}

MediaSetManagerImpl::path MediaSetManagerImpl::filePath(
  Media::ConstBaseFilePtr file) const
{
  auto mediumIt( mediaPaths.find( file->parent()->medium()));

  if (mediumIt == mediaPaths.end())
  {
    return {};
  }

  return config.mediaSetBase / mediumIt->second / file->path().relative_path();
}

}
}
