// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of namespace Arinc665::Utils.
 **/

#include "Utils.hpp"

#include <arinc_665/files/FileListFile.hpp>
#include <arinc_665/files/MediaSetInformation.hpp>

#include <arinc_665/Logger.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include <fstream>

namespace Arinc665::Utils {

std::optional< Files::MediaSetInformation > getMediumInformation( const std::filesystem::path &directory )
{
  BOOST_LOG_FUNCTION()

  try
  {
    // Check existence of directory
    if ( !std::filesystem::is_directory( directory ) )
    {
      BOOST_LOG_SEV( Logger::get(), Helper::Severity::warning )
        << directory << " is not a directory";
      return {};
    }

    const auto fileListFilePath{ directory / ListOfFilesName };

    // check existence of list of files
    if ( !std::filesystem::is_regular_file( fileListFilePath ) )
    {
      BOOST_LOG_SEV( Logger::get(), Helper::Severity::warning )
        << fileListFilePath << " is not a regular file";
      return {};
    }

    const auto fileSize{ std::filesystem::file_size( fileListFilePath ) };

    std::ifstream file{ fileListFilePath.string().c_str(), std::ifstream::binary | std::ifstream::in };

    if ( !file.is_open() )
    {
      BOOST_LOG_SEV( Logger::get(), Helper::Severity::warning )
        << "Error opening file " << fileListFilePath;
      return {};
    }

    Helper::RawData rawFile( fileSize );
    file.read( (char *)std::data( rawFile ), static_cast< std::streamsize >( fileSize ) );

    if ( file.bad() || ( file.gcount() != static_cast< std::streamsize >( fileSize ) ) )
    {
      BOOST_LOG_SEV( Logger::get(), Helper::Severity::warning )
        << "Error reading from file " << fileListFilePath
        << " read " << file.gcount() << " bytes";
      return {};
    }

    const Files::FileListFile fileListFile{ rawFile };

    return fileListFile.mediaSetInformation();
  }
  catch ( const boost::exception &e )
  {
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::warning )
      << "Exception " << boost::diagnostic_information( e, false );
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::info )
      << "Exception Details " << boost::diagnostic_information( e );

    return {};
  }
}

}
