/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::Arinc665XmlSaveImpl.
 **/

#ifndef ARINC665_UTILS_ARINC665XMLSAVEIMPL_HPP
#define ARINC665_UTILS_ARINC665XMLSAVEIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc645/Arinc645.hpp>

#include <libxml++/libxml++.h>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Media Sets XML File Exporter.
 **/
class Arinc665XmlSaveImpl final
{
  public:
    /**
     * @brief Constructs the Media Set XML Exporter
     *
     * @param[in] mediaSet
     *   Media Set Information.
     * @param[in] filePathMapping
     *   File Path Mapping
     *   (used to insert the correct source path attribute.)
     * @param[in] xmlFile
     *   ARINC 665 XML file.
     **/
    Arinc665XmlSaveImpl(
      const Media::MediaSet &mediaSet,
      const FilePathMapping &filePathMapping,
      const std::filesystem::path &xmlFile ) noexcept;

    /**
     * @brief Saves the given Media Set information to the given XML file.
     *
     * @throw Arinc665Exception
     *   When XML file cannot be written.
     **/
    void operator()();

  private:
    /**
     * @brief Exports the Media Set section.
     *
     * @param[in,out] mediaSetElement
     *   XML Element where to store media set
     **/
    void saveMediaSet( xmlpp::Element &mediaSetElement );

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

    //! Media Set to export
    const Media::MediaSet &mediaSetV;
    //! File path mappings.
    const FilePathMapping &filePathMappingV;
    //! XML File Path
    const std::filesystem::path &xmlFileV;
    //! Deferred Load Loading Info
    std::list< std::tuple< const xmlpp::Element *, Media::LoadPtr > >
      deferredLoadInfoV{};
    //! Deferred Batch Loading Info
    std::list< std::tuple< const xmlpp::Element *, Media::BatchPtr > >
      deferredBatchInfoV{};
};

}

#endif
