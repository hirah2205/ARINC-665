/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of namespace Arinc665::Utils.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665::Utils.
 **/

#ifndef ARINC665_UTILS_UTILS_HPP
#define ARINC665_UTILS_UTILS_HPP

#include <arinc665/Arinc665.hpp>

#include <memory>

/**
 * @brief ARINC 665 Utilities.
 *
 * This namespace provides utilities for handling ARINC 665 Media Sets:
 *  - Importing and Exporting Media Sets
 *  - XML Handling
 *  - Media Set Manager
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
//! ARINC 665 XML Instance Pointer.
using Arinc665XmlPtr = std::unique_ptr< Arinc665Xml>;

class MediaSetExporter;
//! ARINC 665 Media Set Exporter Instance Pointer.
using MediaSetExporterPtr = std::unique_ptr< MediaSetExporter >;

class MediaSetImporter;
//! ARINC 665 Media Set Importer Instance Pointer.
using MediaSetImporterPtr = std::unique_ptr< MediaSetImporter >;

class MediaSetValidator;
//! ARINC 665 Media Set Validator Instance Pointer.
using MediaSetValidatorPtr = std::unique_ptr< MediaSetValidator >;

class MediaSetManagerConfiguration;
class MediaSetManager;
//! ARINC 665 Media Set Manager Instance Pointer.
using MediaSetManagerPtr = std::shared_ptr< MediaSetManager >;

}

#endif
