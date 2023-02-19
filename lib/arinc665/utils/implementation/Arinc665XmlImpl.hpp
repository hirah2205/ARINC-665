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

#ifndef ARINC665_UTILS_ARINC665XMLIMPL_HPP
#define ARINC665_UTILS_ARINC665XMLIMPL_HPP

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
    [[nodiscard]] LoadXmlResult loadFromXml(
      const std::filesystem::path &xmlFile ) override;

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
      const Media::MediaSet &mediaSet,
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
     * @param[in,out] mediaSetElement
     *   XML Element where to store media set
     **/
    void saveMediaSet(
      const Media::MediaSet &mediaSet,
      xmlpp::Element &mediaSetElement );

    /**
     * @brief Loads file entries.
     *
     * Loads all child elements (Files, Directories) for the given medium, or
     * directory.
     *
     * @param[in] currentElement
     *   XML Element representing the container (root or directory).
     * @param[in,out] current
     *   Current Medium, or Directory.
     *   Files and directories will be added to this.
     *
     * @throw Arinc665::Arinc665Exception
     *   When Name Attribute is missing or empty.
     **/
    void loadEntries(
      const xmlpp::Element &currentElement,
      Media::ContainerEntity &current );

    /**
     * @brief Saves file entries.
     *
     * @param[in] current
     *   Current medium or directory.
     * @param[in,out] currentElement
     *   XML Element, where to add content.
     */
    void saveEntries(
      const Media::ContainerEntity &current,
      xmlpp::Element &currentElement );

    /**
     * @brief Loads the Regular File.
     *
     * A regular file is added to the @p parent container and initialised to its
     * decoded values.
     *
     * @param[in] fileElement
     *   XML Element representing the file.
     * @param parent
     *   Owning parent Containter
     **/
    void loadRegularFile(
      const xmlpp::Element &fileElement,
      Media::ContainerEntity &parent );

    /**
     * @brief Saves the Regular File to the XML DOM.
     *
     * @param[in] file
     *   File
     * @param[in,out] parentElement
     *   Parent XML Element
     **/
    void saveRegularFile(
      const Media::ConstFilePtr &file,
      xmlpp::Element &parentElement );

    /**
     * @brief Loads the given Load-node.
     *
     * @param[in] loadElement
     *   XML-node, where the data is loaded from.
     * @param[in,out] current
     *   Current medium or directory.
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
      Media::ContainerEntity &current );

    /**
     * @brief Deferred Loading of Load Element
     *
     * The data and support files are added here to assoure that all possible
     * files has been added to the Media Set previously.
     *
     * @param[in] loadElement
     *   XML Element represetning the load.
     * @param[in,out] load
     *   Load
     **/
    void loadLoadDeferred(
      const xmlpp::Element &loadElement,
      Media::Load &load );

    /**
     * @brief Stores the given load-node.
     *
     * @param[in] file
     *   Load File to store.
     * @param[in,out] parentElement
     *   Parent XML Element
     **/
    void saveLoad(
      const Media::ConstFilePtr &file,
      xmlpp::Element &parentElement ) const;

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
     * @param[in,out] current
     *   Current medium or directory.
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
      Media::ContainerEntity &current );

    /**
     * @brief Deferred Loading of Batch Element
     *
     * The Target Hardware Information are added here to assoure that all
     * possible loads has been added to the Media Set previously.
     *
     * @param[in] batchElement
     *   XML Element represetning the batch.
     * @param[in,out] batch
     *   Load
     **/
    void loadBatchDeferred(
      const xmlpp::Element &batchElement,
      Media::Batch &batch );

    /**
     * @brief Stores the given batch-node.
     *
     * @param[in] file
     *   Batch to store.
     * @param[in,out] parentElement
     *   Parent XML Element
     **/
    void saveBatch(
      const Media::ConstFilePtr &file,
      xmlpp::Element &parentElement ) const;

    /**
     * @brief Load Base File Attributes.
     *
     * Handles attributes:
     *  - `CheckValue`, and
     *  - `SourcePath`.
     *
     * @param[in] fileElement
     *   XML Element
     * @param[in,out] file
     *   File
     **/
    void loadBaseFile(
      const xmlpp::Element &fileElement,
      const Media::FilePtr &file );

    /**
     * @brief Stores the Base File Attributes.
     *
     * @param[in] file
     *   File
     * @param[in,out] fileElement
     *   XML ELement
     **/
    void saveBaseFile(
      const Media::ConstFilePtr &file,
      xmlpp::Element &fileElement ) const;

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

    //! File path mappings.
    FilePathMapping filePathMappingV{};
    //! Deferred Load Loading Info
    std::list< std::tuple< const xmlpp::Element *, Media::LoadPtr > >
      deferredLoadInfoV{};
    //! Deferred Batch Loading Info
    std::list< std::tuple< const xmlpp::Element *, Media::BatchPtr > >
      deferredBatchInfoV{};
};

}

#endif
