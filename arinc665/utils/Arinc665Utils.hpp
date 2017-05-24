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

#include <arinc665/file/File.hpp>

#include <boost/filesystem.hpp>

#include <functional>
#include <string>

namespace Arinc665 {
namespace Utils {

/**
 * @brief ARINC 665 Utilities.
 *
 * This class provides an interface ARINC 665 Utilities like:
 * * Media set importer, and
 * * Media set exporter
 **/
class Arinc665Utils
{
  public:
    //! Path type
    using path = boost::filesystem::path;

    //! String type
    using string = std::string;

    //! Handler, which is called to generate the given medium.
    using CreateMediumHandler = std::function< void( Media::ConstMediumPtr medium)>;

    //! Handler, which is called to generate the given directory.
    using CreateDirectoryHandler = std::function< void( Media::ConstDirectoryPtr directory)>;

    //! Handler, which is called to generate the given file at the requested position.
    using CreateFileHandler = std::function< void( Media::ConstFilePtr file)>;

    //! Handler, which is called to read a file form a medium.
    using ReadFileHandler = std::function< File::RawFile( uint8_t mediumNumber, const path &path)>;

    //! Handler, which is called to write the given file at the requested position.
    using WriteFileHandler =
      std::function< void( uint8_t mediumNumber, const path &path, const File::RawFile &file)>;

    /**
     * The ARINC 665 Media Set importer.
     * Takes the media set name as input.
     * Returns the MediaSet
     **/
    using Arinc665Importer = std::function< Media::MediaSetPtr(
      const string &mediaSetName)>;

    /**
     * The ARINC 665 Media Set exporter.
     **/
    using Arinc665Exporter = std::function< void()>;

    /**
     * @brief Create a ARINC 665 Media Set importer.
     *
     * @param[in] readFileHandler
     *   Handler which is called to obtain the requested file from the medium.
     *
     * @return The ARINC 665 Media Set importer.
     **/
    static Arinc665Importer createArinc665Importer(
      ReadFileHandler readFileHandler);

    /**
     * @brief Creates a ARINC 665 Media Set exporter.
     *
     * @param[in] mediaSet
     *   The media set, which shall be exported.
     * @param[in] createMediumHandler
     *   Called to create the given medium.
     * @param[in] createDirectoryHandler
     *   Called to create the given directory.
     * @param[in] createFileHandler
     *   When a file needs to be generated, this handler is called.
     * @param[in] writeFileHandler
     *   Writes a given file to the output media set.
     * @param[in] readFileHandler
     *   Reads a given file from the output media set.
     *   Used for CRC calculation.
     * @param[in] arinc665Version
     *   The ARINC 665 version used for exporting
     * @param[in] createBatchFiles
     *   If set to true, Batch Files are created by exporter.
     * @param[in] createLoadHeaderFiles
     *   If set to true, Load Header Files are created by exporter.
     *
     * @return The ARINC 665 Media Set exporter.
     **/
    static Arinc665Exporter createArinc665Exporter(
      Media::ConstMediaSetPtr mediaSet,
      CreateMediumHandler createMediumHandler,
      CreateDirectoryHandler createDirectoryHandler,
      CreateFileHandler createFileHandler,
      WriteFileHandler writeFileHandler,
      ReadFileHandler readFileHandler,
      Arinc665Version arinc665Version = Arinc665Version::ARINC_665_2,
      bool createBatchFiles = false,
      bool createLoadHeaderFiles = false);
};

}
}

#endif
