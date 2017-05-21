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
 * @brief Declaration of class Arinc665::Utils::MediaSetExporterImpl.
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP_
#define ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP_

#include <arinc665/utils/Arinc665Utils.hpp>

namespace Arinc665 {
namespace Utils {

class MediaSetExporterImpl
{
  public:
    using path = Arinc665Utils::path;

    using string = std::string;

    /**
     * @brief Initialises the ARINC 665 media set importer
     *
     * @param[in] mediaSet
     *   The media set, which shall be exported.
     * @param[in] createFileHandler
     *   When a file needs to be generated, this handler is called.
     * @param[in] writeFileHandler
     *   When a file needs to be generated, this handler is called with data.
     * @param[in] createBatchFiles
     *   If set to true, Batch Files are created by exporter.
     * @param[in] createLoadHeaderFiles
     *   If set to true, Load Header Files are created by exporter.
     **/
    MediaSetExporterImpl(
      Media::ConstMediaSetPtr mediaSet,
      Arinc665Utils::CreateFileHandler createFileHandler,
      Arinc665Utils::WriteFileHandler writeFileHandler,
      bool createBatchFiles,
      bool createLoadHeaderFiles);

    /**
     * @brief Entry-point of the ARINC 665 media set exporter.
     *
     * @return
     **/
    void operator()();

  private:
    void exportMedium( Media::ConstMediumPtr medium);

    void exportDirectory( Media::ConstDirectoryPtr directory);

    void exportFile( Media::ConstFilePtr file);

    //! The Media Set
    Media::ConstMediaSetPtr mediaSet;
    //! Medium path handler
    Arinc665Utils::CreateFileHandler createFileHandler;
    Arinc665Utils::WriteFileHandler writeFileHandler;
    const bool createBatchFiles;
    const bool createLoadHeaderFiles;
};

}
}

#endif
