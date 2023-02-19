/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::Arinc665XmlImpl.
 **/

#include "Arinc665XmlImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValueTypeDescription.hpp>

#include <helper/SafeCast.hpp>

#include <fmt/format.h>

namespace Arinc665::Utils {

/**
 * @brief Converts GLib string to std::string_view.
 *
 * @param[in] str
 *   GLib String.
 *
 * @return string view of @p str.
 **/
static std::string_view toStringView( const Glib::ustring &str );

/**
 * @brief Converts std::string_view to GLib string.
 *
 * @param[in] str
 *   String
 *
 * @return GLib String.
 **/
static Glib::ustring toGlibString( std::string_view str );

/**
 * @brief Returns the Common Name attribute for directories and files.
 *
 * @param[in] element
 *   XML Element
 *
 * @return Content of the name attribute.
 **/
static std::string name( const xmlpp::Element &element );

/**
 * @brief Return Common Medium attribute for files
 *
 * @param[in] element
 *   XML Element
 *
 * @return Content of the Medium attriubte.
 * @retval {}
 *   If Medium is not set.
 **/
static OptionalMediumNumber mediumNumber( const xmlpp::Element &element );

static Media::FileType fileType( const xmlpp::Element &element );

Arinc665XmlImpl::LoadXmlResult Arinc665XmlImpl::loadFromXml(
  const std::filesystem::path &xmlFile )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Load Media Set from " << xmlFile;

  // Check existence of file
  if ( !std::filesystem::is_regular_file( xmlFile ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "XML File does not exist" }
      << boost::errinfo_file_name{ xmlFile } );
  }

  try
  {
    xmlpp::DomParser parser{};
    parser.parse_file( xmlFile.string() );
    if ( !parser )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Error Parsing File" }
        << boost::errinfo_file_name{ xmlFile.string() } );
    }

    auto mediaSetElement{ parser.get_document()->get_root_node() };
    if ( ( nullptr == mediaSetElement )
      || ( "MediaSet" != mediaSetElement->get_name() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "MediaSet XML Element not found" }
        << boost::errinfo_file_name{ xmlFile.string() } );
    }

    return loadMediaSet( *mediaSetElement );
  }
  catch ( const xmlpp::exception &e )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ e.what() }
      << boost::errinfo_file_name{ xmlFile.string() } );
  }
}

void Arinc665XmlImpl::saveToXml(
  const Media::MediaSet &mediaSet,
  const FilePathMapping &filePathMapping,
  const std::filesystem::path &xmlFile )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Save Media Set " << mediaSet.partNumber() << " to " << xmlFile;

  try
  {
    filePathMappingV = filePathMapping;

    xmlpp::Document xmlDoc{};
    auto mediaSetNode{ xmlDoc.create_root_node( "MediaSet" ) };

    saveMediaSet( mediaSet, *mediaSetNode );

    xmlDoc.write_to_file_formatted( xmlFile.string() );
  }
  catch ( const xmlpp::exception &e )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ e.what() }
      << boost::errinfo_file_name{ xmlFile.string() } );
  }
}

Arinc665XmlImpl::LoadXmlResult Arinc665XmlImpl::loadMediaSet(
  const xmlpp::Element &mediaSetElement )
{
  auto mediaSet{ std::make_shared< Media::MediaSet>() };

  filePathMappingV.clear();
  deferredLoadInfoV.clear();
  deferredBatchInfoV.clear();

  // Part Number
  const auto partNumber{ mediaSetElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "'PartNumber' attribute missing" }
      << boost::errinfo_at_line{ mediaSetElement.get_line() } );
  }
  mediaSet->partNumber( partNumber );

  // Media Set Check Value
  if (
    const auto checkValue{
      loadCheckValue( mediaSetElement, "MediaSetCheckValue" ) };
    checkValue )
  {
    mediaSet->mediaSetCheckValueType( checkValue );
  }

  // List of Files Check Value
  if (
    const auto checkValue{
      loadCheckValue( mediaSetElement, "ListOfFilesCheckValue" ) };
    checkValue )
  {
    mediaSet->listOfFilesCheckValueType( checkValue );
  }

  // List of Loads Check Value
  if (
    const auto checkValue{
      loadCheckValue( mediaSetElement, "ListOfLoadsCheckValue" ) };
    checkValue )
  {
    mediaSet->listOfLoadsCheckValueType( checkValue );
  }

  // List of Batches Check Value
  if (
    const auto checkValue{
      loadCheckValue( mediaSetElement, "ListOfBatchesCheckValue" ) };
    checkValue )
  {
    mediaSet->listOfBatchesCheckValueType( checkValue );
  }

  // Files Check Value
  if (
    const auto checkValue{
      loadCheckValue( mediaSetElement, "FilesCheckValue" ) };
    checkValue )
  {
    mediaSet->filesCheckValueType( checkValue );
  }

  if (
    const auto filesUserDefinedDataNode{ dynamic_cast< const xmlpp::Element*>(
      mediaSetElement.get_first_child( "FilesUserDefinedData" ) ) };
    nullptr != filesUserDefinedDataNode )
  {
    const auto userDefinedData{
      filesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->filesUserDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  if (
    const auto loadsUserDefinedDataNode{ dynamic_cast< const xmlpp::Element *>(
      mediaSetElement.get_first_child( "LoadsUserDefinedData" ) ) };
    nullptr != loadsUserDefinedDataNode )
  {
    const auto userDefinedData{
      loadsUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->loadsUserDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  if (
    const auto batchesUserDefinedDataNode{ dynamic_cast< const xmlpp::Element*>(
      mediaSetElement.get_first_child( "BatchesUserDefinedData") ) };
    nullptr != batchesUserDefinedDataNode )
  {
    const auto userDefinedData{
      batchesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->batchesUserDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  // content node
  const auto contentNode{ dynamic_cast< const xmlpp::Element * >(
    mediaSetElement.get_first_child( "Content" ) ) };
  if ( nullptr == contentNode )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "No 'Content' element given" }
      << boost::errinfo_at_line{ mediaSetElement.get_line() } );
  }

  // load files
  loadEntries( *contentNode, *mediaSet );

#if 1
  // deferred loading of loads and batches
  for ( const auto &[ loadElement, load ] : deferredLoadInfoV )
  {
    loadLoadDeferred( *loadElement, *load );
  }
  for ( const auto &[ batchElement, batch ] : deferredBatchInfoV )
  {
    loadBatchDeferred( *batchElement, *batch );
  }
#endif

  return std::make_tuple( std::move( mediaSet ), std::move( filePathMappingV ) );
}

void Arinc665XmlImpl::saveMediaSet(
  const Media::MediaSet &mediaSet,
  xmlpp::Element &mediaSetElement )
{
  mediaSetElement.set_attribute( "PartNumber", mediaSet.partNumber().data() );

  // Media Set Check Value
  saveCheckValue(
    mediaSetElement,
    "MediaSetCheckValue",
    mediaSet.mediaSetCheckValueType() );

  // List of Files Check Value
  saveCheckValue(
    mediaSetElement,
    "ListOfFilesCheckValue",
    mediaSet.listOfFilesCheckValueType() );

  // List of Loads Check Value
  saveCheckValue(
    mediaSetElement,
    "ListOfLoadsCheckValue",
    mediaSet.listOfLoadsCheckValueType() );

  // List of Batches Check Value
  saveCheckValue(
    mediaSetElement,
    "ListOfBatchesCheckValue",
    mediaSet.listOfBatchesCheckValueType() );

  // Files Check Value
  saveCheckValue(
    mediaSetElement,
    "FilesCheckValue",
    mediaSet.filesCheckValueType() );

  // Files List User Defined Data
  if (
    const auto &filesUserDefinedData{ mediaSet.filesUserDefinedData() };
    !filesUserDefinedData.empty() )
  {
    mediaSetElement.add_child( "FilesUserDefinedData" )->add_child_text(
      std::string{ filesUserDefinedData.begin(), filesUserDefinedData.end() } );
  }

  // List of Loads User Defined Data
  if (
    const auto &loadsUserDefinedData{ mediaSet.loadsUserDefinedData() };
    !loadsUserDefinedData.empty() )
  {
    mediaSetElement.add_child( "LoadsUserDefinedData" )->add_child_text(
      std::string{ loadsUserDefinedData.begin(), loadsUserDefinedData.end() } );
  }

  // List of Batches User Defined Data
  if (
    const auto &batchesUserDefinedData{ mediaSet.batchesUserDefinedData() };
    !batchesUserDefinedData.empty() )
  {
    mediaSetElement.add_child( "BatchesUserDefinedData" )->add_child_text(
      std::string{
        batchesUserDefinedData.begin(),
        batchesUserDefinedData.end() } );
  }

  // Content
  auto contentNode{ mediaSetElement.add_child( "Content" ) };
  saveEntries( mediaSet, *contentNode );
}

void Arinc665XmlImpl::loadEntries(
  const xmlpp::Element &currentElement,
  Media::ContainerEntity &current )
{
  // Common Default Medium attribute for directories and Contents root
  if ( const auto defaultMedium{
    currentElement.get_attribute_value( "DefaultMedium" ) };
    !defaultMedium.empty() )
  {
    const auto defaultMediumValue{ std::stoull( defaultMedium ) };

    if ( !std::in_range< uint8_t >( defaultMediumValue ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid DefaultMedium value" }
        << boost::errinfo_at_line{ currentElement.get_line() } );
    }

    current.defaultMediumNumber(
      MediumNumber{ static_cast< uint8_t >( defaultMediumValue ) } );
  }

  // iterate over all XML nodes
  for ( auto entryNode : currentElement.get_children() )
  {
    using namespace std::string_literals;

    auto entryElement{ dynamic_cast< xmlpp::Element*>( entryNode ) };

    if ( nullptr == entryElement )
    {
       // skip non-elements
      continue;
    }

    // add subdirectory and add content recursively
    if ( entryNode->get_name() == "Directory"s )
    {
      auto directory{ current.addSubdirectory( name( *entryElement ) ) };
      loadEntries( *entryElement, *directory );
      continue;
    }

    switch ( fileType( *entryElement ) )
    {
      using enum Arinc665::Media::FileType;

      case RegularFile:
        loadRegularFile( *entryElement, current );
        break;

      case LoadFile:
        loadLoad( *entryElement, current );
        break;

      case BatchFile:
        loadBatch( *entryElement, current );
        break;

      default:
        break;
    }
  }
}

void Arinc665XmlImpl::saveEntries(
  const Media::ContainerEntity &current,
  xmlpp::Element &currentElement )
{
  // set default medium if provided
  if ( const auto defaultMedium{ current.defaultMediumNumber() };
    defaultMedium )
  {
    currentElement.set_attribute(
      "DefaultMedium",
      std::to_string( static_cast< uint8_t >( *defaultMedium )) );
  }

  // iterate over subdirectories within container and add them recursively
  for ( const auto &dirEntry : current.subdirectories() )
  {
    auto directoryNode{ currentElement.add_child( "Directory" ) };

    directoryNode->set_attribute( "Name", toGlibString( dirEntry->name() ) );

    saveEntries( *dirEntry, *directoryNode );
  }

  // iterate over files within current container
  for ( const auto &fileEntry : current.files() )
  {
    switch ( fileEntry->fileType() )
    {
      using enum Media::FileType;

      case RegularFile:
        saveRegularFile( fileEntry, currentElement );
        break;

      case LoadFile:
        saveLoad( fileEntry, currentElement );
        break;

      case BatchFile:
        saveBatch( fileEntry, currentElement );
        break;

      default:
        // should never ever happen
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid file type" } );
    }
  }
}

void Arinc665XmlImpl::loadRegularFile(
  const xmlpp::Element &fileElement,
  Media::ContainerEntity &parent )
{
  auto file{
    parent.addRegularFile( name( fileElement ), mediumNumber( fileElement ) ) };

  loadBaseFile( fileElement, file );
}

void Arinc665XmlImpl::saveRegularFile(
  const Media::ConstFilePtr &file,
  xmlpp::Element &parentElement )
{
  xmlpp::Element * const fileElement{ parentElement.add_child( "File" ) };
  assert( nullptr != fileElement );
  saveBaseFile( file, *fileElement );
}

void Arinc665XmlImpl::loadLoad(
  const xmlpp::Element &loadElement,
  Media::ContainerEntity &current )
{
  auto load{
    current.addLoad( name( loadElement ), mediumNumber( loadElement ) ) };

  loadBaseFile( loadElement, load );

  // Part Number
  auto partNumber{ loadElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }
  load->partNumber( std::move( partNumber ) );

  // Part Flags
  if (
    const auto partFlags{ loadElement.get_attribute_value( "PartFlags" ) };
    !partFlags.empty() )
  {
    uint16_t partFlagsValue{
      Helper::safeCast< uint16_t >( std::stoul( partFlags, nullptr, 0 ) ) };

    load->partFlags( partFlagsValue );
  }

  // Load Type (Description + Type Value)
  if (
    const auto &[ description, type ]{ std::make_tuple(
      loadElement.get_attribute_value( "Description" ),
      loadElement.get_attribute_value( "Type" ) ) };
    !type.empty() && !description.empty() )
  {
    const uint16_t typeValue{
      Helper::safeCast< uint16_t >( std::stoul( type, nullptr, 0 ) ) };

    load->loadType( { std::make_pair( description, typeValue ) } );
  }

  Media::Load::TargetHardwareIdPositions thwIds{};

  // iterate over target hardware
  for ( auto targetHardwareNode : loadElement.get_children( "TargetHardware" ) )
  {
    auto targetHardwareElement{
      dynamic_cast< xmlpp::Element*>( targetHardwareNode ) };

    if ( nullptr == targetHardwareElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "TargetHardware Element invalid" }
        << boost::errinfo_at_line{ targetHardwareNode->get_line() } );
    }

    auto thwId{ targetHardwareElement->get_attribute_value( "ThwId" ) };

    Media::Load::Positions positions{};

    // iterate over positions
    for ( auto positionNode : targetHardwareElement->get_children( "Position" ) )
    {
      auto positionElement{ dynamic_cast< xmlpp::Element*>( positionNode ) };

      if ( nullptr == positionElement )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Position Element invalid" }
          << boost::errinfo_at_line{ positionNode->get_line() } );
      }

      auto position{ positionElement->get_attribute_value( "Pos" ) };

      positions.emplace( position );
    }

    thwIds.try_emplace( thwId, std::move( positions ) );
  }

  load->targetHardwareIdPositions( std::move( thwIds ) );

  //  handle data and support files later in defferd loading

  if (
    const auto userDefinedDataElement{ dynamic_cast< const xmlpp::Element*>(
      loadElement.get_first_child( "UserDefinedData" ) ) };
    nullptr != userDefinedDataElement )
  {
    const auto userDefinedData{
      userDefinedDataElement->get_child_text()->get_content() };

    load->userDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  // Load Check Value
  if (
    const auto checkValue{ loadCheckValue( loadElement, "LoadCheckValue" ) };
    checkValue )
  {
    load->loadCheckValueType( checkValue );
  }

  // Data Files Check Value (optional)
  if (
    const auto checkValue{ loadCheckValue( loadElement, "DataFilesCheckValue" ) };
    checkValue )
  {
    load->dataFilesCheckValueType( checkValue );
  }

  // Support Files Check Value (optional)
  if (
    const auto checkValue{
      loadCheckValue( loadElement, "SupportFilesCheckValue" ) };
    checkValue )
  {
    load->supportFilesCheckValueType( checkValue );
  }

  // add load to deferred load list
  deferredLoadInfoV.emplace_back( &loadElement, load );
}

void Arinc665XmlImpl::loadLoadDeferred(
  const xmlpp::Element &loadElement,
  Media::Load &load )
{
  const auto &mediaSet{ *load.mediaSet() };

  // data files
  load.dataFiles( loadLoadFiles( loadElement, "DataFile", mediaSet ) );

  // support files
  load.supportFiles( loadLoadFiles( loadElement, "SupportFile", mediaSet ) );
}

void Arinc665XmlImpl::saveLoad(
  const Media::ConstFilePtr &file,
  xmlpp::Element &parentElement ) const
{
  xmlpp::Element * const loadElement{ parentElement.add_child( "Load" ) };
  assert( nullptr != loadElement );
  saveBaseFile( file, *loadElement );

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
  saveCheckValue(
    *loadElement,
    "LoadCheckValue",
    load->loadCheckValueType() );

  // Data Files Check Value
  saveCheckValue(
    *loadElement,
    "DataFilesCheckValue",
    load->dataFilesCheckValueType() );

  // Support Files Check Value
  saveCheckValue(
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
  saveLoadFiles( load->dataFiles(), "DataFile", *loadElement );

  // support files
  saveLoadFiles( load->supportFiles(), "SupportFile", *loadElement );

  if (
    const auto &userDefinedData{ load->userDefinedData() };
    !userDefinedData.empty() )
  {
    loadElement->add_child( "UserDefinedData" )->add_child_text(
      std::string{ userDefinedData.begin(), userDefinedData.end() } );
  }
}

Media::ConstLoadFiles Arinc665XmlImpl::loadLoadFiles(
  const xmlpp::Element &loadElement,
  std::string_view fileElementName,
  const Media::MediaSet &mediaSet ) const
{
  Media::ConstLoadFiles loadFiles{};

  // iterate over files
  for ( auto fileNode :
    loadElement.get_children( toGlibString( fileElementName ) ) )
  {
    auto fileElement{ dynamic_cast< xmlpp::Element*>( fileNode ) };

    if ( nullptr == fileElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "File Element invalid" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    const auto filePath{ fileElement->get_attribute_value( "FilePath" ) };

    if ( filePath.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "'FilePath' attribute missing or empty" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    const auto filePartNumber{ fileElement->get_attribute_value( "PartNumber" ) };

    if ( filePartNumber.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "'PartNumber' attribute missing or empty" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    //  Check Value
    auto checkValueType{ loadCheckValue( *fileElement, "CheckValue" ) };

    // Find File
    auto file{ mediaSet.regularFile(
      std::filesystem::path{ filePath.raw(), std::locale{} } ) };

    if ( !file )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "'FilePath' attribute does not reference file" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    loadFiles.emplace_back( file, filePartNumber, checkValueType );
  }

  return loadFiles;
}

void Arinc665XmlImpl::saveLoadFiles(
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

void Arinc665XmlImpl::loadBatch(
  const xmlpp::Element &batchElement,
  Media::ContainerEntity &current )
{
  auto batch{
    current.addBatch( name( batchElement ), mediumNumber( batchElement ) ) };

  loadBaseFile( batchElement, batch );

  auto partNumber{ batchElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }
  batch->partNumber( std::move( partNumber ) );

  auto comment{ batchElement.get_attribute_value( "Comment" ) };
  batch->comment( std::move( comment ) );

  // handle batch load file handling in deferred batch loading

  // add load to deferred load list
  deferredBatchInfoV.emplace_back( &batchElement, batch );
}

void Arinc665XmlImpl::loadBatchDeferred(
  const xmlpp::Element &batchElement,
  Media::Batch &batch )
{
  // iterate over targets
  for ( auto targetNode : batchElement.get_children( "Target" ) )
  {
    auto targetElement{ dynamic_cast< xmlpp::Element* >( targetNode ) };
    if ( nullptr == targetElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Target Element" }
        << boost::errinfo_at_line{ targetNode->get_line() } );
    }

    const auto thwIdPos{ targetElement->get_attribute_value( "ThwIdPos" ) };

    Media::ConstLoads targetLoads{};

    // iterate over loads
    for ( auto loadNode : targetNode->get_children( "Load" ) )
    {
      auto loadElement{ dynamic_cast< xmlpp::Element* >( loadNode ) };
      if ( nullptr == loadElement)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Node is not an Element" }
          << boost::errinfo_at_line{ loadNode->get_line() } );
      }

      const auto loadFilePath{ loadElement->get_attribute_value( "FilePath" ) };

      if ( loadFilePath.empty())
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "FilePath attribute missing or empty" }
          << boost::errinfo_at_line{ loadElement->get_line() } );
      }

      auto load{ batch.mediaSet()->load(
        std::filesystem::path{ loadFilePath.raw(), std::locale{} } ) };

      if ( !load )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "FilePath attribute does not reference load" }
          << boost::errinfo_at_line{ loadElement->get_line() } );
      }

      targetLoads.push_back( load );
    }

    // add THW ID POS with Loads
    batch.target( thwIdPos, targetLoads );
  }
}

void Arinc665XmlImpl::saveBatch(
  const Media::ConstFilePtr &file,
  xmlpp::Element &parentElement ) const
{
  xmlpp::Element * const batchElement{ parentElement.add_child( "Batch" ) };
  assert( nullptr != batchElement );
  saveBaseFile( file, *batchElement );

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

void Arinc665XmlImpl::loadBaseFile(
  const xmlpp::Element &fileElement,
  const Media::FilePtr &file )
{
  // File Check Value
  const auto checkValue{ loadCheckValue( fileElement, "CheckValue" ) };

  file->checkValueType( checkValue );

  // common source path attribute for files
  const auto sourcePath{ fileElement.get_attribute_value( "SourcePath" ) };

  // set source path if attribute is present
  if ( !sourcePath.empty() )
  {
    filePathMappingV.try_emplace( file, toStringView( sourcePath ) );
  }
}

void Arinc665XmlImpl::saveBaseFile(
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

std::optional< Arinc645::CheckValueType > Arinc665XmlImpl::loadCheckValue(
  const xmlpp::Element &element,
  std::string_view attribute ) const
{
  if (
    const auto checkValueString{
      element.get_attribute_value( toGlibString( attribute ) ) };
    !checkValueString.empty() )
  {
    const auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( checkValueString ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ element.get_line() } );
    }

    return checkValue;
  }

  return {};
}

void Arinc665XmlImpl::saveCheckValue(
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

static std::string_view toStringView( const Glib::ustring &str )
{
  return std::string_view{ str.data(), str.length() };
}

static Glib::ustring toGlibString( std::string_view str )
{
  return Glib::ustring{ str.data(), str.size() };
}

static std::string name( const xmlpp::Element &element )
{
  const auto name{ element.get_attribute_value( "Name" ) };

  if ( name.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "'Name' attribute missing or empty" }
      << boost::errinfo_at_line{ element.get_line() } );
  }

  return name;
}

static OptionalMediumNumber mediumNumber( const xmlpp::Element &element )
{
  if (
    const auto medium{ element.get_attribute_value( "Medium" ) };
    !medium.empty() )
  {
    const auto mediumValue{ std::stoull( medium ) };

    if ( !std::in_range< uint8_t >( mediumValue ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid 'Medium' attribute value" }
        << boost::errinfo_at_line{ element.get_line() } );
    }

    return MediumNumber{ static_cast< uint8_t >( mediumValue ) };
  }

  return {};
}

static Media::FileType fileType( const xmlpp::Element &element )
{
  using namespace std::string_literals;

  if ( element.get_name() == "File"s )
  {
    return Media::FileType::RegularFile;
  }

  if ( element.get_name() == "Load"s )
  {
    return Media::FileType::LoadFile;
  }

  if ( element.get_name() == "Batch"s )
  {
    return Media::FileType::BatchFile;
  }

  BOOST_THROW_EXCEPTION( Arinc665Exception()
    << Helper::AdditionalInfo{ "Invalid XML Element" }
    << boost::errinfo_at_line{ element.get_line() }
    << boost::errinfo_type_info_name{ element.get_name() } );
}

}
