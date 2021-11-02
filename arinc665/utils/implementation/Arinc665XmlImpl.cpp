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
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/SafeCast.hpp>

#include <boost/format.hpp>

namespace Arinc665::Utils {

/**
 * @brief Converts GLib string to std::string_view
 *
 * @param[in] str
 *   GLib String.
 *
 * @return string view of @p str.
 **/
static std::string_view toStringView( const Glib::ustring &str );

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
    if (( nullptr == mediaSetElement )
      || ( "MediaSet" != mediaSetElement->get_name() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "MediaSet XML Element not found" }
        << boost::errinfo_file_name{ xmlFile.string() } );
    }

    return loadMediaSet( *mediaSetElement );
  }
  catch ( xmlpp::exception &e)
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
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Save Media Set " << mediaSet->partNumber() << " to " << xmlFile;

  try
  {
    xmlpp::Document xmlDoc{};
    auto mediaSetNode{ xmlDoc.create_root_node( "MediaSet" ) };

    saveMediaSet( mediaSet, filePathMapping, *mediaSetNode );

    xmlDoc.write_to_file_formatted( xmlFile.string() );
  }
  catch ( xmlpp::exception &e)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Error writing XML file" }
      << Helper::AdditionalInfo{ e.what() }
      << boost::errinfo_file_name{ xmlFile.string() } );
  }
}

Arinc665XmlImpl::LoadXmlResult Arinc665XmlImpl::loadMediaSet(
  const xmlpp::Element &mediaSetElement )
{
  const auto partNumber{ mediaSetElement.get_attribute_value( "PartNumber" ) };

  auto mediaSet{ std::make_shared< Media::MediaSet>() };
  mediaSet->partNumber( partNumber );

  const auto filesUserDefinedDataNode = dynamic_cast< const xmlpp::Element*>(
    mediaSetElement.get_first_child( "FilesUserDefinedData" ) );

  if ( nullptr != filesUserDefinedDataNode )
  {
    const auto userDefinedData{
      filesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->filesUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  const auto loadsUserDefinedDataNode = dynamic_cast< const xmlpp::Element *>(
    mediaSetElement.get_first_child( "LoadsUserDefinedData" ) );
  if ( nullptr != loadsUserDefinedDataNode )
  {
    const auto userDefinedData{
      loadsUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->loadsUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  const auto batchesUserDefinedDataNode = dynamic_cast< const xmlpp::Element*>(
      mediaSetElement.get_first_child( "BatchesUserDefinedData") );
  if ( nullptr != batchesUserDefinedDataNode )
  {
    const auto userDefinedData{
      batchesUserDefinedDataNode->get_child_text()->get_content() };
    mediaSet->batchesUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
  }

  // iterate over media
  FilePathMapping filePathMapping{};
  for ( auto mediumNode : mediaSetElement.get_children( "Medium" ) )
  {
    loadMedium( mediaSet, filePathMapping, *mediumNode );
  }

  // handle Loads
  auto loadsNode{ mediaSetElement.get_first_child( "Loads" ) };

  // iterate over loads
  for ( auto loadNode : loadsNode->get_children( "Load" ) )
  {
    auto loadElement{ dynamic_cast< xmlpp::Element*>( loadNode ) };
    if ( nullptr == loadElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Load XML Element" } );
    }

    loadLoad( mediaSet, *loadElement );
  }

  // handle Batches
  auto batchesNode{ mediaSetElement.get_first_child( "Batches" ) };

  // iterate over loads
  for ( auto batchNode : batchesNode->get_children( "Batch") )
  {
    auto batchElement{ dynamic_cast< xmlpp::Element*>( batchNode ) };
    if ( nullptr == batchElement )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Batch XML Element" } );
    }

    loadBatch( mediaSet, *batchElement );
  }

  return std::make_tuple( std::move( mediaSet ), std::move( filePathMapping ) );
}

void Arinc665XmlImpl::saveMediaSet(
  const Media::ConstMediaSetPtr& mediaSet,
  const FilePathMapping &filePathMapping,
  xmlpp::Element &mediaSetNode )
{
  mediaSetNode.set_attribute( "PartNumber", mediaSet->partNumber().data() );

  // Files List User Defined Data
  const auto filesUserDefinedData{ mediaSet->filesUserDefinedData()};
  if ( !filesUserDefinedData.empty() )
  {
    mediaSetNode.add_child( "FilesUserDefinedData")->add_child_text(
      std::string{ filesUserDefinedData.begin(), filesUserDefinedData.end() } );
  }

  // List of Loads User Defined Data
  const auto loadsUserDefinedData{ mediaSet->loadsUserDefinedData()};
  if ( !loadsUserDefinedData.empty())
  {
    mediaSetNode.add_child( "LoadsUserDefinedData")->add_child_text(
      std::string{ loadsUserDefinedData.begin(), loadsUserDefinedData.end() } );
  }

  // List of Batches User Defined Data
  const auto batchesUserDefinedData{ mediaSet->batchesUserDefinedData() };
  if ( !batchesUserDefinedData.empty() )
  {
    mediaSetNode.add_child( "BatchesUserDefinedData")->add_child_text(
      std::string{
        batchesUserDefinedData.begin(),
        batchesUserDefinedData.end() } );
  }

  // iterate over media
  for (
    unsigned int mediumNumber = 1U;
    mediumNumber <= mediaSet->numberOfMedia();
    ++mediumNumber )
  {
    auto medium{ mediaSet->medium( mediumNumber ) };

    auto mediumNode{ mediaSetNode.add_child( "Medium" ) };

    saveMedium( medium, filePathMapping, *mediumNode );
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

void Arinc665XmlImpl::loadMedium(
  const Media::MediaSetPtr& mediaSet,
  FilePathMapping &filePathMapping,
  const xmlpp::Node &mediumNode )
{
  auto medium{ mediaSet->addMedium() };

  loadEntries( medium, filePathMapping, mediumNode );
}

void Arinc665XmlImpl::saveMedium(
  const Media::ConstMediumPtr& medium,
  const FilePathMapping &filePathMapping,
  xmlpp::Node &mediumNode )
{
  saveEntries( medium, filePathMapping, mediumNode );
}

void Arinc665XmlImpl::loadDirectory(
  const Media::ContainerEntityPtr& parent,
  FilePathMapping &filePathMapping,
  const xmlpp::Element &directoryElement )
{
  const auto name{ directoryElement.get_attribute_value( "Name" ) };

  if ( name.empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Name attribute missing or empty" }
      << boost::errinfo_at_line{ directoryElement.get_line() } );
  }

  auto directory{ parent->addSubDirectory( static_cast< std::string>( name ) ) };

  loadEntries( directory, filePathMapping, directoryElement );
}

void Arinc665XmlImpl::saveDirectory(
  const Media::ConstDirectoryPtr& directory,
  const FilePathMapping &filePathMapping,
  xmlpp::Element &directoryElement )
{
  directoryElement.set_attribute( "Name", directory->name().data() );

  saveEntries( directory, filePathMapping, directoryElement );
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

    // iterate recursively over directories
    if ( entryNode->get_name() == "Directory"s )
    {
      loadDirectory( current, filePathMapping, *entryElement );
      continue;
    }

    const auto filename{ entryElement->get_attribute_value( "Name" ) };
    const auto sourcePath{ entryElement->get_attribute_value( "SourcePath" ) };

    if ( filename.empty() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Name attribute missing or empty" }
        << boost::errinfo_at_line{ currentNode.get_line() } );
    }

    if ( entryNode->get_name() == "File"s )
    {
      auto file{ current->addFile( toStringView( filename ) ) };

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.insert( { file, toStringView( sourcePath ) } );
      }
    }
    else if ( entryNode->get_name() == "LoadFile"s )
    {
      auto load{ current->addLoad( toStringView( filename ) ) };

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.insert( { load, toStringView( sourcePath ) } );
      }
    }
    else if ( entryNode->get_name() == "BatchFile"s )
    {
      auto batch{ current->addBatch( toStringView( filename ) ) };

      // set source path if attribute is present
      if ( !sourcePath.empty() )
      {
        filePathMapping.insert( { batch, toStringView( sourcePath ) } );
      }
    }
    else
    {
      BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
        << "Ignore element " << entryNode->get_name();
      continue;
    }
  }
}

void Arinc665XmlImpl::saveEntries(
  const Media::ConstContainerEntityPtr &current,
  const FilePathMapping &filePathMapping,
  xmlpp::Node &currentNode )
{
  // iterate over sub-directories within container
  for ( const auto &dirEntry : current->subDirectories() )
  {
    auto directoryNode{ currentNode.add_child( "Directory" ) };

    saveDirectory( dirEntry, filePathMapping, *directoryNode );
  }

  // iterate over files within container
  for ( const auto &fileEntry : current->files( false ) )
  {
    xmlpp::Element * fileNode{};

    switch ( fileEntry->fileType() )
    {
      case Media::BaseFile::FileType::RegularFile:
        fileNode = currentNode.add_child( "File" );
        break;

      case Media::BaseFile::FileType::LoadFile:
      {
        fileNode = currentNode.add_child( "LoadFile" );
        break;
      }

      case Media::BaseFile::FileType::BatchFile:
      {
        fileNode = currentNode.add_child( "BatchFile" );
        break;
      }

      default:
        continue;
    }

    // Add name attribute
    fileNode->set_attribute( "Name", fileEntry->name().data() );

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

    if ( nullptr == targetHardwareElement)
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
      //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    const auto filePartNumber{ dataFileElement->get_attribute_value( "PartNumber" ) };

    if ( filePartNumber.empty() )
    {
      //! @throw Arinc665::Arinc665Exception when PartNumber attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    auto file{ mediaSet->file( toStringView( fileNameRef ) ) };

    if ( !file )
    {
      //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference file.
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute does not reference file" }
        << boost::errinfo_at_line{ dataFileNode->get_line() } );
    }

    load->dataFile( { file, filePartNumber } );
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
      //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute missing or empty" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    const auto filePartNumber{ supportFileElement->get_attribute_value( "PartNumber" ) };

    if ( filePartNumber.empty() )
    {
      //! @throw Arinc665::Arinc665Exception when PartNumber attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "PartNumber attribute missing or empty" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    auto file{ mediaSet->file( toStringView( fileNameRef ) ) };

    if ( !file )
    {
      //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference file.
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "NameRef attribute does not reference file" }
        << boost::errinfo_at_line{ supportFileNode->get_line() } );
    }

    load->supportFile( { file, filePartNumber } );
  }

  const auto userDefinedDataElement{ dynamic_cast< const xmlpp::Element*>(
    loadElement.get_first_child( "UserDefinedData" ) ) };

  if ( nullptr != userDefinedDataElement )
  {
    const auto userDefinedData{
      userDefinedDataElement->get_child_text()->get_content() };

    load->userDefinedData(
      Arinc665::Media::Load::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end() } );
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
    (boost::format( "0x%04X" ) % load->partFlags() ).str() );

  // Optional Load Type (Description + Type Value)
  if ( const auto &loadType{ load->loadType() }; loadType )
  {
    const auto &[description,id]{ *loadType};
    loadElement.set_attribute( "Description", description );
    loadElement.set_attribute(
      "Type",
      (boost::format( "0x%04X") % id ).str() );
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
  for ( const auto &[weakDataFile,partNumber] : load->dataFiles() )
  {
    const auto dataFile{ weakDataFile.lock() };
    assert( dataFile );

    auto * const dataFileElement{ loadElement.add_child( "DataFile" ) };
    dataFileElement->set_attribute( "NameRef", dataFile->name().data() );
    dataFileElement->set_attribute( "PartNumber", partNumber );
  }

  // iterate over support files
  for ( const auto &[weakSupportFile,partNumber] : load->supportFiles())
  {
    const auto supportFile{ weakSupportFile.lock() };
    assert( supportFile );

    auto supportFileElement{ loadElement.add_child( "SupportFile" ) };
    supportFileElement->set_attribute(
      "NameRef",
      supportFile->name().data() );
    supportFileElement->set_attribute( "PartNumber", partNumber );
  }

  const auto userDefinedData{ load->userDefinedData() };
  if ( !userDefinedData.empty() )
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

    Media::WeakLoads loads{};

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
    batch->target( toStringView( thwIdPos ), loads);
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

      loadNode->set_attribute( "NameRef", load.lock()->name().data() );
    }
  }
}

static std::string_view toStringView( const Glib::ustring &str )
{
  return std::string_view{ str.data(), str.length() };
}

}
