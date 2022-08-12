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

    saveMediaSet( mediaSet, filePathMapping, *mediaSetNode );

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
  const auto partNumber{ mediaSetElement.get_attribute_value( "PartNumber" ) };
  const auto mediaSetCheckValue{ mediaSetElement.get_attribute_value( "MediaSetCheckValue" ) };
  const auto listOfFilesCheckValue{ mediaSetElement.get_attribute_value( "ListOfFilesCheckValue" ) };
  const auto listOfLoadsCheckValue{ mediaSetElement.get_attribute_value( "ListOfLoadsCheckValue" ) };
  const auto listOfBatchesCheckValue{ mediaSetElement.get_attribute_value( "ListOfBatchesCheckValue" ) };
  const auto filesCheckValue{ mediaSetElement.get_attribute_value( "FilesCheckValue" ) };

  auto mediaSet{ std::make_shared< Media::MediaSet>() };
  mediaSet->partNumber( partNumber );

  // Media Set Check Value
  if ( !mediaSetCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( mediaSetCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ mediaSetElement.get_line() } );
    }

    mediaSet->mediaSetCheckValueType( checkValue );
  }

  // List of Files Check Value
  if ( !listOfFilesCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( listOfFilesCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ mediaSetElement.get_line() } );
    }

    mediaSet->listOfFilesCheckValueType( checkValue );
  }

  // List of Loads Check Value
  if ( !listOfLoadsCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( listOfLoadsCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ mediaSetElement.get_line() } );
    }

    mediaSet->listOfLoadsCheckValueType( checkValue );
  }

  // List of Batches Check Value
  if ( !listOfBatchesCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( listOfBatchesCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ mediaSetElement.get_line() } );
    }

    mediaSet->listOfBatchesCheckValueType( checkValue );
  }

  // Files Check Value
  if ( !filesCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( filesCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ mediaSetElement.get_line() } );
    }

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
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  if (
    const auto loadsUserDefinedDataNode{ dynamic_cast< const xmlpp::Element *>(
      mediaSetElement.get_first_child( "LoadsUserDefinedData" ) ) };
    nullptr != loadsUserDefinedDataNode )
  {
    const auto userDefinedData{
      loadsUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->loadsUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  if (
    const auto batchesUserDefinedDataNode{ dynamic_cast< const xmlpp::Element*>(
      mediaSetElement.get_first_child( "BatchesUserDefinedData") ) };
    nullptr != batchesUserDefinedDataNode )
  {
    const auto userDefinedData{
      batchesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->batchesUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  // iterate over media
  auto mediaNodes{ mediaSetElement.get_children( "Medium" ) };
  if ( mediaNodes.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Medium XML Elements missing" } );
  }

  FilePathMapping filePathMapping{};
  for ( auto mediumNode : mediaNodes )
  {
    auto medium{ mediaSet->addMedium() };

    loadEntries( medium, filePathMapping, *mediumNode );
  }

  // handle Loads
  auto loadsNode{ mediaSetElement.get_first_child( "Loads" ) };

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

  for ( auto loadNode : loadNodes )
  {
    auto loadElement{ dynamic_cast< xmlpp::Element*>( loadNode ) };
    if ( nullptr == loadElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Load XML Element" } );
    }

    loadLoad( mediaSet, *loadElement );
  }

  // handle Batches (optional)
  if (
    auto batchesNode{ mediaSetElement.get_first_child( "Batches" ) };
    nullptr != batchesNode )
  {
    // iterate over loads
    for ( auto batchNode : batchesNode->get_children( "Batch" ) )
    {
      auto batchElement{ dynamic_cast< xmlpp::Element * >( batchNode ) };
      if ( nullptr == batchElement )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid Batch XML Element" } );
      }

      loadBatch( mediaSet, *batchElement );
    }
  }

  return std::make_tuple( std::move( mediaSet ), std::move( filePathMapping ) );
}

void Arinc665XmlImpl::saveMediaSet(
  const Media::ConstMediaSetPtr& mediaSet,
  const FilePathMapping &filePathMapping,
  xmlpp::Element &mediaSetNode )
{
  mediaSetNode.set_attribute( "PartNumber", mediaSet->partNumber().data() );

  // Media Set Check Value
  if ( auto checkValue{ mediaSet->mediaSetCheckValueType() }; checkValue )
  {
    mediaSetNode.set_attribute(
      "MediaSetCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // List of Files Check Value
  if ( auto checkValue{ mediaSet->listOfFilesCheckValueType() }; checkValue )
  {
    mediaSetNode.set_attribute(
      "ListOfFilesCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // List of Loads Check Value
  if ( auto checkValue{ mediaSet->listOfLoadsCheckValueType() }; checkValue )
  {
    mediaSetNode.set_attribute(
      "ListOfLoadsCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // List of Batches Check Value
  if ( auto checkValue{ mediaSet->listOfBatchesCheckValueType() }; checkValue )
  {
    mediaSetNode.set_attribute(
      "ListOfBatchesCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // Files Check Value
  if ( auto checkValue{ mediaSet->filesCheckValueType() }; checkValue )
  {
    mediaSetNode.set_attribute(
      "FilesCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // Files List User Defined Data
  if (
    const auto &filesUserDefinedData{ mediaSet->filesUserDefinedData() };
    !filesUserDefinedData.empty() )
  {
    mediaSetNode.add_child( "FilesUserDefinedData" )->add_child_text(
      std::string{ filesUserDefinedData.begin(), filesUserDefinedData.end() } );
  }

  // List of Loads User Defined Data
  if (
    const auto &loadsUserDefinedData{ mediaSet->loadsUserDefinedData() };
    !loadsUserDefinedData.empty() )
  {
    mediaSetNode.add_child( "LoadsUserDefinedData" )->add_child_text(
      std::string{ loadsUserDefinedData.begin(), loadsUserDefinedData.end() } );
  }

  // List of Batches User Defined Data
  if (
    const auto &batchesUserDefinedData{ mediaSet->batchesUserDefinedData() };
    !batchesUserDefinedData.empty() )
  {
    mediaSetNode.add_child( "BatchesUserDefinedData" )->add_child_text(
      std::string{
        batchesUserDefinedData.begin(),
        batchesUserDefinedData.end() } );
  }

  // iterate over media
  for (
    uint8_t mediumNumber = 1U;
    mediumNumber <= mediaSet->numberOfMedia();
    ++mediumNumber )
  {
    auto medium{ mediaSet->medium( mediumNumber ) };

    auto mediumNode{ mediaSetNode.add_child( "Medium" ) };

    saveEntries( medium, filePathMapping, *mediumNode );
  }

  // handle Loads
  const auto loadsElement{ mediaSetNode.add_child( "Loads" ) };

  // iterate over loads
  for ( const auto &load : mediaSet->loads() )
  {
    const auto loadElement{ loadsElement->add_child( "Load" ) };
    saveLoad( load, *loadElement );
  }

  // handle Batches
  const auto batchesElement{ mediaSetNode.add_child( "Batches" ) };

  // iterate over batches
  for ( const auto &batch : mediaSet->batches() )
  {
    const auto batchElement{ batchesElement->add_child( "Batch" ) };
    saveBatch( batch, *batchElement );
  }
}

void Arinc665XmlImpl::loadEntries(
  const Media::ContainerEntityPtr& current,
  FilePathMapping &filePathMapping,
  const xmlpp::Node &currentNode )
{
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

    // iterate recursively over directories
    if ( entryNode->get_name() == "Directory"s )
    {
      auto directory{ current->addSubDirectory( static_cast< std::string>( name ) ) };

      loadEntries( directory, filePathMapping, *entryElement );

      continue;
    }

    // Check Value
    const auto checkValueAttr{ entryElement->get_attribute_value( "CheckValue" ) };
    // common source path attribute for files
    const auto sourcePath{ entryElement->get_attribute_value( "SourcePath" ) };

    std::optional< Arinc645::CheckValueType > checkValue{};

    // File Check Value
    if ( !checkValueAttr.empty() )
    {
      checkValue = Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( checkValueAttr ) );

      if ( Arinc645::CheckValueType::Invalid == checkValue )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid Check Value" }
          << boost::errinfo_at_line{ entryNode->get_line() } );
      }
    }

    if ( entryNode->get_name() == "File"s )
    {
      auto file{ current->addRegularFile( toStringView( name ) ) };

      file->checkValueType( checkValue );

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.insert( { file, toStringView( sourcePath ) } );
      }

      continue;
    }

    if ( entryNode->get_name() == "LoadFile"s )
    {
      auto load{ current->addLoad( toStringView( name ) ) };

      load->checkValueType( checkValue );

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.insert( { load, toStringView( sourcePath ) } );
      }

      continue;
    }

    if ( entryNode->get_name() == "BatchFile"s )
    {
      auto batch{ current->addBatch( toStringView( name ) ) };

      batch->checkValueType( checkValue );

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.insert( { batch, toStringView( sourcePath ) } );
      }

      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
      << "Ignore element " << entryNode->get_name();
  }
}

void Arinc665XmlImpl::saveEntries(
  const Media::ConstContainerEntityPtr &current,
  const FilePathMapping &filePathMapping,
  xmlpp::Node &currentNode )
{
  // iterate over subdirectories within container
  for ( const auto &dirEntry : current->subDirectories() )
  {
    auto directoryNode{ currentNode.add_child( "Directory" ) };

    directoryNode->set_attribute( "Name", toGlibString( dirEntry->name() ) );

    saveEntries( dirEntry, filePathMapping, *directoryNode );
  }

  // iterate over files within container
  for ( const auto &fileEntry : current->files( false ) )
  {
    xmlpp::Element * fileNode{};

    switch ( fileEntry->fileType() )
    {
      case Media::File::FileType::RegularFile:
        fileNode = currentNode.add_child( "File" );
        break;

      case Media::File::FileType::LoadFile:
      {
        fileNode = currentNode.add_child( "LoadFile" );
        break;
      }

      case Media::File::FileType::BatchFile:
      {
        fileNode = currentNode.add_child( "BatchFile" );
        break;
      }

      default:
        // should not happen
        continue;
    }

    assert( nullptr != fileNode );

    // Add name attribute
    fileNode->set_attribute( "Name", toGlibString( fileEntry->name() ) );

    // Check Value Type
    if ( auto checkValue{ fileEntry->checkValueType() }; checkValue )
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
  const Media::MediaSetPtr &mediaSet,
  const xmlpp::Element &loadElement )
{
  const auto nameRef{ loadElement.get_attribute_value( "NameRef" ) };
  const auto partNumber{ loadElement.get_attribute_value( "PartNumber" ) };
  const auto partFlags{ loadElement.get_attribute_value( "PartFlags" ) };
  const auto description{ loadElement.get_attribute_value( "Description" ) };
  const auto type{ loadElement.get_attribute_value( "Type" ) };
  const auto loadCheckValue{ loadElement.get_attribute_value( "LoadCheckValue" ) };
  const auto dataFilesCheckValue{ loadElement.get_attribute_value( "DataFilesCheckValue" ) };
  const auto supportFilesCheckValue{ loadElement.get_attribute_value( "SupportFilesCheckValue" ) };

  if ( nameRef.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }

  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }

  auto load{ mediaSet->load( toStringView( nameRef ) ) };

  if ( !load )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "NameRef attribute does not reference load" }
      << boost::errinfo_at_line{ loadElement.get_line() } );
  }

  load->partNumber( toStringView( partNumber ) );

  // Part Flags
  if ( !partFlags.empty() )
  {
    uint16_t partFlagsValue{ 0U };
    partFlagsValue= Helper::safeCast< uint16_t >( std::stoul( partFlags, nullptr, 0 ) );

    load->partFlags( partFlagsValue );
  }

  // Load Type (Description + Type Value)

  // try to decode type value and description if present
  if ( !type.empty() && !description.empty() )
  {
    uint16_t typeValue{ 0U };
    typeValue= Helper::safeCast< uint16_t >( std::stoul( type, nullptr, 0 ) );

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

    thwIds.emplace( thwId, std::move( positions ) );
  }

  load->targetHardwareIdPositions( std::move( thwIds ) );

  // iterate over data files
  for ( auto dataFileNode : loadElement.get_children( "DataFile" ) )
  {
    auto dataFileElement{ dynamic_cast< xmlpp::Element*>( dataFileNode ) };

    if ( nullptr == dataFileElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "DataFile Element invalid" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    const auto fileNameRef{ dataFileElement->get_attribute_value( "NameRef" ) };

    if ( fileNameRef.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    const auto filePartNumber{ dataFileElement->get_attribute_value( "PartNumber" ) };

    if ( filePartNumber.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    //  Check Value
    std::optional< Arinc645::CheckValueType > checkValueType{};
    if (
      const auto checkValueAttr{
        dataFileElement->get_attribute_value( "CheckValue" ) };
      !checkValueAttr.empty() )
    {
      auto checkValue{
        Arinc645::CheckValueTypeDescription::instance().enumeration(
          static_cast< std::string >( checkValueAttr ) ) };

      if ( Arinc645::CheckValueType::Invalid == checkValue )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid Check Value" }
          << boost::errinfo_at_line{ dataFileNode->get_line() } );
      }

      checkValueType = checkValue;
    }

    auto file{ mediaSet->file( toStringView( fileNameRef ) ) };

    if ( !file )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute does not reference file" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    load->dataFile( file, filePartNumber, std::move( checkValueType ) );
  }

  // iterate over support files
  for ( auto supportFileNode : loadElement.get_children( "SupportFile" ) )
  {
    auto supportFileElement{ dynamic_cast< xmlpp::Element*>( supportFileNode ) };

    if ( nullptr == supportFileElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "SupportFile Element invalid" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    auto fileNameRef{ supportFileElement->get_attribute_value( "NameRef" ) };

    if ( fileNameRef.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    const auto filePartNumber{ supportFileElement->get_attribute_value( "PartNumber" ) };

    if ( filePartNumber.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    //  Check Value
    std::optional< Arinc645::CheckValueType > checkValueType{};

    if (
      const auto checkValueAttr{
        supportFileElement->get_attribute_value( "CheckValue" ) };
      !checkValueAttr.empty() )
    {
      auto checkValue{
        Arinc645::CheckValueTypeDescription::instance().enumeration(
          static_cast< std::string >( checkValueAttr ) ) };

      if ( Arinc645::CheckValueType::Invalid == checkValue )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Invalid Check Value" }
          << boost::errinfo_at_line{ supportFileElement->get_line() } );
      }

      checkValueType = checkValue;
    }

    auto file{ mediaSet->file( toStringView( fileNameRef ) ) };

    if ( !file )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute does not reference file" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    load->supportFile( file, filePartNumber, std::move( checkValueType ) );
  }

  if (
    const auto userDefinedDataElement{ dynamic_cast< const xmlpp::Element*>(
      loadElement.get_first_child( "UserDefinedData" ) ) };
    nullptr != userDefinedDataElement )
  {
    const auto userDefinedData{
      userDefinedDataElement->get_child_text()->get_content() };

    load->userDefinedData(
      Arinc665::Media::Load::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  // Load Check Value
  if ( !loadCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( loadCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ loadElement.get_line() } );
    }

    load->loadCheckValueType( checkValue );
  }

  // Data Files Check Value
  if ( !dataFilesCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( dataFilesCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ loadElement.get_line() } );
    }

    load->dataFilesCheckValueType( checkValue );
  }

  // Support Files Check Value
  if ( !supportFilesCheckValue.empty() )
  {
    auto checkValue{
      Arinc645::CheckValueTypeDescription::instance().enumeration(
        static_cast< std::string >( supportFilesCheckValue ) ) };

    if ( Arinc645::CheckValueType::Invalid == checkValue )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Check Value" }
        << boost::errinfo_at_line{ loadElement.get_line() } );
    }

    load->supportFilesCheckValueType( checkValue );
  }
}

void Arinc665XmlImpl::saveLoad(
  const Media::ConstLoadPtr &load,
  xmlpp::Element &loadElement )
{
  loadElement.set_attribute( "NameRef", load->name().data() );

  loadElement.set_attribute( "PartNumber", load->partNumber().data() );

  loadElement.set_attribute(
    "PartFlags",
    fmt::format( "0x{:04X}", load->partFlags() ) );

  // Optional Load Type (Description + Type Value)
  if ( const auto &loadType{ load->loadType() }; loadType )
  {
    const auto &[ description, id ]{ *loadType };
    loadElement.set_attribute( "Description", description );
    loadElement.set_attribute(
      "Type",
      fmt::format( "0x{:04X}", id ) );
  }

  // Load Check Value
  if ( auto checkValue{ load->loadCheckValueType() }; checkValue )
  {
    loadElement.set_attribute(
      "LoadCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // Data Files Check Value
  if ( auto checkValue{ load->dataFilesCheckValueType() }; checkValue )
  {
    loadElement.set_attribute(
      "DataFilesCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // Support Files Check Value
  if ( auto checkValue{ load->supportFilesCheckValueType() }; checkValue )
  {
    loadElement.set_attribute(
      "SupportFilesCheckValue",
      toGlibString(
        Arinc645::CheckValueTypeDescription::instance().name( *checkValue ) ) );
  }

  // iterate over target hardware
  for ( const auto &[targetHardwareId, positions] :
    load->targetHardwareIdPositions() )
  {
    auto * const targetHardwareElement{ loadElement.add_child( "TargetHardware" ) };
    targetHardwareElement->set_attribute( "ThwId", targetHardwareId );

    for( const auto &position : positions )
    {
      auto positionElement{ targetHardwareElement->add_child( "Position" ) };
      positionElement->set_attribute( "Pos", position );
    }
  }

  // iterate over data files
  for ( const auto &[ dataFile, partNumber, checkValueType ] : load->dataFiles() )
  {
    auto * const dataFileElement{ loadElement.add_child( "DataFile" ) };
    dataFileElement->set_attribute( "NameRef", dataFile->name().data() );
    dataFileElement->set_attribute( "PartNumber", partNumber );
    if ( checkValueType )
    {
      dataFileElement->set_attribute(
        "CheckValue",
        toGlibString(
          Arinc645::CheckValueTypeDescription::instance().name(
            *checkValueType ) ) );
    }
  }

  // iterate over support files
  for ( const auto &[ supportFile, partNumber, checkValueType ] : load->supportFiles() )
  {
    auto supportFileElement{ loadElement.add_child( "SupportFile" ) };
    supportFileElement->set_attribute(
      "NameRef",
      supportFile->name().data() );
    supportFileElement->set_attribute( "PartNumber", partNumber );
    if ( checkValueType )
    {
      supportFileElement->set_attribute(
        "CheckValue",
        toGlibString(
          Arinc645::CheckValueTypeDescription::instance().name(
            *checkValueType ) ) );
    }
  }

  if (
    const auto userDefinedData{ load->userDefinedData() };
    !userDefinedData.empty() )
  {
    loadElement.add_child( "UserDefinedData" )->add_child_text(
      std::string{ userDefinedData.begin(), userDefinedData.end() } );
  }
}

void Arinc665XmlImpl::loadBatch(
  const Media::MediaSetPtr &mediaSet,
  const xmlpp::Element &batchElement )
{
  const auto nameRef{ batchElement.get_attribute_value( "NameRef" ) };
  const auto partNumber{ batchElement.get_attribute_value( "PartNumber" ) };
  const auto comment{ batchElement.get_attribute_value( "Comment" ) };

  if ( nameRef.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  if ( partNumber.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  auto batch{ mediaSet->batch( toStringView( nameRef ) ) };

  if ( !batch )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "NameRef attribute does not reference batch" )
      << boost::errinfo_at_line{ batchElement.get_line() } );
  }

  batch->partNumber( toStringView( partNumber ) );
  batch->comment( comment );

  // iterate over targets
  for ( auto targetNode : batchElement.get_children( "Target" ) )
  {
    auto targetElement{ dynamic_cast< xmlpp::Element*>( targetNode ) };
    if ( nullptr == targetElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Target Element" }
        << boost::errinfo_at_line{ targetNode->get_line() } );
    }

    const auto thwIdPos{ targetElement->get_attribute_value( "ThwIdPos" ) };

    Media::Loads loads{};

    // iterate over loads
    for ( auto LoadNode : targetNode->get_children( "Load" ) )
    {
      auto loadElement{ dynamic_cast< xmlpp::Element*>( LoadNode ) };
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

      auto load{ mediaSet->load( toStringView( loadNameRef ) ) };

      if ( !load )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "NameRef attribute does not reference load" }
          << boost::errinfo_at_line{ loadElement->get_line() } );
      }

      loads.push_back( load);
    }

    // add load
    batch->target( toStringView( thwIdPos ), loads );
  }
}

void Arinc665XmlImpl::saveBatch(
  const Media::ConstBatchPtr &batch,
  xmlpp::Element &batchElement )
{
  batchElement.set_attribute( "NameRef", batch->name().data());

  batchElement.set_attribute( "PartNumber", batch->partNumber().data() );

  // set optional comment
  if ( !batch->comment().empty())
  {
    batchElement.set_attribute( "Comment", batch->comment().data() );
  }

  // Iterate over batch information
  for ( const auto &[thwIdPos,loads] : batch->targets() )
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

static std::string_view toStringView( const Glib::ustring &str )
{
  return std::string_view{ str.data(), str.length() };
}

static Glib::ustring toGlibString( std::string_view str )
{
  return Glib::ustring{ str.data(), str.size() };
}

}
