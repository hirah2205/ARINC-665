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
 * @brief Declaration of class Arinc665::Utils::Arinc665ImporterImpl.
 **/

#ifndef ARINC665_UTILS_ARINC665IMPORTERIMPL_HPP
#define ARINC665_UTILS_ARINC665IMPORTERIMPL_HPP

#include <arinc665/utils/Arinc665Importer.hpp>


#include <arinc665/file/Arinc665File.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <boost/filesystem.hpp>

#include <functional>

namespace Arinc665 {
namespace Utils {

using Arinc665::File::RawFile;
using Arinc665::File::Arinc665File;
using Arinc665::Media::MediaSetPtr;

class Arinc665ImporterImpl : public Arinc665Importer
{
  public:
    void import( GetMediumHandler getMediumHandler);

    MediaSetPtr getMediaSet( void);

  protected:

    void addMedium( const unsigned int mediaIndex, const path &mediumPath);

    RawFile loadFile( const path &filePath);

  private:
    //! The media set.
    MediaSetPtr mediaSet;
};

}
}

#endif
