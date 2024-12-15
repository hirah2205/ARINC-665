// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::Arinc665Xml.
 **/

#include "Arinc665Xml.hpp"

#if LIBXMLPPVERSION==26
#include <arinc_665/utils/implementation/Arinc665XmlLoadImpl26.hpp>
#include <arinc_665/utils/implementation/Arinc665XmlSaveImpl26.hpp>
#elif LIBXMLPPVERSION==5
#include <arinc_665/utils/implementation/Arinc665XmlLoadImpl5.hpp>
#include <arinc_665/utils/implementation/Arinc665XmlSaveImpl5.hpp>
#endif

namespace Arinc665::Utils {

LoadXmlResult Arinc665Xml_load( const std::filesystem::path &xmlFile )
{
#if LIBXMLPPVERSION==26
  Arinc665XmlLoadImpl26 load{ xmlFile };
#elif LIBXMLPPVERSION==5
  Arinc665XmlLoadImpl5 load{ xmlFile };
#endif
  return load();
}
void Arinc665Xml_save(
  const Media::MediaSet &mediaSet,
  const FilePathMapping &filePathMapping,
  const std::filesystem::path &xmlFile )
{
#if LIBXMLPPVERSION==26
  Arinc665XmlSaveImpl26 save{ mediaSet, filePathMapping, xmlFile };
#elif LIBXMLPPVERSION==5
  Arinc665XmlSaveImpl5 save{ mediaSet, filePathMapping, xmlFile };
#endif
  save();
}

}
