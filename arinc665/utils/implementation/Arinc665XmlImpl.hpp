/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::Arinc665XmlImpl.
 **/

#ifndef ARINC665_UTILS_ARINC665XMLPUGIXMLIMPL_HPP
#define ARINC665_UTILS_ARINC665XMLPUGIXMLIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc645/Arinc645.hpp>

#include <libxml++/libxml++.h>

namespace Arinc665::Utils {

/**
 * @brief Handles the representation of Media Sets as XML file.
 **/
class Arinc665XmlImpl final : public Arinc665Xml
{
  public:
   //! Constructor
    Arinc665XmlImpl() = default;

    //! Destructor
    ~Arinc665XmlImpl() override = default;

    /**
     * @brief Loads the Media Set information from the given XML file.
     *
     * @param[in] xmlFile
     *   ARINC 665 XML file.
     *
     * @return Loaded Media Set information.
     *
     * @throw Arinc665::Arinc665Exception
     *   When XML file does not exist.
     * @throw Arinc665::Arinc665Exception
     *   When Loading of XML fails.
     **/
    LoadXmlResult loadFromXml( const std::filesystem::path &xmlFile ) final;

    /**
     * @brief Saves the given Media Set information to the given XML file.
     *
     * @param[in] mediaSet
     *   Media Set Information.
     * @param[in] filePathMapping
     *   File Path Mapping
     *   (used to insert the correct source path attribute.)
     * @param[in] xmlFile
     *   ARINC 665 XML file.
     *
     * @throw Arinc665Exception
     *   When XML file cannot be written.
     **/
    void saveToXml(
      Media::ConstMediaSetPtr mediaSet,
      const FilePathMapping &filePathMapping,
      const std::filesystem::path &xmlFile ) final;

  private:
    /**
     * @brief Loads the media set section.
     *
     * @todo Add Loads, and Batches handling.
     *
     * @param[in] mediaSetElement
     *   Media Set to Load.
     *
     * @return Representation of Media Set
     **/
    LoadXmlResult loadMediaSet( const xmlpp::Element &mediaSetElement );

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
      const Media::ConstMediaSetPtr &mediaSet,
      const FilePathMapping &filePathMapping,
      xmlpp::Element &mediaSetNode );

    /**
     * @brief Loads file entries.
     *
     * Loads all child elements (Files, Directories) for the given medium, or
     * directory.
     *
     * @param[in,out] current
     *   Current Medium, or Directory
     * @param[in,out] filePathMapping
     *   File path mapping
     * @param[in] currentNode
     *   XML node representing the directory.
     *
     * @throw Arinc665::Arinc665Exception
     *   When Name Attribute is missing or empty.
     **/
    void loadEntries(
      const Media::ContainerEntityPtr &current,
      FilePathMapping &filePathMapping,
      const xmlpp::Node &currentNode );

    /**
     * @brief Saves file entries.
     *
     * @param current
     * @param filePathMapping
     * @param currentNode
     */
    void saveEntries(
      const Media::ConstContainerEntityPtr &current,
      const FilePathMapping &filePathMapping,
      xmlpp::Node &currentNode );

    /**
     * @brief Loads the given load-node.
     *
     * @param[in,out] mediaSet
     *   The media set where the load is stored into.
     * @param[in] loadElement
     *   The XML-node, where the data is loaded from.
     *
     * @throw Arinc665::Arinc665Exception
     *   When NameRef attribute is missing or empty.
     * @throw Arinc665::Arinc665Exception
     *   When NameRef attribute does not reference a load.
     * @throw Arinc665::Arinc665Exception
     *   When PartNumber attribute is missing or empty.
     **/
    void loadLoad(
     const  Media::MediaSetPtr &mediaSet,
      const xmlpp::Element &loadElement );

    /**
     * @brief Stores the given load-node.
     *
     * @param[in] load
     *   The load to store.
     * @param[in,out] loadElement
     *   The XML-node where the data is stored to.
     **/
    void saveLoad(
      const Media::ConstLoadPtr &load,
      xmlpp::Element &loadElement );

    /**
     * @brief Loads the given batch-node.
     *
     * @param[in,out] mediaSet
     *   The media set where the load is stored into.
     * @param[in] batchElement
     *   The XML-node, where the data is loaded from.
     *
     * @throw Arinc665::Arinc665Exception
     *   When @p Batch element cannot be parsed i.e.:
     *     - When @p NameRef attribute is missing or empty.
     *     - When @p NameRef attribute does not reference a batch file.
     * @throw Arinc665::Arinc665Exception
     *   When @p Load elements cannot be parsed i.e.:
     *     - When @p NameRef attribute is missing or empty.
     *     - When @p NameRef attribute does not reference load.
     **/
    void loadBatch(
      const Media::MediaSetPtr &mediaSet,
      const xmlpp::Element &batchElement );

    /**
     * @brief Stores the given batch-node.
     *
     * @param[in] batch
     *   The batch to store.
     * @param[in,out] batchElement
     *   The XML-node where the data is stored to.
     **/
    void saveBatch(
      const Media::ConstBatchPtr &batch,
      xmlpp::Element &batchElement );
};

}

#endif
