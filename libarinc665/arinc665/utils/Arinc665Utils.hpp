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
 * @brief Declaration of class Arinc665::Utils::Arinc665Utils.
 **/

#ifndef ARINC665_UTILS_ARINC665IMPORTER_HPP
#define ARINC665_UTILS_ARINC665IMPORTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>

#include <boost/filesystem.hpp>

#include <functional>

namespace Arinc665 {
namespace Utils {

class Arinc665Utils
{
  public:
    using MediaSetPtr = Arinc665::Media::MediaSetPtr;
    using ConstMediaSetPtr = Arinc665::Media::ConstMediaSetPtr;
    using path = boost::filesystem::path;

    //! Handler, which is called to obtain the path to the next medium.
    using GetMediumPathHandler = std::function< path( uint8_t mediumNumber)>;
    //! Handler, which is called to obtain the path to the requested file
    using CopyFileHandler = std::function< path( const path &destination)>;

    /**
     * The ARINC 665 Media Set importer.
     * Returns the MediaSet
     **/
    using Arinc665Importer = std::function< MediaSetPtr( void)>;

    /**
     * The ARINC 665 Media Set exporter.
     **/
    using Arinc665Exporter = std::function< void( void)>;

    /**
     * @brief Create a ARINC 665 Media Set importer.
     *
     * @param[in] getMediumPathHandler
     *   Handler which is called to obtain the the current path of the selected
     *   medium.
     *
     * @return The ARINC 665 Media Set importer.
     **/
    static Arinc665Importer createArinc665Importer(
      GetMediumPathHandler getMediumPathHandler);

    /**
     * @brief Creates a ARINC 665 Media Set exporter.
     *
     * @param[in] mediaSet
     *   The media set, which shall be exported.
     * @param[in] getMediumPathHandler
     *   Handler, which is called to obtain the path, where the requested medium
     *   shall be exported to.
     * @param[in] copyFileHandler
     *   When a file needs to be generated, this handler is called.
     * @param[in] createBatchFiles
     *   If set to true, Batch Files are created by exporter.
     * @param[in] createLoadHeaderFiles
     *   If set to true, Load Header Files are created by exporter.
     *
     * @return The ARINC 665 Media Set exporter.
     **/
    static Arinc665Exporter createArinc665Exporter(
      ConstMediaSetPtr mediaSet,
      GetMediumPathHandler getMediumPathHandler,
      CopyFileHandler copyFileHandler,
      bool createBatchFiles = true,
      bool createLoadHeaderFiles = true);
};

}
}

#endif
