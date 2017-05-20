/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
*
* @brief Definition of class Arinc665::Utils::Arinc665XmlPugiXmlImpl.
 **/

#include "Arinc665XmlPugiXmlImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Logger.hpp>

namespace Arinc665 {
namespace Utils {

Arinc665XmlPugiXmlImpl::LoadXmlResult Arinc665XmlPugiXmlImpl::loadFromXml(
  const path &xmlFile)
{
  BOOST_LOG_FUNCTION();

  if (!boost::filesystem::is_regular( xmlFile))
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
      "File " << xmlFile << " does not exist";
    return {};
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
  const path &xmlFile)
{
  BOOST_LOG_FUNCTION();

  pugi::xml_document xmlDoc;
  pugi::xml_node mediaSetNode( xmlDoc.root().append_child( "MediaSet"));

  saveMediaSet( mediaSet, filePathMapping, mediaSetNode);

  xmlDoc.save_file( xmlFile.c_str());
}

Media::MediaSetPtr Arinc665XmlPugiXmlImpl::loadMediaSet(
  FilePathMapping &filePathMapping,
  const pugi::xml_node &mediaSetNode)
{
  std::string name( mediaSetNode.attribute( "Name").as_string());
  std::string partNumber( mediaSetNode.attribute( "PartNumber").as_string());

  auto mediaSet( std::make_shared< Media::MediaSet>( name));
  mediaSet->setPartNumber( partNumber);

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
  mediaSetNode.append_attribute( "Name") = mediaSet->getName().c_str();
  mediaSetNode.append_attribute( "PartNumber") = mediaSet->getPartNumber().c_str();

  // iterate over media
  for (
    unsigned int mediumNumber = 1;
    mediumNumber <= mediaSet->getNumberOfMedia();
    ++mediumNumber)
  {
    Media::ConstMediumPtr medium( mediaSet->getMedium( mediumNumber));

    pugi::xml_node mediumNode( mediaSetNode.append_child( "Medium"));

    saveMedium( medium, filePathMapping, mediumNode);
  }

  // handle Loads
  auto loadsNode( mediaSetNode.append_child( "Loads"));

  // iterate over loads
  for ( auto load : mediaSet->getLoads())
  {
    pugi::xml_node loadNode( loadsNode.append_child( "Load"));

    saveLoad( load, loadNode);
  }

  // handle Batches
  auto batchesNode( mediaSetNode.append_child( "Batches"));

  // iterate over batches
  for ( auto batch : mediaSet->getBatches())
  {
    pugi::xml_node batchNode( batchesNode.append_child( "Batch"));

    saveBatch( batch, batchNode);
  }
}

void Arinc665XmlPugiXmlImpl::loadMedium(
  Media::MediaSetPtr mediaSet,
  FilePathMapping &filePathMapping,
  const pugi::xml_node &mediumNode)
{
  auto medium( mediaSet->addMedium());

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
  auto directory(
    parent->addSubDirectory( directoryNode.attribute( "Name").as_string()));

  loadEntries( directory, filePathMapping, directoryNode);
}

void Arinc665XmlPugiXmlImpl::saveDirectory(
  Media::ConstDirectoryPtr directory,
  const FilePathMapping &filePathMapping,
  pugi::xml_node &directoryNode)
{
  directoryNode.append_attribute( "Name") = directory->getName().c_str();

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

    std::string filename( entryNode.attribute( "Name").as_string());
    std::string partNumber( entryNode.attribute( "PartNumber").as_string());
    std::string sourcePath( entryNode.attribute( "SourcePath").as_string());

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
      file->setPartNumber( partNumber);
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
  for (auto dirEntry : current->getSubDirectories())
  {
    pugi::xml_node directoryNode(
      currentNode.append_child( dirEntry->getName().c_str()));

    saveDirectory( dirEntry, filePathMapping, directoryNode);
  }

  // iterate over files within container
  for (auto fileEntry : current->getFiles( false))
  {
    pugi::xml_node fileNode( currentNode.append_child(
      pugi::xml_node_type::node_element));

    switch (fileEntry->getFileType())
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
    fileNode.append_attribute( "Name") = fileEntry->getName().c_str();

    // Add part number attribute (optional)
    if (!fileEntry->getPartNumber().empty())
    {
      fileNode.append_attribute( "PartNumber") = fileEntry->getPartNumber().c_str();
    }

    // Add source path attribute (optional)
    auto filePathIt( filePathMapping.find( fileEntry));
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
  // name
  const std::string nameRef( loadNode.attribute( "NameRef").as_string());

  auto load( mediaSet->getLoad( nameRef));
  //! @todo error handling

  Media::Load::ThwIdList thwIdList;

  // iterate over target hardware
  for ( pugi::xml_node targetHardwareNode : loadNode.children( "TargetHardware"))
  {
    const std::string thwId( targetHardwareNode.attribute( "ThwId").as_string());

    // iterate over positions
    /*
    for ( pugi::xml_node positionNode : targetHardwareNode.children( "Position"))
    {
      const std::string position( targetHardwareNode.attribute( "Pos").as_string());
    }
    */

    thwIdList.push_back( thwId);
  }

  load->setTargetHardwareIdList( thwIdList);

  // iterate over data files
  for ( pugi::xml_node dataFileNode : loadNode.children( "DataFile"))
  {
    std::string fileNameRef( dataFileNode.attribute( "NameRef").as_string());

    auto file( mediaSet->getFile( fileNameRef));

    //! @todo error handling

    load->addDataFile( file);
  }

  // iterate over support files
  for ( pugi::xml_node dataFileNode : loadNode.children( "SupportFile"))
  {
    std::string fileNameRef( dataFileNode.attribute( "NameRef").as_string());

    auto file( mediaSet->getFile( fileNameRef));

    //! @todo error handling

    load->addSupportFile( file);
  }
}

void Arinc665XmlPugiXmlImpl::saveLoad(
  Media::ConstLoadPtr load,
  pugi::xml_node &loadNode)
{
  loadNode.append_attribute( "NameRef") = load->getName().c_str();


  // iterate over target hardware id
  for (auto targetHardware : load->getTargetHardwareIdList())
  {
    auto targetHardwareNode( loadNode.append_child( "TargetHardware"));
    targetHardwareNode.append_attribute( "ThwId") =
      targetHardware.c_str();
  }

  // iterate over data files
  for (auto dataFile : load->getDataFiles())
  {
    auto dataFileNode( loadNode.append_child( "DataFile"));
    dataFileNode.append_attribute( "NameRef") =
      dataFile.lock()->getName().c_str();
  }

  // iterate over support files
  for (auto supportFile : load->getSupportFiles())
  {
    auto supportFileNode( loadNode.append_child( "SupportFile"));
    supportFileNode.append_attribute( "NameRef") =
      supportFile.lock()->getName().c_str();
  }
}

void Arinc665XmlPugiXmlImpl::loadBatch(
  Media::MediaSetPtr mediaSet,
  const pugi::xml_node &batchNode)
{
  // name
  const std::string nameRef( batchNode.attribute( "NameRef").as_string());

  auto batch( mediaSet->getBatch( nameRef));
  //! @todo error handling

  // iterate over targets
  for ( pugi::xml_node targetNode : batchNode.children( "Target"))
  {
    const std::string thwIdPos( targetNode.attribute( "ThwIdPos").as_string());

    Media::WeakLoads loads;

    // iterate over loads
    for ( pugi::xml_node LoadNode : targetNode.children( "Load"))
    {
      const std::string loadNameRef( LoadNode.attribute( "NameRef").as_string());

      auto load( mediaSet->getLoad( loadNameRef));

      //! @todo error handling

      loads.push_back( load);
    }

    // add load
    batch->addTarget( thwIdPos, loads);
  }
}

void Arinc665XmlPugiXmlImpl::saveBatch(
  Media::ConstBatchPtr batch,
  pugi::xml_node &batchNode)
{
  batchNode.append_attribute( "NameRef") = batch->getName().c_str();

  // Iterate over batch infos
  for ( auto target : batch->getTargets())
  {
    auto targetNode( batchNode.append_child( "Target"));

    targetNode.append_attribute( "ThwIdPos") = target.first.c_str();

    // iterate over loads
    for ( auto load : target.second)
    {
      auto loadNode( targetNode.append_child( "Load"));

      loadNode.append_attribute( "NameRef")= load.lock()->getName().c_str();
    }
  }
}

}
}

