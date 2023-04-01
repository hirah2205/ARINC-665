/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::FilesystemMediaSetCopierImpl.
 **/

#include "FilesystemMediaSetCopierImpl.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <fmt/format.h>

namespace Arinc665::Utils {

FilesystemMediaSetCopier& FilesystemMediaSetCopierImpl::mediaPaths(
  MediaPaths mediaPaths )
{
  mediaPathsV = std::move( mediaPaths );
  return * this;
}

FilesystemMediaSetCopier& FilesystemMediaSetCopierImpl::mediaSetBasePath(
  std::filesystem::path mediaSetBasePath )
{
  mediaSetBasePathV = std::move( mediaSetBasePath );
  return *this;
}

MediaPaths FilesystemMediaSetCopierImpl::operator()()
{
  if ( mediaPathsV.empty() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "No Media Paths Provided" } );
  }

  if ( mediaSetBasePathV.empty()
    || !std::filesystem::is_directory( mediaSetBasePathV ) )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set Base Directory must exist" }
      << boost::errinfo_file_name{ mediaSetBasePathV.string() } );
  }

  // iterate over media
  MediaPaths destinationMediaPaths{};
  for ( auto const &[ mediumNumber, mediumPath ] : mediaPathsV )
  {
    auto destinationMediumDir{
      fmt::format( "MEDIUM_{:03d}", static_cast< uint8_t >( mediumNumber ) ) };

    // copy medium
    std::filesystem::copy(
      mediumPath,
      mediaSetBasePathV / destinationMediumDir,
      std::filesystem::copy_options::recursive );

    // store medium destination path
    destinationMediaPaths.try_emplace(
      mediumNumber,
      destinationMediumDir );
  }

  return destinationMediaPaths;
}

}
