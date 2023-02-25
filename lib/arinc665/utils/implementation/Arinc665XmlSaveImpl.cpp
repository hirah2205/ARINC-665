/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::Arinc665XmlSaveImpl.
 **/

#include "Arinc665XmlSaveImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValueTypeDescription.hpp>

#include <helper/Exception.hpp>

#include <fmt/format.h>

#include <boost/exception/all.hpp>

namespace Arinc665::Utils {

/**
 * @brief Converts std::string_view to GLib string.
 *
 * @param[in] str
 *   String
 *
 * @return GLib String.
 **/
static Glib::ustring toGlibString( std::string_view str );

Arinc665XmlSaveImpl::Arinc665XmlSaveImpl(
  const Media::MediaSet &mediaSet,
  const FilePathMapping &filePathMapping,
  const std::filesystem::path &xmlFile ) noexcept :
  mediaSetV{ mediaSet },
  filePathMappingV{ filePathMapping },
  xmlFileV{ xmlFile }
{
}

void Arinc665XmlSaveImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Save Media Set " << mediaSetV.partNumber() << " to " << xmlFileV;

  try
  {
    xmlpp::Document xmlDoc{};
    auto mediaSetNode{ xmlDoc.create_root_node( "MediaSet" ) };

    mediaSet( *mediaSetNode );

    xmlDoc.write_to_file_formatted( xmlFileV.string() );
  }
  catch ( const xmlpp::exception &e )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ e.what() }
      << boost::errinfo_file_name{ xmlFileV.string() } );
  }
}

void Arinc665XmlSaveImpl::mediaSet( xmlpp::Element &mediaSetElement ) const
{
  mediaSetElement.set_attribute( "PartNumber", mediaSetV.partNumber().data() );

  // Media Set Check Value
  checkValue(
    mediaSetElement,
    "MediaSetCheckValue",
    mediaSetV.mediaSetCheckValueType() );

  // List of Files Check Value
  checkValue(
    mediaSetElement,
    "ListOfFilesCheckValue",
    mediaSetV.listOfFilesCheckValueType() );

  // List of Loads Check Value
  checkValue(
    mediaSetElement,
    "ListOfLoadsCheckValue",
    mediaSetV.listOfLoadsCheckValueType() );

  // List of Batches Check Value
  checkValue(
    mediaSetElement,
    "ListOfBatchesCheckValue",
    mediaSetV.listOfBatchesCheckValueType() );

  // Files Check Value
  checkValue(
    mediaSetElement,
    "FilesCheckValue",
    mediaSetV.filesCheckValueType() );

  // Files List User Defined Data
  if (
    const auto &filesUserDefinedData{ mediaSetV.filesUserDefinedData() };
    !filesUserDefinedData.empty() )
  {
    mediaSetElement.add_child( "FilesUserDefinedData" )->add_child_text(
      std::string{ filesUserDefinedData.begin(), filesUserDefinedData.end() } );
  }

  // List of Loads User Defined Data
  if (
    const auto &loadsUserDefinedData{ mediaSetV.loadsUserDefinedData() };
    !loadsUserDefinedData.empty() )
  {
    mediaSetElement.add_child( "LoadsUserDefinedData" )->add_child_text(
      std::string{ loadsUserDefinedData.begin(), loadsUserDefinedData.end() } );
  }

  // List of Batches User Defined Data
  if (
    const auto &batchesUserDefinedData{ mediaSetV.batchesUserDefinedData() };
    !batchesUserDefinedData.empty() )
  {
    mediaSetElement.add_child( "BatchesUserDefinedData" )->add_child_text(
      std::string{
        batchesUserDefinedData.begin(),
        batchesUserDefinedData.end() } );
  }

  // Content
  auto contentNode{ mediaSetElement.add_child( "Content" ) };
  entries( mediaSetV, *contentNode );
}

void Arinc665XmlSaveImpl::entries(
  const Media::ContainerEntity &currentContainer,
  xmlpp::Element &currentContainerElement ) const
{
  // set default medium if provided
  if ( const auto defaultMedium{ currentContainer.defaultMediumNumber() };
    defaultMedium )
  {
    currentContainerElement.set_attribute(
      "DefaultMedium",
      std::to_string( static_cast< uint8_t >( *defaultMedium )) );
  }

  // iterate over subdirectories within container and add them recursively
  for ( const auto &dirEntry : currentContainer.subdirectories() )
  {
    auto directoryNode{ currentContainerElement.add_child( "Directory" ) };

    directoryNode->set_attribute( "Name", toGlibString( dirEntry->name() ) );

    entries( *dirEntry, *directoryNode );
  }

  // iterate over files within current container
  for ( const auto &fileEntry : currentContainer.files() )
  {
    switch ( fileEntry->fileType() )
    {
      using enum Media::FileType;

      case RegularFile:
        regularFile( fileEntry, currentContainerElement );
        break;

      case LoadFile:
        load( fileEntry, currentContainerElement );
        break;

      case BatchFile:
        batch( fileEntry, currentContainerElement );
        break;

      default:
        // should never ever happen
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid file type" } );
    }
  }
}

void Arinc665XmlSaveImpl::regularFile(
  const Media::ConstFilePtr &file,
  xmlpp::Element &parentElement ) const
{
  xmlpp::Element * const fileElement{ parentElement.add_child( "File" ) };
  assert( nullptr != fileElement );
  baseFile( file, *fileElement );
}

void Arinc665XmlSaveImpl::load(
  const Media::ConstFilePtr &file,
  xmlpp::Element &parentElement ) const
{
  xmlpp::Element * const loadElement{ parentElement.add_child( "Load" ) };
  assert( nullptr != loadElement );
  baseFile( file, *loadElement );

  const auto load{ std::dynamic_pointer_cast< const Media::Load >( file ) };
  assert( load );

  loadElement->set_attribute( "PartNumber", load->partNumber().data() );

  loadElement->set_attribute(
    "PartFlags",
    fmt::format( "0x{:04X}", load->partFlags() ) );

  // Optional Load Type (Description + Type Value)
  if ( const auto &loadType{ load->loadType() }; loadType )
  {
    const auto &[ description, id ]{ *loadType };
    loadElement->set_attribute( "Description", description );
    loadElement->set_attribute(
      "Type",
      fmt::format( "0x{:04X}", id ) );
  }

  // Load Check Value
  checkValue( *loadElement, "LoadCheckValue", load->loadCheckValueType() );

  // Data Files Check Value
  checkValue(
    *loadElement,
    "DataFilesCheckValue",
    load->dataFilesCheckValueType() );

  // Support Files Check Value
  checkValue(
    *loadElement,
    "SupportFilesCheckValue",
    load->supportFilesCheckValueType() );

  // iterate over target hardware
  for ( const auto &[ targetHardwareId, positions ] :
    load->targetHardwareIdPositions() )
  {
    auto * const targetHardwareElement{
      loadElement->add_child( "TargetHardware" ) };
    targetHardwareElement->set_attribute( "ThwId", targetHardwareId );

    for( const auto &position : positions )
    {
      auto positionElement{ targetHardwareElement->add_child( "Position" ) };
      positionElement->set_attribute( "Pos", position );
    }
  }

  // data files
  loadFiles( load->dataFiles(), "DataFile", *loadElement );

  // support files
  loadFiles( load->supportFiles(), "SupportFile", *loadElement );

  if (
    const auto &userDefinedData{ load->userDefinedData() };
    !userDefinedData.empty() )
  {
    loadElement->add_child( "UserDefinedData" )->add_child_text(
      std::string{ userDefinedData.begin(), userDefinedData.end() } );
  }
}

void Arinc665XmlSaveImpl::loadFiles(
  const Media::ConstLoadFiles &files,
  std::string_view fileElementName,
  xmlpp::Element &loadElement ) const
{
  const auto fileElementNameStr{ toGlibString( fileElementName ) };

  // iterate over files
  for ( const auto &[ file, partNumber, checkValueType ] : files )
  {
    auto * const fileElement{ loadElement.add_child( fileElementNameStr ) };
    fileElement->set_attribute( "FilePath", file->path().string() );
    fileElement->set_attribute( "PartNumber", partNumber );

    if ( checkValueType )
    {
      fileElement->set_attribute(
        "CheckValue",
        toGlibString(
          Arinc645::CheckValueTypeDescription::instance().name(
            *checkValueType ) ) );
    }
  }
}

void Arinc665XmlSaveImpl::batch(
  const Media::ConstFilePtr &file,
  xmlpp::Element &parentElement ) const
{
  xmlpp::Element * const batchElement{ parentElement.add_child( "Batch" ) };
  assert( nullptr != batchElement );
  baseFile( file, *batchElement );

  const auto batch{ std::dynamic_pointer_cast< const Media::Batch >( file ) };
  assert( batch );

  batchElement->set_attribute( "PartNumber", batch->partNumber().data() );

  // set optional comment
  if ( const auto comment{ batch->comment() }; !comment.empty() )
  {
    batchElement->set_attribute( "Comment", comment.data() );
  }

  // Iterate over batch information
  for ( const auto &[ thwIdPos, loads ] : batch->targets() )
  {
    auto targetElement{ batchElement->add_child( "Target" ) };

    targetElement->set_attribute( "ThwIdPos", thwIdPos );

    // iterate over loads
    for ( const auto &load : loads )
    {
      auto loadNode{ targetElement->add_child( "Load" ) };

      loadNode->set_attribute( "FilePath", load->path().string() );
    }
  }
}

void Arinc665XmlSaveImpl::baseFile(
  const Media::ConstFilePtr &file,
  xmlpp::Element &fileElement ) const
{
  // Add name attribute
  fileElement.set_attribute( "Name", toGlibString( file->name() ) );

  // Check Value Type
  if ( const auto checkValue{ file->checkValueType() }; checkValue )
  {
    fileElement.set_attribute(
      "CheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // Add source path attribute (optional)
  if (
    auto filePathIt{ filePathMappingV.find( file ) };
    filePathIt != filePathMappingV.end() )
  {
    fileElement.set_attribute( "SourcePath", filePathIt->second.string() );
  }

  // Add medium if provided
  if ( const auto mediumNumber{ file->mediumNumber() };
       mediumNumber )
  {
    fileElement.set_attribute(
      "Medium",
      std::to_string( static_cast< uint8_t >( *mediumNumber )) );
  }
}

void Arinc665XmlSaveImpl::checkValue(
  xmlpp::Element &element,
  std::string_view attribute,
  std::optional< Arinc645::CheckValueType > checkValue ) const
{
  if ( checkValue )
  {
    element.set_attribute(
      toGlibString( attribute ),
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }
}

static Glib::ustring toGlibString( std::string_view str )
{
  return Glib::ustring{ str.data(), str.size() };
}

}
