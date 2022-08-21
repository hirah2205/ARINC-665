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
    LoadXmlResult loadFromXml( const std::filesystem::path &xmlFile ) override;

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
      const std::filesystem::path &xmlFile ) override;

  private:
    /**
     * @brief Loads the media set section.
     *
     * @param[in] mediaSetElement
     *   Media Set to Load.
     *
     * @return Representation of Media Set
     **/
    LoadXmlResult loadMediaSet( const xmlpp::Element &mediaSetElement );

    /**
     * @brief Saves the Media Set section.
     *
     * @param[in] mediaSet
     *   Media Set to store.
     * @param[in] filePathMapping
     *   File path mapping.
     * @param[in,out] mediaSetElement
     *   XML Element where to store media set
     **/
    void saveMediaSet(
      const Media::MediaSet &mediaSet,
      const FilePathMapping &filePathMapping,
      xmlpp::Element &mediaSetElement );

    /**
     * @brief Loads file entries.
     *
     * Loads all child elements (Files, Directories) for the given medium, or
     * directory.
     *
     * @param[in] currentNode
     *   XML node representing the directory.
     * @param[in,out] current
     *   Current Medium, or Directory.
     *   Files and directories will be added to this.
     * @param[in,out] filePathMapping
     *   File path mappings.
     *
     * @throw Arinc665::Arinc665Exception
     *   When Name Attribute is missing or empty.
     **/
    void loadEntries(
      const xmlpp::Node &currentNode,
      Media::ContainerEntity &current,
      FilePathMapping &filePathMapping );

    /**
     * @brief Saves file entries.
     *
     * @param[in] current
     *   Current medium or directory.
     * @param[in] filePathMapping
     *   File path mapping.
     * @param[in,out] currentNode
     *   XML Node, where to add content.
     */
    void saveEntries(
      const Media::ContainerEntity &current,
      const FilePathMapping &filePathMapping,
      xmlpp::Node &currentNode );

    /**
     * @brief Loads the given Load-node.
     *
     * @param[in] loadElement
     *   XML-node, where the data is loaded from.
     * @param[in,out] mediaSet
     *   Media Set where the load is stored into.
     *
     * @throw Arinc665::Arinc665Exception
     *   When NameRef attribute is missing or empty.
     * @throw Arinc665::Arinc665Exception
     *   When NameRef attribute does not reference a load.
     * @throw Arinc665::Arinc665Exception
     *   When PartNumber attribute is missing or empty.
     **/
    void loadLoad(
      const xmlpp::Element &loadElement,
      Media::MediaSet &mediaSet ) const;

    /**
     * @brief Stores the given load-node.
     *
     * @param[in] load
     *   Load to store.
     * @param[in,out] loadElement
     *   XML-node where the data is stored to.
     **/
    void saveLoad(
      const Media::Load &load,
      xmlpp::Element &loadElement ) const;

    /**
     * @brief Load Data/ Support Files.
     *
     * @param[in] loadElement
     *   XML Element Representing the Load
     * @param[in] fileElementName
     *   XML Files Element Name
     * @param[in] mediaSet
     *   Media Set
     *
     * @return List of Files.
     **/
    [[nodiscard]] Media::ConstLoadFiles loadLoadFiles(
      const xmlpp::Element &loadElement,
      std::string_view fileElementName,
      const Media::MediaSet &mediaSet ) const;

    /**
     * @brief Save Load Files Information to XML.
     *
     * @param[in] files
     *   Files to store
     * @param[in] fileElementName
     *   XML Element Name
     * @param[in,out] loadElement
     *   XML Load Element
     */
    void saveLoadFiles(
      const Media::ConstLoadFiles &files,
      std::string_view fileElementName,
      xmlpp::Element &loadElement ) const;

    /**
     * @brief Loads the given batch-node.
     *
     * @param[in] batchElement
     *   XML-node, where the data is loaded from.
     * @param[in,out] mediaSet
     *   Media set where the load is stored into.
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
      const xmlpp::Element &batchElement,
      Media::MediaSet &mediaSet ) const;

    /**
     * @brief Stores the given batch-node.
     *
     * @param[in] batch
     *   Batch to store.
     * @param[in,out] batchElement
     *   XML-node where the data is stored to.
     **/
    void saveBatch(
      const Media::Batch &batch,
      xmlpp::Element &batchElement ) const;

    /**
     * @brief Decodes the attribute as Check Value Type.
     *
     * @param[in] element
     *   XML Element
     * @param[in] attribute
     *   XML Attribute Name of Check Value
     *
     * @return Decoded Check Value Type.
     *
     * @throw Arinc665Exception
     *   When Attribute value is invalid.
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType > loadCheckValue(
      const xmlpp::Element &element,
      std::string_view attribute ) const;

    /**
     * @brief Encodes the Check Value and stores it as attribute.
     *
     * @param[in,out] element
     *   XML Element
     * @param[in] attribute
     *   XML Attribute Name of Check Value
     * @param[in] checkValue
     *   CheckValue
     **/
    void saveCheckValue(
      xmlpp::Element &element,
      std::string_view attribute,
      std::optional< Arinc645::CheckValueType > checkValue ) const;
};

}

#endif
