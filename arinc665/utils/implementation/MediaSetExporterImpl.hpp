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
     * @param[in] getMediumPathHandler
     *   The handler which is called to obtain the path of a medium.
     **/
    MediaSetExporterImpl(
      Media::ConstMediaSetPtr mediaSet,
      Arinc665Utils::GetMediumPathHandler getMediumPathHandler,
      Arinc665Utils::CopyFileHandler copyFileHandler,
      bool createBatchFiles = false,
      bool createLoadHeaderFiles = false);

    /**
     * @brief Entry-point of the ARINC 665 media set exporter.
     *
     * @return
     **/
    void operator()();

  private:
    void exportMedium( Media::ConstMediumPtr medium);

    void exportDirectory(  const path &base, Media::ConstDirectoryPtr directory);

    void exportFile(  const path &base, Media::ConstFilePtr file);

    //! The Media Set
    Media::ConstMediaSetPtr mediaSet;
    //! Medium path handler
    Arinc665Utils::GetMediumPathHandler getMediumPathHandler;
    Arinc665Utils::CopyFileHandler copyFileHandler;
    const bool createBatchFiles;
    const bool createLoadHeaderFiles;
};

}
}

#endif
