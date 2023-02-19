/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::Arinc665XmlLoadImpl.
 **/

#ifndef ARINC665_UTILS_ARINC665XMLLOADIMPL_HPP
#define ARINC665_UTILS_ARINC665XMLLOADIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc645/Arinc645.hpp>

#include <libxml++/libxml++.h>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Media Sets XML File Importer.
 **/
class Arinc665XmlLoadImpl final
{
  public:
    /**
     * @brief Constructs the ARINC 665 Importer
     *
     * @param[in] xmlFile
     *   ARINC 665 XML file.
     **/
    Arinc665XmlLoadImpl( const std::filesystem::path &xmlFile ) noexcept;

    /**
     * @brief Loads the Media Set information from the given XML file.
     *
     * @return Loaded Media Set information.
     *
     * @throw Arinc665::Arinc665Exception
     *   When XML file does not exist.
     * @throw Arinc665::Arinc665Exception
     *   When Loading of XML fails.
     **/
    [[nodiscard]] LoadXmlResult operator()();

  private:
    /**
     * @brief Import the media set from the XML DOM.
     *
     * @param[in] mediaSetElement
     *   Media Set to Load.
     **/
    void mediaSet( const xmlpp::Element &mediaSetElement );

    /**
     * @brief Import Container from the XML DOM.
     *
     * Loads all child elements (Files, Directories) for the given medium, or
     * directory.
     *
     * @param[in] currentContainerElement
     *   XML Element representing the container (root or directory).
     * @param[in,out] currentContainer
     *   Current Medium, or Directory.
     *   Files and directories will be added to this.
     *
     * @throw Arinc665::Arinc665Exception
     *   When Name Attribute is missing or empty.
     **/
    void entries(
      const xmlpp::Element &currentContainerElement,
      Media::ContainerEntity &currentContainer );

    /**
     * @brief Import Regular File from the XML DOM.
     *
     * A regular file is added to the @p parent container and initialised to its
     * decoded values.
     *
     * @param[in] fileElement
     *   XML Element representing the file.
     * @param parent
     *   Owning parent Containter
     **/
    void regularFile(
      const xmlpp::Element &fileElement,
      Media::ContainerEntity &parent );

    /**
     * @brief Import Load from the XML DOM.
     *
     * @param[in] loadElement
     *   XML-node, where the data is loaded from.
     * @param[in,out] parent
     *   Owning parent Containter.
     *
     * @throw Arinc665::Arinc665Exception
     *   When NameRef attribute is missing or empty.
     * @throw Arinc665::Arinc665Exception
     *   When NameRef attribute does not reference a load.
     * @throw Arinc665::Arinc665Exception
     *   When PartNumber attribute is missing or empty.
     **/
    void load(
      const xmlpp::Element &loadElement,
      Media::ContainerEntity &parent );

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
    void loadDeferred(
      const xmlpp::Element &loadElement,
      Media::Load &load );

    /**
     * @brief Import Load Data/ Support Files from XML DOM.
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
    [[nodiscard]] Media::ConstLoadFiles loadFiles(
      const xmlpp::Element &loadElement,
      std::string_view fileElementName,
      const Media::MediaSet &mediaSet ) const;

    /**
     * @brief Import Batch from XML DOM.
     *
     * @param[in] batchElement
     *   XML-node, where the data is loaded from.
     * @param[in,out] parent
     *   Owning parent Containter.
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
    void batch(
      const xmlpp::Element &batchElement,
      Media::ContainerEntity &parent );

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
     * @brief Import Base File Attributes from the XML DOM.
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
    [[nodiscard]] std::optional< Arinc645::CheckValueType > checkValue(
      const xmlpp::Element &element,
      std::string_view attribute ) const;

    //! XML File path
    const std::filesystem::path &xmlFileV;
    //! Media Set
    Media::MediaSetPtr mediaSetV;
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
