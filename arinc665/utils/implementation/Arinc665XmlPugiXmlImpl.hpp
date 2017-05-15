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
   virtual LoadXmlResult loadFromXml( const path &xmlFile) override final;

   /**
    * @brief Saves the given Media Set information to the given XML file.
    *
    * @param[in] mediaSet
    *   The Media Set information.
    * @param[in] xmlFile
    *   The ARINC 665 XML file.
    */
   virtual void saveToXml(
     Media::ConstMediaSetPtr mediaSet,
     const FilePathMapping &filePathMapping,
     const path &xmlFile) override final;

  private:
   /**
    * @brief Loads the media set section.
    *
    * @todo Add Loads, and Batches handling.
    *
    * @param filePathMapping
    * @param mediaSetNode
    *
    * @return
    **/
    Media::MediaSetPtr loadMediaSet(
      FilePathMapping &filePathMapping,
      const pugi::xml_node &mediaSetNode);

    /**
     * @brief Saves the media set section.
     *
     * @todo Add Loads, and Batches handling.
     *
     * @param mediaSet
     * @param filePathMapping
     * @param mediaSetNode
     **/
    void saveMediaSet(
      Media::ConstMediaSetPtr mediaSet,
      const FilePathMapping &filePathMapping,
      pugi::xml_node &mediaSetNode);

    /**
     * @brief Load the medium section.
     *
     * @param mediaSet
     * @param filePathMapping
     * @param mediumNode
     **/
    void loadMedium(
      Media::MediaSetPtr mediaSet,
      FilePathMapping &filePathMapping,
      const pugi::xml_node &mediumNode);

    /**
     * @brief Saves the medium section.
     *
     * @param medium
     * @param filePathMapping
     * @param mediumNode
     **/
    void saveMedium(
      Media::ConstMediumPtr medium,
      const FilePathMapping &filePathMapping,
      pugi::xml_node &mediumNode);

    /**
     * @brief Loads a directory section.
     *
     * @param parent
     * @param filePathMapping
     * @param directoryNode
     **/
    void loadDirectory(
      Media::ContainerEntityPtr parent,
      FilePathMapping &filePathMapping,
      const pugi::xml_node &directoryNode);

    /**
     * @brief Saves a directory section.
     *
     * @param directory
     * @param filePathMapping
     * @param directoryNode
     **/
    void saveDirectory(
      Media::ConstDirectoryPtr directory,
      const FilePathMapping &filePathMapping,
      pugi::xml_node &directoryNode);

    /**
     * @brief Loads file entries.
     *
     * @param current
     * @param filePathMapping
     * @param currentNode
     */
    void loadEntries(
      Media::ContainerEntityPtr current,
      FilePathMapping &filePathMapping,
      const pugi::xml_node &currentNode);

    /**
     * @brief Saves file entries.
     *
     * @param current
     * @param filePathMapping
     * @param currentNode
     */
    void saveEntries(
      Media::ConstContainerEntityPtr current,
      const FilePathMapping &filePathMapping,
      pugi::xml_node &currentNode);

    void loadLoad(
      Media::MediaSetPtr mediaSet,
      const pugi::xml_node &loadNode);

    void saveLoad(
      Media::ConstLoadPtr load,
      pugi::xml_node &loadNode);

    void loadBatch(
      Media::MediaSetPtr mediaSet,
      const pugi::xml_node &batchNode);

    void saveBatch(
      Media::ConstBatchPtr load,
      pugi::xml_node &batchNode);
};

}
}

#endif
