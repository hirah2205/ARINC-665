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
 * @brief Declaration of class Arinc665::File::LoadHeaderFile.
 **/

#include "LoadHeaderFile.hpp"

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

LoadHeaderFile::LoadHeaderFile( Arinc665Version version) :
  Arinc665File( FileType::LoadUploadHeader, version, 6U),
  loadCrc( 0)
{
}

LoadHeaderFile::LoadHeaderFile( const RawFile &file):
  Arinc665File( FileType::LoadUploadHeader, file, 6U)
{
  decodeHeader( file, Arinc665FileFormatVersion::LOAD_FILE_VERSION_2);
  decodeBody( file);
}

LoadHeaderFile& LoadHeaderFile::operator=( const RawFile &file)
{
  decodeHeader( file, Arinc665FileFormatVersion::LOAD_FILE_VERSION_2);
  decodeBody( file);

  return *this;
}

LoadHeaderFile::string LoadHeaderFile::getPartNumber() const
{
  return partNumber;
}

void LoadHeaderFile::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

const std::list< std::string>& LoadHeaderFile::getTargetHardwareIdList() const
{
  return targetHardwareIdList;
}

std::list< std::string>& LoadHeaderFile::getTargetHardwareIdList()
{
  return targetHardwareIdList;
}

const LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getDataFileList() const
{
  return dataFileList;
}

LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getDataFileList()
{
  return dataFileList;
}

const LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getSupportFileList() const
{
  return supportFileList;
}

LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getSupportFileList()
{
  return supportFileList;
}

const LoadHeaderFile::UserDefinedData& LoadHeaderFile::getUserDefinedData() const
{
  return userDefinedData;
}

void LoadHeaderFile::setUserDefinedData( const UserDefinedData &userDefinedData)
{
  this->userDefinedData = userDefinedData;
}

uint32_t LoadHeaderFile::getLoadCrc() const
{
  return loadCrc;
}

void LoadHeaderFile::setLoadCrc( const uint32_t loadCrc)
{
  this->loadCrc = loadCrc;
}

RawFile LoadHeaderFile::encode() const
{
  RawFile file( BaseHeaderOffset + 3 * sizeof( uint32_t));

  // set header and crc
  insertHeader( file);

  return file;
}

void LoadHeaderFile::decodeBody( const RawFile &file)
{
  // set processing start to position after spare
  RawFile::const_iterator it = file.begin() + BaseHeaderOffset;

  uint32_t loadPartNumberPtr;
  it = getInt< uint32_t>( it, loadPartNumberPtr);

  uint32_t targetHardwareIdListPtr;
  it = getInt< uint32_t>( it, targetHardwareIdListPtr);

  uint32_t dataFileListPtr;
  it = getInt< uint32_t>( it, dataFileListPtr);

  uint32_t supportFileListPtr;
  it = getInt< uint32_t>( it, supportFileListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // load part number
  it = file.begin() + loadPartNumberPtr * 2;
  it = decodeString( it, partNumber);

  // target hardware id list
  it = file.begin() + targetHardwareIdListPtr * 2;
  it = decodeStringList( it, targetHardwareIdList);

  // data file list
  dataFileList = decodeFileList( file, dataFileListPtr * 2);

  // support file list
  if ( 0 != supportFileListPtr)
  {
    supportFileList = decodeFileList( file, supportFileListPtr * 2);
  }

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = file.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, file.end() - 6U);
  }

  // file crc decoded and checked within base class

  // load crc
  getInt< uint32_t>( file.end() - 4U, loadCrc);
}

LoadHeaderFile::LoadFileInfoList LoadHeaderFile::decodeFileList(
  const RawFile &file,
  const std::size_t offset)
{
  RawFile::const_iterator it( file.begin() + offset);

  LoadFileInfoList files;

  // number of data files
  uint16_t numberOfFiles;
  it = getInt< uint16_t>( it, numberOfFiles);

  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    auto listIt( it);

    // next file pointer
    uint16_t filePointer;
    listIt = getInt< uint16_t>( listIt, filePointer);

    // filename
    string name;
    listIt = decodeString( listIt, name);

    // part number
    string partNumber;
    listIt = decodeString( listIt, partNumber);

    // file length
    uint32_t length;
    listIt = getInt< uint32_t>( listIt, length);

    // CRC
    uint16_t crc;
    listIt = getInt< uint16_t>( listIt, crc);

    // set it to begin of next file
    it += filePointer * 2;

    // file info
    files.emplace_back( name, partNumber, length, crc);
  }

  return files;
}

}
}
