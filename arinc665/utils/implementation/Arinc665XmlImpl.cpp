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
#include <arinc665/media/Medium.hpp>
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
      << Helper::AdditionalInfo{ "XML File does not exist" } );
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
  Media::ConstMediaSetPtr mediaSet,
  const FilePathMapping &filePathMapping,
  const std::filesystem::path &xmlFile )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Save Media Set " << mediaSet->partNumber() << " to " << xmlFile;

  try
  {
    xmlpp::Document xmlDoc{};
    auto mediaSetNode{ xmlDoc.create_root_node( "MediaSet" ) };

    saveMediaSet( *mediaSet, filePathMapping, *mediaSetNode );

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

  // Part Number
  const auto partNumber{ mediaSetElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "No Part Number given" }
      << boost::errinfo_at_line{ mediaSetElement.get_line() } );
  }
  mediaSet->partNumber( partNumber );

  // Media Set Check Value
  if (
    const auto checkValue{ loadCheckValue( mediaSetElement, "MediaSetCheckValue" ) };
    checkValue )
  {
    mediaSet->mediaSetCheckValueType( checkValue );
  }

  // List of Files Check Value
  if (
    const auto checkValue{ loadCheckValue( mediaSetElement, "ListOfFilesCheckValue" ) };
    checkValue )
  {
    mediaSet->listOfFilesCheckValueType( checkValue );
  }

  // List of Loads Check Value
  if (
    const auto checkValue{ loadCheckValue( mediaSetElement, "ListOfLoadsCheckValue" ) };
    checkValue )
  {
    mediaSet->listOfLoadsCheckValueType( checkValue );
  }

  // List of Batches Check Value
  if (
    const auto checkValue{ loadCheckValue( mediaSetElement, "ListOfBatchesCheckValue" ) };
    checkValue )
  {
    mediaSet->listOfBatchesCheckValueType( checkValue );
  }

  // Files Check Value
  if (
    const auto checkValue{ loadCheckValue( mediaSetElement, "FilesCheckValue" ) };
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

  // iterate over media
  auto mediaNodes{ mediaSetElement.get_children( "Medium" ) };
  if ( mediaNodes.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Medium XML Elements missing" } );
  }

  // load files
  FilePathMapping filePathMapping{};
  for ( auto const * const mediumNode : mediaNodes )
  {
    auto medium{ mediaSet->addMedium() };

    loadEntries( *mediumNode, *medium, filePathMapping );
  }

  // handle Loads
  const auto loadsNode{ mediaSetElement.get_first_child( "Loads" ) };

  if ( nullptr == loadsNode )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Loads XML Element missing" } );
  }

  // iterate over loads
  auto loadNodes{ loadsNode->get_children( "Load" ) };

  if ( loadNodes.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Load XML Elements missing" } );
  }

  for ( const auto * const loadNode : loadNodes )
  {
    const auto loadElement{ dynamic_cast< const xmlpp::Element * >( loadNode ) };
    if ( nullptr == loadElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Load XML Element" } );
    }

    loadLoad( *loadElement, *mediaSet );
  }

  // handle Batches (optional)
  if (
    const auto batchesNode{ mediaSetElement.get_first_child( "Batches" ) };
    nullptr != batchesNode )
  {
    // iterate over loads
    for ( const auto * const batchNode : batchesNode->get_children( "Batch" ) )
    {
      const auto batchElement{ dynamic_cast< const xmlpp::Element * >( batchNode ) };
      if ( nullptr == batchElement )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid Batch XML Element" } );
      }

      loadBatch( *batchElement, *mediaSet );
    }
  }

  return std::make_tuple( std::move( mediaSet ), std::move( filePathMapping ) );
}

void Arinc665XmlImpl::saveMediaSet(
  const Media::MediaSet &mediaSet,
  const FilePathMapping &filePathMapping,
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

  // iterate over media
  for (
    uint8_t mediumNumber{ 1U };
    mediumNumber <= mediaSet.numberOfMedia();
    ++mediumNumber )
  {
    auto medium{ mediaSet.medium( mediumNumber ) };

    auto mediumNode{ mediaSetElement.add_child( "Medium" ) };

    saveEntries( *medium, filePathMapping, *mediumNode );
  }

  // handle Loads
  const auto loadsElement{ mediaSetElement.add_child( "Loads" ) };

  // iterate over loads
  for ( const auto &load : mediaSet.loads() )
  {
    const auto loadElement{ loadsElement->add_child( "Load" ) };
    saveLoad( *load, *loadElement );
  }

  // handle Batches
  const auto batchesElement{ mediaSetElement.add_child( "Batches" ) };

  // iterate over batches
  for ( const auto &batch : mediaSet.batches() )
  {
    const auto batchElement{ batchesElement->add_child( "Batch" ) };
    saveBatch( *batch, *batchElement );
  }
}

void Arinc665XmlImpl::loadEntries(
  const xmlpp::Node &currentNode,
  Media::ContainerEntity &current,
  FilePathMapping &filePathMapping )
{
  // iterate over all XML nodes
  for ( auto entryNode : currentNode.get_children() )
  {
    using namespace std::string_literals;

    auto entryElement{ dynamic_cast< xmlpp::Element*>( entryNode ) };

    if ( nullptr == entryElement )
    {
       // skip non-elements
      continue;
    }

    // Common Name attribute for directories and files
    const auto name{ entryElement->get_attribute_value( "Name" ) };

    if ( name.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Name attribute missing or empty" }
        << boost::errinfo_at_line{ currentNode.get_line() } );
    }

    // add subdirectory and add content recursively
    if ( entryNode->get_name() == "Directory"s )
    {
      auto directory{ current.addSubdirectory( static_cast< std::string>( name ) ) };

      loadEntries( *entryElement, *directory, filePathMapping );

      continue;
    }

    // File Check Value
    const auto checkValue{ loadCheckValue( *entryElement, "CheckValue" ) };

    // common source path attribute for files
    const auto sourcePath{ entryElement->get_attribute_value( "SourcePath" ) };

    if ( entryNode->get_name() == "File"s )
    {
      auto file{ current.addRegularFile( name ) };

      file->checkValueType( checkValue );

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.try_emplace( file, toStringView( sourcePath ) );
      }

      continue;
    }

    if ( entryNode->get_name() == "LoadFile"s )
    {
      auto load{ current.addLoad( name ) };

      load->checkValueType( checkValue );

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.try_emplace(  load, toStringView( sourcePath ) );
      }

      continue;
    }

    if ( entryNode->get_name() == "BatchFile"s )
    {
      auto batch{ current.addBatch( name ) };

      batch->checkValueType( checkValue );

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.try_emplace( batch, toStringView( sourcePath ) );
      }

      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
      << "Ignore element " << entryNode->get_name();
  }
}

void Arinc665XmlImpl::saveEntries(
  const Media::ContainerEntity &current,
  const FilePathMapping &filePathMapping,
  xmlpp::Node &currentNode )
{
  // iterate over subdirectories within container and add them recursively
  for ( const auto &dirEntry : current.subdirectories() )
  {
    auto directoryNode{ currentNode.add_child( "Directory" ) };

    directoryNode->set_attribute( "Name", toGlibString( dirEntry->name() ) );

    saveEntries( *dirEntry, filePathMapping, *directoryNode );
  }

  // iterate over files within current container
  for ( const auto &fileEntry : current.files() )
  {
    xmlpp::Element * fileNode;

    switch ( fileEntry->fileType() )
    {
      case Media::File::FileType::RegularFile:
        fileNode = currentNode.add_child( "File" );
        break;

      case Media::File::FileType::LoadFile:
        fileNode = currentNode.add_child( "LoadFile" );
        break;

      case Media::File::FileType::BatchFile:
        fileNode = currentNode.add_child( "BatchFile" );
        break;

      default:
        // should never ever happen
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid file type" } );
    }

    assert( nullptr != fileNode );

    // Add name attribute
    fileNode->set_attribute( "Name", toGlibString( fileEntry->name() ) );

    // Check Value Type
    if ( const auto checkValue{ fileEntry->checkValueType() }; checkValue )
    {
      fileNode->set_attribute(
        "CheckValue",
        toGlibString(
          Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
    }

    // Add source path attribute (optional)
    if (
      auto filePathIt{ filePathMapping.find( fileEntry ) };
      filePathIt != filePathMapping.end() )
    {
      fileNode->set_attribute( "SourcePath", filePathIt->second.string() );
    }
  }
}

void Arinc665XmlImpl::loadLoad(
  const xmlpp::Element &loadElement,
  Media::MediaSet &mediaSet ) const
{
  const auto nameRef{ loadElement.get_attribute_value( "NameRef" ) };
  if ( nameRef.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }

  const auto loads{ mediaSet.loads( toStringView( nameRef ) ) };

  if ( loads.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "NameRef attribute does not reference load" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }

  if ( loads.size() > 1U )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "More than 1 load with same name" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }

  // Part Number
  auto partNumber{ loadElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }
  loads.front()->partNumber( std::move( partNumber ) );

  // Part Flags
  if (
    const auto partFlags{ loadElement.get_attribute_value( "PartFlags" ) };
    !partFlags.empty() )
  {
    uint16_t partFlagsValue{
      Helper::safeCast< uint16_t >( std::stoul( partFlags, nullptr, 0 ) ) };

    loads.front()->partFlags( partFlagsValue );
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

    loads.front()->loadType( { std::make_pair( description, typeValue ) } );
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

  loads.front()->targetHardwareIdPositions( std::move( thwIds ) );

  // data files
  loads.front()->dataFiles( loadLoadFiles( loadElement, "DataFile", mediaSet ) );

  // support files
  loads.front()->supportFiles( loadLoadFiles( loadElement, "SupportFile", mediaSet ) );

  if (
    const auto userDefinedDataElement{ dynamic_cast< const xmlpp::Element*>(
      loadElement.get_first_child( "UserDefinedData" ) ) };
    nullptr != userDefinedDataElement )
  {
    const auto userDefinedData{
      userDefinedDataElement->get_child_text()->get_content() };

    loads.front()->userDefinedData(
      Media::UserDefinedData{ userDefinedData.begin(), userDefinedData.end() } );
  }

  // Load Check Value
  if (
    const auto checkValue{ loadCheckValue( loadElement, "LoadCheckValue" ) };
    checkValue )
  {
    loads.front()->loadCheckValueType( checkValue );
  }

  // Data Files Check Value
  if (
    const auto checkValue{ loadCheckValue( loadElement, "DataFilesCheckValue" ) };
    checkValue )
  {
    loads.front()->dataFilesCheckValueType( checkValue );
  }

  // Support Files Check Value
  if (
    const auto checkValue{ loadCheckValue( loadElement, "SupportFilesCheckValue" ) };
    checkValue )
  {
    loads.front()->supportFilesCheckValueType( checkValue );
  }
}

void Arinc665XmlImpl::saveLoad(
  const Media::Load &load,
  xmlpp::Element &loadElement ) const
{
  loadElement.set_attribute( "NameRef", load.name().data() );

  loadElement.set_attribute( "PartNumber", load.partNumber().data() );

  loadElement.set_attribute(
    "PartFlags",
    fmt::format( "0x{:04X}", load.partFlags() ) );

  // Optional Load Type (Description + Type Value)
  if ( const auto &loadType{ load.loadType() }; loadType )
  {
    const auto &[ description, id ]{ *loadType };
    loadElement.set_attribute( "Description", description );
    loadElement.set_attribute(
      "Type",
      fmt::format( "0x{:04X}", id ) );
  }

  // Load Check Value
  saveCheckValue( loadElement, "LoadCheckValue", load.loadCheckValueType() );

  // Data Files Check Value
  saveCheckValue( loadElement, "DataFilesCheckValue", load.dataFilesCheckValueType() );

  // Support Files Check Value
  saveCheckValue( loadElement, "SupportFilesCheckValue", load.supportFilesCheckValueType() );

  // iterate over target hardware
  for ( const auto &[ targetHardwareId, positions ] :
    load.targetHardwareIdPositions() )
  {
    auto * const targetHardwareElement{ loadElement.add_child( "TargetHardware" ) };
    targetHardwareElement->set_attribute( "ThwId", targetHardwareId );

    for( const auto &position : positions )
    {
      auto positionElement{ targetHardwareElement->add_child( "Position" ) };
      positionElement->set_attribute( "Pos", position );
    }
  }

  // data files
  saveLoadFiles( load.dataFiles(), "DataFile", loadElement );

  // support files
  saveLoadFiles( load.supportFiles(), "SupportFile", loadElement );

  if (
    const auto &userDefinedData{ load.userDefinedData() };
    !userDefinedData.empty() )
  {
    loadElement.add_child( "UserDefinedData" )->add_child_text(
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
  for ( auto fileNode : loadElement.get_children( toGlibString( fileElementName ) ) )
  {
    auto fileElement{ dynamic_cast< xmlpp::Element*>( fileNode ) };

    if ( nullptr == fileElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "File Element invalid" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    const auto fileNameRef{ fileElement->get_attribute_value( "NameRef" ) };

    if ( fileNameRef.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    const auto filePartNumber{ fileElement->get_attribute_value( "PartNumber" ) };

    if ( filePartNumber.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    //  Check Value
    auto checkValueType{ loadCheckValue( *fileElement, "CheckValue" ) };

    // Find File
    auto files{ mediaSet.regularFiles( toStringView( fileNameRef ) ) };

    if ( files.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute does not reference file" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    if ( files.size() > 1U )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "file name references more than one file" }
        << boost::errinfo_at_line{ fileNode->get_line() } );
    }

    loadFiles.emplace_back( files.front(), filePartNumber, checkValueType );
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
    fileElement->set_attribute( "NameRef", toGlibString( file->name() ) );
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
  Media::MediaSet &mediaSet ) const
{
  const auto nameRef{ batchElement.get_attribute_value( "NameRef" ) };
  if ( nameRef.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  auto partNumber{ batchElement.get_attribute_value( "PartNumber" ) };
  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  auto comment{ batchElement.get_attribute_value( "Comment" ) };

  auto batches{ mediaSet.batches( toStringView( nameRef ) ) };

  if ( batches.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "NameRef attribute does not reference batch" )
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  if ( batches.size() > 1U )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "NameRef attribute does reference more than one batch" )
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  batches.front()->partNumber( std::move( partNumber ) );
  batches.front()->comment( std::move( comment ) );

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
    for ( auto LoadNode : targetNode->get_children( "Load" ) )
    {
      auto loadElement{ dynamic_cast< xmlpp::Element* >( LoadNode ) };
      if ( nullptr == loadElement)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Node is not an Element" }
          << boost::errinfo_at_line{ LoadNode->get_line() } );
      }

      const auto loadNameRef{ loadElement->get_attribute_value( "NameRef" ) };

      if ( loadNameRef.empty())
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
          << boost::errinfo_at_line{ loadElement->get_line() } );
      }

      auto loads{ mediaSet.loads( toStringView( loadNameRef ) ) };

      if ( loads.empty() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "NameRef attribute does not reference load" }
          << boost::errinfo_at_line{ loadElement->get_line() } );
      }

      if ( loads.size() > 1U )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "NameRef attribute does reference more than one load" }
          << boost::errinfo_at_line{ loadElement->get_line() } );
      }

      targetLoads.push_back( loads.front() );
    }

    // add THW ID POS with Loads
    batches.front()->target( thwIdPos, targetLoads );
  }
}

void Arinc665XmlImpl::saveBatch(
  const Media::Batch &batch,
  xmlpp::Element &batchElement ) const
{
  batchElement.set_attribute( "NameRef", batch.name().data());

  batchElement.set_attribute( "PartNumber", batch.partNumber().data() );

  // set optional comment
  if ( const auto comment{ batch.comment() }; !comment.empty() )
  {
    batchElement.set_attribute( "Comment", comment.data() );
  }

  // Iterate over batch information
  for ( const auto &[ thwIdPos, loads ] : batch.targets() )
  {
    auto targetElement{ batchElement.add_child( "Target" ) };

    targetElement->set_attribute( "ThwIdPos", thwIdPos );

    // iterate over loads
    for ( const auto &load : loads )
    {
      auto loadNode{ targetElement->add_child( "Load" ) };

      loadNode->set_attribute( "NameRef", load->name().data() );
    }
  }
}

std::optional< Arinc645::CheckValueType > Arinc665XmlImpl::loadCheckValue(
  const xmlpp::Element &element,
  std::string_view attribute ) const
{
  if (
    const auto checkValueString{ element.get_attribute_value( toGlibString( attribute ) ) };
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

}
