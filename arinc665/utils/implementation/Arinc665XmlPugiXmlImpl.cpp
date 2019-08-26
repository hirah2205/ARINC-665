/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::Arinc665XmlPugiXmlImpl.
 **/

#include "Arinc665XmlPugiXmlImpl.hpp"

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

Arinc665XmlPugiXmlImpl::LoadXmlResult Arinc665XmlPugiXmlImpl::loadFromXml(
  const std::filesystem::path &xmlFile)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Load Media Set from " << xmlFile;

  // Check existence of file
  if (!std::filesystem::is_regular_file( xmlFile))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "XML File does not exist"));
  }

  pugi::xml_document xmlDoc;

  xmlDoc.load_file( xmlFile.c_str());
  pugi::xml_node mediaSetNode( xmlDoc.root().child( "MediaSet"));

  FilePathMapping filePathMapping;
  auto mediaSet( loadMediaSet( filePathMapping, mediaSetNode));

  return { mediaSet, filePathMapping};
}

void Arinc665XmlPugiXmlImpl::saveToXml(
  Media::ConstMediaSetPtr mediaSet,
  const FilePathMapping &filePathMapping,
  const std::filesystem::path &xmlFile)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Save Media Set " << mediaSet->partNumber() << " to " << xmlFile;

  pugi::xml_document xmlDoc;
  pugi::xml_node mediaSetNode{ xmlDoc.root().append_child( "MediaSet")};

  saveMediaSet( mediaSet, filePathMapping, mediaSetNode);

  if (!xmlDoc.save_file( xmlFile.c_str(), "  "))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Error writing XML file"));
  }
}

Media::MediaSetPtr Arinc665XmlPugiXmlImpl::loadMediaSet(
  FilePathMapping &filePathMapping,
  const pugi::xml_node &mediaSetNode)
{
  const std::string partNumber( mediaSetNode.attribute( "PartNumber").as_string());

  auto mediaSet{ std::make_shared< Media::MediaSet>()};
  mediaSet->partNumber( partNumber);

  const auto filesUserDefinedDataNode{ mediaSetNode.child( "FilesUserDefinedData")};
  if ( filesUserDefinedDataNode)
  {
    const std::string userDefinedData{ filesUserDefinedDataNode.child_value()};
    mediaSet->filesUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end()});
  }

  const auto loadsUserDefinedDataNode{ mediaSetNode.child( "LoadsUserDefinedData")};
  if ( loadsUserDefinedDataNode)
  {
    const std::string userDefinedData{ loadsUserDefinedDataNode.child_value()};
    mediaSet->loadsUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end()});
  }

  const auto batchesUserDefinedDataNode{ mediaSetNode.child( "BatchesUserDefinedData")};
  if ( batchesUserDefinedDataNode)
  {
    const std::string userDefinedData{ batchesUserDefinedDataNode.child_value()};
    mediaSet->batchesUserDefinedData(
      Arinc665::Media::MediaSet::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end()});
  }

  // iterate over media
  for ( pugi::xml_node mediumNode : mediaSetNode.children( "Medium"))
  {
    loadMedium( mediaSet, filePathMapping, mediumNode);
  }

  // handle Loads
  auto loadsNode( mediaSetNode.child( "Loads"));

  // iterate over loads
  for ( pugi::xml_node loadNode : loadsNode.children( "Load"))
  {
    loadLoad( mediaSet, loadNode);
  }

  // handle Batches
  auto batchesNode( mediaSetNode.child( "Batches"));

  // iterate over loads
  for ( pugi::xml_node batchNode : batchesNode.children( "Batch"))
  {
    loadBatch( mediaSet, batchNode);
  }

  return mediaSet;
}

void Arinc665XmlPugiXmlImpl::saveMediaSet(
  Media::ConstMediaSetPtr mediaSet,
  const FilePathMapping &filePathMapping,
  pugi::xml_node &mediaSetNode)
{
  mediaSetNode.append_attribute( "PartNumber") = mediaSet->partNumber().data();

  const auto filesUserDefinedData{ mediaSet->filesUserDefinedData()};
  if (!filesUserDefinedData.empty())
  {
    mediaSetNode.append_child( "FilesUserDefinedData").append_child(
      pugi::node_pcdata).set_value(
      std::string{ filesUserDefinedData.begin(), filesUserDefinedData.end()}.c_str());
  }

  const auto loadsUserDefinedData{ mediaSet->loadsUserDefinedData()};
  if (!loadsUserDefinedData.empty())
  {
    mediaSetNode.append_child( "LoadsUserDefinedData").append_child(
      pugi::node_pcdata).set_value(
      std::string{ loadsUserDefinedData.begin(), loadsUserDefinedData.end()}.c_str());
  }

  const auto batchesUserDefinedData{ mediaSet->batchesUserDefinedData()};
  if (!batchesUserDefinedData.empty())
  {
    mediaSetNode.append_child( "BatchesUserDefinedData").append_child(
      pugi::node_pcdata).set_value(
      std::string{ batchesUserDefinedData.begin(), batchesUserDefinedData.end()}.c_str());
  }

  // iterate over media
  for (
    unsigned int mediumNumber = 1U;
    mediumNumber <= mediaSet->numberOfMedia();
    ++mediumNumber)
  {
    Media::ConstMediumPtr medium{ mediaSet->medium( mediumNumber)};

    pugi::xml_node mediumNode{ mediaSetNode.append_child( "Medium")};

    saveMedium( medium, filePathMapping, mediumNode);
  }

  // handle Loads
  auto loadsNode{ mediaSetNode.append_child( "Loads")};

  // iterate over loads
  for ( auto load : mediaSet->loads())
  {
    pugi::xml_node loadNode{ loadsNode.append_child( "Load")};

    saveLoad( load, loadNode);
  }

  // handle Batches
  auto batchesNode{ mediaSetNode.append_child( "Batches")};

  // iterate over batches
  for ( auto batch : mediaSet->batches())
  {
    pugi::xml_node batchNode{ batchesNode.append_child( "Batch")};

    saveBatch( batch, batchNode);
  }
}

void Arinc665XmlPugiXmlImpl::loadMedium(
  Media::MediaSetPtr mediaSet,
  FilePathMapping &filePathMapping,
  const pugi::xml_node &mediumNode)
{
  auto medium{ mediaSet->addMedium()};

  loadEntries( medium, filePathMapping, mediumNode);
}

void Arinc665XmlPugiXmlImpl::saveMedium(
  Media::ConstMediumPtr medium,
  const FilePathMapping &filePathMapping,
  pugi::xml_node &mediumNode)
{
  saveEntries( medium, filePathMapping, mediumNode);
}

void Arinc665XmlPugiXmlImpl::loadDirectory(
  Media::ContainerEntityPtr parent,
  FilePathMapping &filePathMapping,
  const pugi::xml_node &directoryNode)
{
  const std::string name{ directoryNode.attribute( "Name").as_string()};

  if (name.empty())
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "Name attribute missing or empty"));
  }

  auto directory{ parent->addSubDirectory( name) };

  loadEntries( directory, filePathMapping, directoryNode);
}

void Arinc665XmlPugiXmlImpl::saveDirectory(
  Media::ConstDirectoryPtr directory,
  const FilePathMapping &filePathMapping,
  pugi::xml_node &directoryNode)
{
  directoryNode.append_attribute( "Name") = directory->name().data();

  saveEntries( directory, filePathMapping, directoryNode);
}

void Arinc665XmlPugiXmlImpl::loadEntries(
  Media::ContainerEntityPtr current,
  FilePathMapping &filePathMapping,
  const pugi::xml_node &currentNode)
{
  for ( pugi::xml_node entryNode : currentNode.children())
  {
    using namespace std::string_literals;

    if (entryNode.type() != pugi::xml_node_type::node_element)
    {
       // skip non-elements
      continue;
    }

    if (entryNode.name() == "Directory"s)
    {
      // iterate recursively over directories
      loadDirectory( current, filePathMapping, entryNode);
      continue;
    }

    const std::string filename{ entryNode.attribute( "Name").as_string()};
    const std::string partNumber{ entryNode.attribute( "PartNumber").as_string()};
    const std::string sourcePath{ entryNode.attribute( "SourcePath").as_string()};

    if (filename.empty())
    {
      //! @throw Arinc665::Arinc665Exception when Name Attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Name attribute missing or empty"));
    }

    // create the right file
    Media::BaseFilePtr file;

    if (entryNode.name() == "File"s)
    {
      file = current->addFile( filename);
    }
    else if (entryNode.name() == "LoadFile"s)
    {
      file = current->addLoad( filename);
    }
    else if (entryNode.name() == "BatchFile"s)
    {
      file = current->addBatch( filename);
    }
    else
    {
      BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
        "Ignore element " << entryNode.name();
      continue;
    }

    // set part number if attribute is present
    if (!partNumber.empty())
    {
      file->partNumber( partNumber);
    }

    // set source path if attribute is present
    if (!sourcePath.empty())
    {
      filePathMapping.insert( {file, sourcePath});
    }
  }
}

void Arinc665XmlPugiXmlImpl::saveEntries(
  Media::ConstContainerEntityPtr current,
  const FilePathMapping &filePathMapping,
  pugi::xml_node &currentNode)
{
  // iterate over sub-directories within container
  for (auto dirEntry : current->subDirectories())
  {
    auto directoryNode{ currentNode.append_child( "Directory")};

    saveDirectory( dirEntry, filePathMapping, directoryNode);
  }

  // iterate over files within container
  for (auto fileEntry : current->files( false))
  {
    pugi::xml_node fileNode{ currentNode.append_child(
      pugi::xml_node_type::node_element)};

    switch (fileEntry->fileType())
    {
      case Media::BaseFile::FileType::RegularFile:
        fileNode.set_name( "File");
        break;

      case Media::BaseFile::FileType::LoadFile:
        fileNode.set_name( "LoadFile");
        break;

      case Media::BaseFile::FileType::BatchFile:
        fileNode.set_name( "BatchFile");
        break;

      default:
        continue;
    }

    // Add name attribute
    fileNode.append_attribute( "Name") = fileEntry->name().data();

    // Add part number attribute (optional)
    if (!fileEntry->partNumber().empty())
    {
      fileNode.append_attribute( "PartNumber") = fileEntry->partNumber().data();
    }

    // Add source path attribute (optional)
    auto filePathIt{ filePathMapping.find( fileEntry)};
    if (filePathIt != filePathMapping.end())
    {
      fileNode.append_attribute( "SourcePath") = filePathIt->second.c_str();
    }
  }
}

void Arinc665XmlPugiXmlImpl::loadLoad(
  Media::MediaSetPtr mediaSet,
  const pugi::xml_node &loadNode)
{
  const std::string nameRef{ loadNode.attribute( "NameRef").as_string()};
  const std::string description{ loadNode.attribute( "Description").as_string()};
  const std::string type{ loadNode.attribute( "Type").as_string()};

  if (nameRef.empty())
  {
    //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "NameRef attribute missing or empty"));
  }

  auto load{ mediaSet->load( nameRef)};

  if (!load)
  {
    //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference load.
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "NameRef attribute does not reference load"));
  }

  // Load Type (Description + Type Value)
  if (!description.empty())
  {
    uint16_t typeValue{ safeCast< uint16_t >( std::stoul( type, 0, 0))};

    load->loadType( {std::make_pair( description, typeValue)});
  }

  Media::Load::TargetHardwareIdPositions thwIds;

  // iterate over target hardware
  for ( pugi::xml_node targetHardwareNode : loadNode.children( "TargetHardware"))
  {
    const std::string thwId{ targetHardwareNode.attribute( "ThwId").as_string()};

    std::list< std::string> positions;

    // iterate over positions
    for ( pugi::xml_node positionNode : targetHardwareNode.children( "Position"))
    {
      const std::string position{ positionNode.attribute( "Pos").as_string()};

      positions.push_back( std::move( position));
    }

    thwIds.insert( std::make_pair( std::move( thwId), std::move( positions)));
  }

  load->targetHardwareIdPositions( std::move( thwIds));

  // iterate over data files
  for ( pugi::xml_node dataFileNode : loadNode.children( "DataFile"))
  {
    std::string fileNameRef{ dataFileNode.attribute( "NameRef").as_string()};

    if (fileNameRef.empty())
    {
      //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "NameRef attribute missing or empty"));
    }

    auto file{ mediaSet->file( fileNameRef)};

    if (!file)
    {
      //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference file.
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "NameRef attribute does not reference file"));
    }

    load->dataFile( file);
  }

  // iterate over support files
  for ( pugi::xml_node dataFileNode : loadNode.children( "SupportFile"))
  {
    std::string fileNameRef{ dataFileNode.attribute( "NameRef").as_string()};

    if (fileNameRef.empty())
    {
      //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "NameRef attribute missing or empty"));
    }

    auto file{ mediaSet->file( fileNameRef)};

    if (!file)
    {
      //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference file.
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "NameRef attribute does not reference file"));
    }

    load->supportFile( file);
  }

  const auto userDefinedDataNode{ loadNode.child( "UserDefinedData")};
  if ( userDefinedDataNode)
  {
     const std::string userDefinedData{ userDefinedDataNode.child_value()};
    load->userDefinedData(
      Arinc665::Media::Load::UserDefinedData{
        userDefinedData.begin(),
        userDefinedData.end()});
  }
}

void Arinc665XmlPugiXmlImpl::saveLoad(
  Media::ConstLoadPtr load,
  pugi::xml_node &loadNode)
{
  loadNode.append_attribute( "NameRef") = load->name().data();

  // Optional Load Type (Description + Type Value)
  const auto &loadType{ load->loadType()};

  if ( loadType)
  {
    loadNode.append_attribute( "Description") = std::get< 0>( *loadType).c_str();
    loadNode.append_attribute( "Type") =
      (boost::format( "0x%04X") % std::get< 1>( *loadType)).str().c_str();
  }

  // iterate over target hardware
  for (const auto &[targetHardwareId, positions] : load->targetHardwareIdPositions())
  {
    auto targetHardwareNode{ loadNode.append_child( "TargetHardware")};
    targetHardwareNode.append_attribute( "ThwId") =
      targetHardwareId.c_str();

    for( const auto &position : positions)
    {
      auto positionNode{ targetHardwareNode.append_child( "Position")};
      positionNode.append_attribute( "Pos") =
        position.c_str();
    }
  }

  // iterate over data files
  for (auto dataFile : load->dataFiles())
  {
    auto dataFileNode{ loadNode.append_child( "DataFile")};
    dataFileNode.append_attribute( "NameRef") =
      dataFile.lock()->name().data();
  }

  // iterate over support files
  for (auto supportFile : load->supportFiles())
  {
    auto supportFileNode{ loadNode.append_child( "SupportFile")};
    supportFileNode.append_attribute( "NameRef") =
      supportFile.lock()->name().data();
  }

  const auto userDefinedData{ load->userDefinedData()};
  if (!userDefinedData.empty())
  {
    loadNode.append_child( "UserDefinedData").append_child(
      pugi::node_pcdata).set_value(
        std::string{ userDefinedData.begin(), userDefinedData.end()}.c_str());
  }
}

void Arinc665XmlPugiXmlImpl::loadBatch(
  Media::MediaSetPtr mediaSet,
  const pugi::xml_node &batchNode)
{
  const std::string nameRef{ batchNode.attribute( "NameRef").as_string()};
  const std::string comment{ batchNode.attribute( "Comment").as_string()};

  if (nameRef.empty())
  {
    //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "NameRef attribute missing or empty"));
  }

  auto batch{ mediaSet->batch( nameRef)};

  if (!batch)
  {
    //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference batch.
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "NameRef attribute does not reference batch"));
  }

  batch->comment( std::move( comment));

  // iterate over targets
  for ( pugi::xml_node targetNode : batchNode.children( "Target"))
  {
    const std::string thwIdPos{ targetNode.attribute( "ThwIdPos").as_string()};

    Media::WeakLoads loads;

    // iterate over loads
    for ( pugi::xml_node LoadNode : targetNode.children( "Load"))
    {
      const std::string loadNameRef{ LoadNode.attribute( "NameRef").as_string()};

      if (loadNameRef.empty())
      {
        //! @throw Arinc665::Arinc665Exception when NameRef attribute is missing or empty.
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "NameRef attribute missing or empty"));
      }

      auto load{ mediaSet->load( loadNameRef)};

      if (!load)
      {
        //! @throw Arinc665::Arinc665Exception when NameRef attribute does not reference load.
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "NameRef attribute does not reference load"));
      }

      loads.push_back( load);
    }

    // add load
    batch->target( thwIdPos, loads);
  }
}

void Arinc665XmlPugiXmlImpl::saveBatch(
  Media::ConstBatchPtr batch,
  pugi::xml_node &batchNode)
{
  batchNode.append_attribute( "NameRef") = batch->name().data();
  batchNode.append_attribute( "Comment") = batch->comment().data();

  // Iterate over batch information
  for ( const auto &target : batch->targets())
  {
    auto targetNode{ batchNode.append_child( "Target")};

    targetNode.append_attribute( "ThwIdPos") = target.first.c_str();

    // iterate over loads
    for ( auto load : target.second)
    {
      auto loadNode{ targetNode.append_child( "Load")};

      loadNode.append_attribute( "NameRef") = load.lock()->name().data();
    }
  }
}

}
