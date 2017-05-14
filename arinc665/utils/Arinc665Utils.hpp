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
    using path = boost::filesystem::path;

    //! Handler, which is called to obtain the path to the next medium.
    using GetMediumPathHandler = std::function< path( uint8_t mediumNumber)>;

    //! Handler, which is called to generate the given file at the requested position.
    using CopyFileHandler =
      std::function< path( Media::ConstFilePtr file, const path &destination)>;

    //! The result, which is provided by the ARINC 665 importer.
    using Arinc665ImporterResult =
      std::tuple< Media::MediaSetPtr, Media::FilePathMapping>;

    /**
     * The ARINC 665 Media Set importer.
     * Returns the MediaSet
     **/
    using Arinc665Importer = std::function< Media::MediaSetPtr()>;

    /**
     * The ARINC 665 Media Set exporter.
     **/
    using Arinc665Exporter = std::function< void()>;

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
      Media::ConstMediaSetPtr mediaSet,
      GetMediumPathHandler getMediumPathHandler,
      CopyFileHandler copyFileHandler,
      bool createBatchFiles = true,
      bool createLoadHeaderFiles = true);
};

}
}

#endif
