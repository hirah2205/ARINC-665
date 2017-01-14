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

#include <helper/Logger.hpp>

namespace Arinc665 {
namespace Utils {

Arinc665XmlPugiXmlImpl::MediaSetPtr Arinc665XmlPugiXmlImpl::loadFromXml(
  const path &xmlFile)
{
  BOOST_LOG_FUNCTION();

  if (boost::filesystem::is_regular( xmlFile))
  {
    return MediaSetPtr();
  }

  pugi::xml_document xmlDoc;

  xmlDoc.load_file( xmlFile.c_str());
  pugi::xml_node mediaSetNode( xmlDoc.root().child( "MediaSet"));

  return loadMediaSet( mediaSetNode);
}

void Arinc665XmlPugiXmlImpl::saveToXml(
  ConstMediaSetPtr mediaSet,
  const path &xmlFile)
{
  BOOST_LOG_FUNCTION();

  pugi::xml_document xmlDoc;
  pugi::xml_node mediaSetNode( xmlDoc.root().append_child( "MediaSet"));

  saveMediaSet( mediaSet, mediaSetNode);

  xmlDoc.save_file( xmlFile.c_str());
}

Arinc665XmlPugiXmlImpl::MediaSetPtr Arinc665XmlPugiXmlImpl::loadMediaSet(
  const pugi::xml_node &mediaSetNode)
{
  std::string name( mediaSetNode.attribute( "Name").as_string());
  std::string partNumber( mediaSetNode.attribute( "PartNumber").as_string());

  MediaSetPtr mediaSet( std::make_shared< Media::MediaSet>( name));
  mediaSet->setPartNumber( partNumber);

  for ( pugi::xml_node mediumNode : mediaSetNode.children( "Medium"))
  {
    loadMedium( mediaSet, mediumNode);
  }

  return mediaSet;
}

void Arinc665XmlPugiXmlImpl::saveMediaSet(
  ConstMediaSetPtr mediaSet,
  pugi::xml_node &mediaSetNode)
{
  mediaSetNode.append_attribute( "Name") = mediaSet->getName().c_str();
  mediaSetNode.append_attribute( "PartNumber") = mediaSet->getPartNumber().c_str();

  for (
    unsigned int mediumNumber = 1;
    mediumNumber <= mediaSet->getNumberOfMedia();
    ++mediumNumber)
  {
    Media::ConstMediumPtr medium( mediaSet->getMedium( mediumNumber));

    pugi::xml_node mediumNode( mediaSetNode.append_child( "Medium"));

    saveMedium( medium, mediumNode);
  }
}

void Arinc665XmlPugiXmlImpl::loadMedium( MediaSetPtr mediaSet, const pugi::xml_node &mediumNode)
{
  Media::MediumPtr medium( mediaSet->addMedium());

  loadEntries( medium, mediumNode);
}

void Arinc665XmlPugiXmlImpl::saveMedium( ConstMediumPtr medium, pugi::xml_node &mediumNode)
{
  saveEntries( medium, mediumNode);
}

void Arinc665XmlPugiXmlImpl::loadDirectory( ContainerEntityPtr parent, const pugi::xml_node &directoryNode)
{
  Media::DirectoryPtr directory(
    parent->addSubDirectory( directoryNode.attribute( "Name").as_string()));

  loadEntries( directory, directoryNode);
}

void Arinc665XmlPugiXmlImpl::saveDirectory( ConstDirectoryPtr directory, pugi::xml_node &directoryNode)
{
  directoryNode.append_attribute( "Name") = directory->getName().c_str();

  saveEntries( directory, directoryNode);
}

void Arinc665XmlPugiXmlImpl::loadEntries( ContainerEntityPtr current, const pugi::xml_node &currentNode)
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
      loadDirectory( current, entryNode);
      continue;
    }

    Media::BaseFilePtr file;
    std::string filename( entryNode.attribute( "Name").as_string());
    std::string partNumber( entryNode.attribute( "PartNumber").as_string());

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
      continue;
    }

    if (!partNumber.empty())
    {
      file->setPartNumber( partNumber);
    }
  }
}

void Arinc665XmlPugiXmlImpl::saveEntries( ConstContainerEntityPtr current, pugi::xml_node &currentNode)
{
  for (auto dirEntry : current->getSubDirectories())
  {
    pugi::xml_node directoryNode(
      currentNode.append_child( dirEntry->getName().c_str()));

    saveDirectory( dirEntry, directoryNode);
  }

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

    fileNode.append_attribute( "Name") = fileEntry->getName().c_str();

    if (!fileEntry->getPartNumber().empty())
    {
      fileNode.append_attribute( "PartNumber") = fileEntry->getPartNumber().c_str();
    }
  }
}

}
}

