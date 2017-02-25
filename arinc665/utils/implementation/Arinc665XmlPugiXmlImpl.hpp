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
* @brief Declaration of class Arinc665::Utils::Arinc665XmlPugiXmlImpl.
 **/

#ifndef ARINC665_XML_ARINC665XMLPUGIXMLIMPL_HPP
#define ARINC665_XML_ARINC665XMLPUGIXMLIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <pugixml.hpp>

namespace Arinc665 {
namespace Utils {

/**
* @brief Handles the representation of Media Sets as XML file.
**/
class Arinc665XmlPugiXmlImpl : public Arinc665Xml
{
  public:
   //! Default constructor
    Arinc665XmlPugiXmlImpl() = default;

   //! Default descructor
   virtual ~Arinc665XmlPugiXmlImpl() = default;

   /**
    * @brief Loads the Media Set information from the given XML file.
    *
    * @param[in] xmlFile
    *   The ARINC 665 XML file.
    *
    * @return The loaded Media Set information.
    **/
   virtual MediaSetPtr loadFromXml( const path &xmlFile) override final;

   /**
    * @brief Saves the given Media Set information to the given XML file.
    *
    * @param[in] mediaSet
    *   The Media Set information.
    * @param[in] xmlFile
    *   The ARINC 665 XML file.
    */
   virtual void saveToXml( ConstMediaSetPtr mediaSet, const path &xmlFile) override final;

  private:
   using ConstMediumPtr = Media::ConstMediumPtr;
   using ContainerEntityPtr = Media::ContainerEntityPtr;
   using ConstContainerEntityPtr = Media::ConstContainerEntityPtr;
   using ConstDirectoryPtr = Media::ConstDirectoryPtr;

    MediaSetPtr loadMediaSet( const pugi::xml_node &mediaSetNode);

    void saveMediaSet( ConstMediaSetPtr mediaSet, pugi::xml_node &mediaSetNode);


    void loadMedium( MediaSetPtr mediaSet, const pugi::xml_node &mediumNode);

    void saveMedium( ConstMediumPtr medium, pugi::xml_node &mediumNode);


    void loadDirectory( ContainerEntityPtr parent, const pugi::xml_node &directoryNode);

    void saveDirectory( ConstDirectoryPtr directory, pugi::xml_node &directoryNode);


    void loadEntries( ContainerEntityPtr current, const pugi::xml_node &currentNode);

    void saveEntries( ConstContainerEntityPtr current, pugi::xml_node &currentNode);
};

}
}

#endif
