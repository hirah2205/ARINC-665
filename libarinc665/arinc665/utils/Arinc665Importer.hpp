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
 * @brief Declaration of class Arinc665::Utils::Arinc665Importer.
 **/

#ifndef ARINC665_UTILS_ARINC665IMPORTER_HPP
#define ARINC665_UTILS_ARINC665IMPORTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>

#include <boost/filesystem.hpp>

#include <functional>

namespace Arinc665 {
namespace Utils {

class Arinc665Importer
{
  public:
    using MediaSetPtr = Arinc665::Media::MediaSetPtr;
    using path = boost::filesystem::path;

    using GetMedium = path (*)( unsigned int mediumNumber);

    using GetMediumHandler = std::function< GetMedium>;

    static Arinc665ImporterPtr create( void);

    virtual ~Arinc665Importer( void) = default;

    virtual void import( GetMediumHandler getMediumHandler) = 0;

    virtual MediaSetPtr getMediaSet( void) = 0;
};

}
}

#endif
