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
 * @brief Definition of class Arinc665::File::Arinc665File.
 **/

#include "Arinc665File.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Crc.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

RawFile::const_iterator Arinc665File::decodeString(
  RawFile::const_iterator it,
  std::string &str)
{
  // determine string length
  uint16_t strLength;
  it = getInt< uint16_t>( it, strLength);

  // copy string
  str.assign( it, it + strLength);
  it += strLength;

  if ( strLength % 2 == 1)
  {
    ++it;
  }

  return it;
}

RawFile Arinc665File::encodeString( const std::string &str)
{
  RawFile rawString( sizeof( uint16_t));

  auto it( rawString.begin());

  // set string length
  it = setInt< uint16_t>( it, static_cast< uint16_t>( str.size()));

  // copy string
  rawString.insert( it, str.begin(), str.end());

  // fill string if it is odd
  if ( str.size() % 2 == 1)
  {
    rawString.push_back( 0U);
  }

  return rawString;
}

RawFile::const_iterator Arinc665File::decodeStringList(
  RawFile::const_iterator it,
  StringList &strList)
{
  // number of strings
  uint16_t numberOfEntries;
  it = getInt< uint16_t>( it, numberOfEntries);

  for ( unsigned int index = 0; index < numberOfEntries; ++index)
  {
    // string
    string str;
    it = decodeString( it, str);
    strList.push_back( str);
  }

  return it;
}

RawFile Arinc665File::encodeStringList( const StringList &strList)
{
  RawFile rawStringList( sizeof( uint16_t));

  auto it( rawStringList.begin());

  // set number of strings
  it = setInt< uint16_t>( it, static_cast< uint16_t>( strList.size()));

  for ( const auto & str : strList)
  {
    auto rawStr( encodeString( str));
    assert( rawStr.size() % 2 == 0);

    // append string
    rawStringList.insert( rawStringList.end(), rawStr.begin(), rawStr.end());
  }

  return rawStringList;
}

Arinc665File::string Arinc665File::encodePath( const path &path)
{
  string convertedPath( path.string());

  std::replace( convertedPath.begin(), convertedPath.end(), '/', '\\');

  return convertedPath;
}

uint32_t Arinc665File::getFileLength( const RawFile &file)
{
  // check file size
  if ( file.size() < BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File
    //!   If the file size is to small to represent an valid ARINC 665 file.
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
        << AdditionalInfo( "length of check code string invalid"));
  }

  // decode the file length
  uint32_t fileLength;
  getInt< uint32_t>( file.begin(), fileLength);

  return fileLength;
}

uint16_t Arinc665File::getFormatVersion( const RawFile &file)
{
  // check file size
  if ( file.size() < BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File
    //!   If the file size is to small to represent an valid ARINC 665 file.
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "file content to small"));
  }

  // decode the format version
  uint16_t formatVersion;
  getInt< uint16_t>( file.begin() + 4, formatVersion);

  return formatVersion;
}

uint16_t Arinc665File::calculateChecksum(
  const RawFile &file,
  const std::size_t skipLastBytes)
{
  Arinc665Crc16 arincCrc16;

  arincCrc16.process_block(
    &(*file.begin()),
    &(*file.begin()) + file.size() - skipLastBytes);

  return arincCrc16.checksum();
}

Arinc665::FileClassType Arinc665File::getArincFileType( const RawFile &rawFile)
{
  switch (getFormatVersion( rawFile))
  {
    case static_cast< uint16_t>( LoadFileFormatVersion::Version2):
    case static_cast< uint16_t>( LoadFileFormatVersion::Version34):
      return FileClassType::LoadFile;

    case static_cast< uint16_t>( BatchFileFormatVersion::Version2):
    case static_cast< uint16_t>( BatchFileFormatVersion::Version34):
      return FileClassType::BatchFile;

    case static_cast< uint16_t>( MediaFileFormatVersion::Version2):
    case static_cast< uint16_t>( MediaFileFormatVersion::Version34):
      return FileClassType::MediaFile;

    default:
      return FileClassType::Invalid;
  }
}

Arinc665::LoadFileFormatVersion Arinc665File::getLoadFileFormatVersion(
  const RawFile &rawFile)
{
  const uint16_t formatVersion( Arinc665File::getFormatVersion( rawFile));

  switch (formatVersion)
  {
    case static_cast< uint16_t>( LoadFileFormatVersion::Version2):
    case static_cast< uint16_t>( LoadFileFormatVersion::Version34):
      break;

    default:
      return LoadFileFormatVersion::Invalid;
  }

  return static_cast< LoadFileFormatVersion>( formatVersion);
}

Arinc665::BatchFileFormatVersion Arinc665File::getBatchFileFormatVersion(
  const RawFile &rawFile)
{
  const uint16_t formatVersion( Arinc665File::getFormatVersion( rawFile));

  switch (formatVersion)
  {
    case static_cast< uint16_t>( BatchFileFormatVersion::Version2):
    case static_cast< uint16_t>( BatchFileFormatVersion::Version34):
      break;

    default:
      return BatchFileFormatVersion::Invalid;
  }

  return static_cast< BatchFileFormatVersion>( formatVersion);
}

Arinc665::MediaFileFormatVersion Arinc665File::getMediaFileFormatVersion(
  const RawFile &rawFile)
{
  const uint16_t formatVersion( Arinc665File::getFormatVersion( rawFile));

  switch (formatVersion)
  {
    case static_cast< uint16_t>( MediaFileFormatVersion::Version2):
    case static_cast< uint16_t>( MediaFileFormatVersion::Version34):
      break;

    default:
      return MediaFileFormatVersion::Invalid;
  }

  return static_cast< MediaFileFormatVersion>( formatVersion);
}

Arinc665Version Arinc665File::getArinc665Version(
  const FileType fileType,
  const uint16_t formatVersionField)
{
  switch (fileType)
  {
    case FileType::BatchFile:
      switch (static_cast< BatchFileFormatVersion>( formatVersionField))
      {
        case BatchFileFormatVersion::Version1:
          return Arinc665Version::ARINC_665_1;

        case BatchFileFormatVersion::Version2:
          return Arinc665Version::ARINC_665_2;

        case BatchFileFormatVersion::Version34:
          return Arinc665Version::ARINC_665_4;

        default:
          break;
      }
      break;

    case FileType::LoadUploadHeader:
      switch (static_cast< LoadFileFormatVersion>( formatVersionField))
      {
        case LoadFileFormatVersion::Version1:
          return Arinc665Version::ARINC_665_1;

        case LoadFileFormatVersion::Version2:
          return Arinc665Version::ARINC_665_2;

        case LoadFileFormatVersion::Version34:
          return Arinc665Version::ARINC_665_4;

        default:
          break;
      }
      break;

    case FileType::LoadList:
    case FileType::BatchList:
    case FileType::FileList:
      switch (static_cast< MediaFileFormatVersion>( formatVersionField))
      {
        case MediaFileFormatVersion::Version1:
          return Arinc665Version::ARINC_665_1;

        case MediaFileFormatVersion::Version2:
          return Arinc665Version::ARINC_665_2;

        case MediaFileFormatVersion::Version34:
          return Arinc665Version::ARINC_665_4;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return Arinc665Version::Invalid;
}

uint16_t Arinc665File::getFormatVersionField(
  const FileType fileType,
  const Arinc665Version arinc665Version)
{
  switch (fileType)
  {
    case FileType::BatchFile:
      switch (arinc665Version)
      {
        case Arinc665Version::ARINC_665_1:
          return static_cast< uint16_t>( BatchFileFormatVersion::Version1);

        case Arinc665Version::ARINC_665_2:
          return static_cast< uint16_t>( BatchFileFormatVersion::Version2);

        case Arinc665Version::ARINC_665_3:
        case Arinc665Version::ARINC_665_4:
          return static_cast< uint16_t>( BatchFileFormatVersion::Version34);

        default:
          break;
      }
      break;

    case FileType::LoadUploadHeader:
      switch (arinc665Version)
      {
        case Arinc665Version::ARINC_665_1:
          return static_cast< uint16_t>( LoadFileFormatVersion::Version1);

        case Arinc665Version::ARINC_665_2:
          return static_cast< uint16_t>( LoadFileFormatVersion::Version2);

        case Arinc665Version::ARINC_665_3:
        case Arinc665Version::ARINC_665_4:
          return static_cast< uint16_t>( LoadFileFormatVersion::Version34);

        default:
          break;
      }
      break;

    case FileType::LoadList:
    case FileType::BatchList:
    case FileType::FileList:
      switch (arinc665Version)
      {
        case Arinc665Version::ARINC_665_1:
          return static_cast< uint16_t>( MediaFileFormatVersion::Version1);

        case Arinc665Version::ARINC_665_2:
          return static_cast< uint16_t>( MediaFileFormatVersion::Version2);

        case Arinc665Version::ARINC_665_3:
        case Arinc665Version::ARINC_665_4:
          return static_cast< uint16_t>( MediaFileFormatVersion::Version34);

        default:
          break;
      }
      break;

    default:
      break;
  }

  return 0xFFFFU;
}

Arinc665::FileType Arinc665File::getFileType( const path &filename)
{
  std::string filenameN = filename.filename().string();

  if ( filenameN == ListOfLoadsName)
  {
    return FileType::LoadList;
  }

  if ( filenameN == ListOfBatchesName)
  {
    return FileType::BatchList;
  }

  if ( filenameN == ListOfFilesName)
  {
    return FileType::FileList;
  }

  std::string extension = filename.extension().string();

  if ( extension == LoadUploadHeaderExtension)
  {
    return FileType::LoadUploadHeader;
  }

  if ( extension == BatchFileExtension)
  {
    return FileType::BatchFile;
  }

  return FileType::Invalid;
}

Arinc665File& Arinc665File::operator=( const RawFile &rawFile)
{
  decodeHeader( rawFile);
  return *this;
}

Arinc665File::operator RawFile() const
{
  return encode();
}

FileType Arinc665File::getFileType() const
{
  return fileType;
}

Arinc665Version Arinc665File::getArincVersion() const
{
  return arinc665Version;
}

void Arinc665File::setArincVersion( Arinc665Version version)
{
  this->arinc665Version = version;
}

uint16_t Arinc665File::getCrc() const noexcept
{
  return crc;
}

void Arinc665File::setCrc( const uint16_t crc) noexcept
{
  this->crc = crc;
}

void Arinc665File::calculateCrc()
{
  auto rawFile( encode());

  const uint16_t calculatedCrc = calculateChecksum( rawFile, checksumPosition);

  crc = calculatedCrc;
}

Arinc665File::Arinc665File(
  const FileType fileType,
  Arinc665Version version,
  const std::size_t checksumPosition) noexcept :
  fileType( fileType),
  checksumPosition( checksumPosition),
  arinc665Version( version),
  crc( 0)
{
}

Arinc665File::Arinc665File(
  FileType fileType,
  const RawFile &rawFile,
  std::size_t checksumPosition) :
  fileType( fileType),
  checksumPosition( checksumPosition)
{
  decodeHeader( rawFile);
}

Arinc665File& Arinc665File::operator=( const Arinc665File &file)
{
  assert( this->checksumPosition == file.checksumPosition);

  if ( this == &file)
  {
    return *this;
  }

  crc = file.crc;

  return *this;
}

void Arinc665File::insertHeader( RawFile &file) const
{
  // Check file size
  if ( file.size() <= BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File When file is to small
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "File to small"));
  }

  // Check file size
  if ( file.size() % 2 != 0)
  {
    //! @throw InvalidArinc665File When file size is invalid
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "Invalid size"));
  }

  auto it( file.begin());

  // file size
  it = setInt< uint32_t>( it, file.size() / 2);

  // format version
  it = setInt< uint16_t>(
    it,
    static_cast< uint16_t>( getFormatVersionField( fileType, arinc665Version)));

  // spare
  it = setInt< uint16_t>( it, 0U);

  // crc
  setInt< uint16_t>( file.end() - checksumPosition, crc);
}

void Arinc665File::decodeHeader( const RawFile &rawFile)
{
  // Check file size
  if ( rawFile.size() <= BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File When file is to small
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "File to small"));
  }

  auto it( rawFile.begin());

  // check size field
  uint32_t fileLength;
  it = getInt< uint32_t>( it, fileLength);

  if ( fileLength * 2 != rawFile.size())
  {
    //! @throw InvalidArinc665File When file size field is invalid
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "file size invalid"));
  }

  // format version
  uint16_t formatVersion;
  it = getInt< uint16_t>( it, formatVersion);

  arinc665Version = getArinc665Version( fileType, formatVersion);

  // check format field version
  if ( arinc665Version == Arinc665Version::Invalid)
  {
    //! @throw InvalidArinc665File When file format is wrong
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "wrong file format"));
  }

  // spare
  uint16_t spare;
  it = getInt< uint16_t>( it, spare);

  // Decode checksum field;
  uint16_t crc;
  getInt< uint16_t>( rawFile.end() - checksumPosition, crc);
  setCrc( crc);

  // calculate checksum and compare against stored
  uint16_t calcCrc = calculateChecksum( rawFile, checksumPosition);
  if ( crc != calcCrc)
  {
    //! @throw InvalidArinc665File When CRC is invalid
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "Invalid Checksum"));
  }
}

}
}
