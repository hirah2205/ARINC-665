/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::Arinc665XmlLoadImpl.
 **/

#include "Arinc665XmlLoadImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValueTypeDescription.hpp>

#include <helper/SafeCast.hpp>

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
 * @return Content of the Medium attribute.
 * @retval {}
 *   If Medium is not set.
 **/
static OptionalMediumNumber mediumNumber( const xmlpp::Element &element );

/**
 * Return file type of element.
 *
 * @param[in] element
 *   XML Element
 *
 * @return File Type
 **/
static Media::FileType fileType( const xmlpp::Element &element );

Arinc665XmlLoadImpl::Arinc665XmlLoadImpl(
  const std::filesystem::path &xmlFile ) noexcept :
  xmlFileV{ xmlFile }
{
}

LoadXmlResult Arinc665XmlLoadImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Load Media Set from " << xmlFileV;

  // Check existence of file
  if ( !std::filesystem::is_regular_file( xmlFileV ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "XML File does not exist" }
      << boost::errinfo_file_name{ xmlFileV } );
  }

  try
  {
    xmlpp::DomParser parser{};
    parser.parse_file( xmlFileV.string() );
    if ( !parser )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Error Parsing File" }
        << boost::errinfo_file_name{ xmlFileV.string() } );
    }

    auto mediaSetElement{ parser.get_document()->get_root_node() };
    if ( ( nullptr == mediaSetElement )
      || ( "MediaSet" != mediaSetElement->get_name() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "MediaSet XML Element not found" }
        << boost::errinfo_file_name{ xmlFileV.string() } );
    }

    mediaSetV = std::make_shared< Media::MediaSet>();
    filePathMappingV.clear();
    deferredLoadInfoV.clear();
    deferredBatchInfoV.clear();

    mediaSet( *mediaSetElement );

    return std::make_tuple( std::move( mediaSetV ), std::move( filePathMappingV ) );
  }
  catch ( const xmlpp::exception &e )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ e.what() }
      << boost::errinfo_file_name{ xmlFileV.string() } );
  }
}

void Arinc665XmlLoadImpl::mediaSet( const xmlpp::Element &mediaSetElement )
{
  // Part Number
  const auto partNumber{ mediaSetElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "'PartNumber' attribute missing" }
      << boost::errinfo_at_line{ mediaSetElement.get_line() } );
  }
  mediaSetV->partNumber( partNumber );

  // Media Set Check Value
  if (
    const auto mediaSetCheckValue{
      checkValue( mediaSetElement, "MediaSetCheckValue" ) };
    mediaSetCheckValue )
  {
    mediaSetV->mediaSetCheckValueType( mediaSetCheckValue );
  }

  // List of Files Check Value
  if (
    const auto listOfFilesCheckValue{
      checkValue( mediaSetElement, "ListOfFilesCheckValue" ) };
    listOfFilesCheckValue )
  {
    mediaSetV->listOfFilesCheckValueType( listOfFilesCheckValue );
  }

  // List of Loads Check Value
  if (
    const auto listOfLoadsCheckValue{
      checkValue( mediaSetElement, "ListOfLoadsCheckValue" ) };
    listOfLoadsCheckValue )
  {
    mediaSetV->listOfLoadsCheckValueType( listOfLoadsCheckValue );
  }

  // List of Batches Check Value
  if (
    const auto listOfBatchesCheckValue{
      checkValue( mediaSetElement, "ListOfBatchesCheckValue" ) };
    listOfBatchesCheckValue )
  {
    mediaSetV->listOfBatchesCheckValueType( listOfBatchesCheckValue );
  }

  // Files Check Value
  if (
    const auto filesCheckValue{
      checkValue( mediaSetElement, "FilesCheckValue" ) };
    filesCheckValue )
  {
    mediaSetV->filesCheckValueType( filesCheckValue );
  }

  if (
    const auto filesUserDefinedDataNode{ dynamic_cast< const xmlpp::Element*>(
      mediaSetElement.get_first_child( "FilesUserDefinedData" ) ) };
    nullptr != filesUserDefinedDataNode )
  {
    const auto userDefinedData{
      filesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSetV->filesUserDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  if (
    const auto loadsUserDefinedDataNode{ dynamic_cast< const xmlpp::Element *>(
      mediaSetElement.get_first_child( "LoadsUserDefinedData" ) ) };
    nullptr != loadsUserDefinedDataNode )
  {
    const auto userDefinedData{
      loadsUserDefinedDataNode->get_child_text()->get_content() };
    mediaSetV->loadsUserDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  if (
    const auto batchesUserDefinedDataNode{ dynamic_cast< const xmlpp::Element*>(
      mediaSetElement.get_first_child( "BatchesUserDefinedData") ) };
    nullptr != batchesUserDefinedDataNode )
  {
    const auto userDefinedData{
      batchesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSetV->batchesUserDefinedData(
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
  entries( *contentNode, *mediaSetV );

  // deferred loading of loads and batches
  for ( const auto &[ loadElement, load ] : deferredLoadInfoV )
  {
    loadDeferred( *loadElement, *load );
  }
  for ( const auto &[ batchElement, batch ] : deferredBatchInfoV )
  {
    loadBatchDeferred( *batchElement, *batch );
  }
}

void Arinc665XmlLoadImpl::entries(
  const xmlpp::Element &currentContainerElement,
  Media::ContainerEntity &currentContainer )
{
  // Common Default Medium attribute for directories and Contents root
  if ( const auto defaultMedium{
         currentContainerElement.get_attribute_value( "DefaultMedium" ) };
    !defaultMedium.empty() )
  {
    const auto defaultMediumValue{ std::stoull( defaultMedium ) };

    if ( !std::in_range< uint8_t >( defaultMediumValue ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid DefaultMedium value" }
        << boost::errinfo_at_line{ currentContainerElement.get_line() } );
    }

    currentContainer.defaultMediumNumber(
      MediumNumber{ static_cast< uint8_t >( defaultMediumValue ) } );
  }

  // iterate over all XML nodes
  for ( auto entryNode : currentContainerElement.get_children() )
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
      auto directory{
        currentContainer.addSubdirectory( name( *entryElement ) ) };
      entries( *entryElement, *directory );
      continue;
    }

    switch ( fileType( *entryElement ) )
    {
      using enum Arinc665::Media::FileType;

      case RegularFile:
        regularFile( *entryElement, currentContainer );
        break;

      case LoadFile:
        load( *entryElement, currentContainer );
        break;

      case BatchFile:
        batch( *entryElement, currentContainer );
        break;

      default:
        break;
    }
  }
}

void Arinc665XmlLoadImpl::regularFile(
  const xmlpp::Element &fileElement,
  Media::ContainerEntity &parent )
{
  auto file{
    parent.addRegularFile( name( fileElement ), mediumNumber( fileElement ) ) };

  loadBaseFile( fileElement, file );
}

void Arinc665XmlLoadImpl::load(
  const xmlpp::Element &loadElement,
  Media::ContainerEntity &parent )
{
  auto load{
    parent.addLoad( name( loadElement ), mediumNumber( loadElement ) ) };

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

  //  handle data and support files later in deferred loading

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
    const auto loadCheckValue{ checkValue( loadElement, "LoadCheckValue" ) };
    loadCheckValue )
  {
    load->loadCheckValueType( loadCheckValue );
  }

  // Data Files Check Value (optional)
  if (
    const auto dataFilesCheckValue{
      checkValue( loadElement, "DataFilesCheckValue" ) };
    dataFilesCheckValue )
  {
    load->dataFilesCheckValueType( dataFilesCheckValue );
  }

  // Support Files Check Value (optional)
  if (
    const auto supportFilesCheckValue{
      checkValue( loadElement, "SupportFilesCheckValue" ) };
    supportFilesCheckValue )
  {
    load->supportFilesCheckValueType( supportFilesCheckValue );
  }

  // add load to deferred load list
  deferredLoadInfoV.emplace_back( &loadElement, load );
}

void Arinc665XmlLoadImpl::loadDeferred(
  const xmlpp::Element &loadElement,
  Media::Load &load )
{
  const auto &mediaSet{ *load.mediaSet() };

  // data files
  load.dataFiles( loadFiles( loadElement, "DataFile", mediaSet ) );

  // support files
  load.supportFiles( loadFiles( loadElement, "SupportFile", mediaSet ) );
}

Media::ConstLoadFiles Arinc665XmlLoadImpl::loadFiles(
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
    auto checkValueType{ checkValue( *fileElement, "CheckValue" ) };

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

void Arinc665XmlLoadImpl::batch(
  const xmlpp::Element &batchElement,
  Media::ContainerEntity &parent )
{
  auto batch{
    parent.addBatch( name( batchElement ), mediumNumber( batchElement ) ) };

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

void Arinc665XmlLoadImpl::loadBatchDeferred(
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

void Arinc665XmlLoadImpl::loadBaseFile(
  const xmlpp::Element &fileElement,
  const Media::FilePtr &file )
{
  // File Check Value
  const auto fileCheckValue{ checkValue( fileElement, "CheckValue" ) };

  file->checkValueType( fileCheckValue );

  // common source path attribute for files
  const auto sourcePath{ fileElement.get_attribute_value( "SourcePath" ) };

  // set source path if attribute is present
  if ( !sourcePath.empty() )
  {
    filePathMappingV.try_emplace( file, toStringView( sourcePath ) );
  }
}

std::optional< Arinc645::CheckValueType > Arinc665XmlLoadImpl::checkValue(
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
