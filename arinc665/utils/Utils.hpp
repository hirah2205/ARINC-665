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
 * @brief Declaration of namespace Arinc665::Utils.
 **/

#ifndef ARINC665_UTILS_UTILS_HPP
#define ARINC665_UTILS_UTILS_HPP

#include <arinc665/Arinc665.hpp>

#include <memory>

namespace Arinc665 {

/**
 * @brief Namespace of the ARINC 665 utilities.
 **/
namespace Utils {

class Arinc665Xml;
using Arinc665XmlPtr = std::unique_ptr< Arinc665Xml>;

class MediaSetConfiguration;
class MediaSetManager;
using MediaSetManagerPtr = std::shared_ptr< MediaSetManager>;

}
}

#endif
