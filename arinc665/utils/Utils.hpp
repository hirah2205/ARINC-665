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

/**
 * @brief ARINC 665 utilities.
 **/
namespace Arinc665::Utils {

//! File creation policy of the exporter for load headers/ batch file
enum class FileCreationPolicy
{
  //! No file (load header/ batch file) is created by the exporter itself.
  None,
  //! Only non-existing files are created by the exporter itself.
  NoneExisting,
  //! All files are created by the exporter itself - even if already existing in source.
  All,
  //! Invalid value
  Invalid
};


class Arinc665Xml;
//! ARINC 665 XML instance pointer.
using Arinc665XmlPtr = std::unique_ptr< Arinc665Xml>;

class MediaSetConfiguration;
class MediaSetManager;
//! ARINC 665 media set manager instance pointer.
using MediaSetManagerPtr = std::shared_ptr< MediaSetManager>;

}

#endif
