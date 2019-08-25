/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::Arinc665XmlPugiXmlImpl.
 **/

#ifndef ARINC665_UTILS_ARINC665XMLPUGIXMLIMPL_HPP
#define ARINC665_UTILS_ARINC665XMLPUGIXMLIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <pugixml.hpp>

namespace Arinc665::Utils {

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
     *
     * @throw Arinc665::Arinc665Exception
     *   When XML file does not exist.
     * @throw Arinc665::Arinc665Exception
     *   When Loading of XML fails.
     **/
    LoadXmlResult loadFromXml( const std::filesystem::path &xmlFile) final;

    /**
     * @brief Saves the given Media Set information to the given XML file.
     *
     * @param[in] mediaSet
     *   The Media Set information.
     * @param[in] filePathMapping
     *   The file path mapping
     *   (used to insert the correct source path attribute.)
     * @param[in] xmlFile
     *   The ARINC 665 XML file.
     *
     * @throw Arinc665Exception
     *   When XML file cannot be written.
     **/
    void saveToXml(
      Media::ConstMediaSetPtr mediaSet,
      const FilePathMapping &filePathMapping,
      const std::filesystem::path &xmlFile) final;

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
     * @param[in] parent
     *   Parent Container
     * @param[in] filePathMapping
     *   The file path mapping
     * @param[in] directoryNode
     *   The XML node representing the directory.
     *
     * @throw Arinc665::Arinc665Exception
     *   When Name Attribute is missing or empty.
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

    /**
     * @brief Loads the given load-node.
     *
     * @param[in,out] mediaSet
     *   The media set where the load is stored into.
     * @param[in] loadNode
     *   The XML-node, where the data is loaded from.
     **/
    void loadLoad( Media::MediaSetPtr mediaSet, const pugi::xml_node &loadNode);

    /**
     * @brief Stores the given load-node.
     *
     * @param[in] load
     *   The load to store.
     * @param[in,out] loadNode
     *   The XML-node where the data is stored to.
     **/
    void saveLoad(
      Media::ConstLoadPtr load,
      pugi::xml_node &loadNode);

    /**
     * @brief Loads the given batch-node.
     *
     * @param[in,out] mediaSet
     *   The media set where the load is stored into.
     * @param[in] batchNode
     *   The XML-node, where the data is loaded from.
     **/
    void loadBatch(
      Media::MediaSetPtr mediaSet,
      const pugi::xml_node &batchNode);

    /**
     * @brief Stores the given batch-node.
     *
     * @param[in] batch
     *   The batch to store.
     * @param[in,out] batchNode
     *   The XML-node where the data is stored to.
     **/
    void saveBatch(
      Media::ConstBatchPtr batch,
      pugi::xml_node &batchNode);
};

}

#endif
