/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Files::Arinc665File.
 **/

#include "Arinc665File.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Crc.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::Files {

ConstRawFileSpan::iterator Arinc665File::decodeString(
  ConstRawFileSpan::iterator it,
  std::string &str )
{
  // determine string length
  uint16_t strLength{};
  it = Helper::getInt< uint16_t>( it, strLength);

  // copy string
  str.assign( it, it + strLength);
  it += strLength;

  // if string is odd skipp filled 0-character
  if ( strLength % 2 == 1)
  {
    ++it;
  }

  return it;
}

RawFile Arinc665File::encodeString( std::string_view str )
{
  RawFile rawString( sizeof( uint16_t ));

  auto it{ rawString.begin() };

  // set string length
  it = Helper::setInt< uint16_t>( it, Helper::safeCast< uint16_t>( str.size()));

  // copy string
  rawString.insert( it, str.begin(), str.end());

  // fill string if it is odd
  if ( str.size() % 2 == 1)
  {
    rawString.push_back( 0U);
  }

  return rawString;
}

ConstRawFileSpan::iterator Arinc665File::decodeStrings(
  ConstRawFileSpan::iterator it,
  std::list< std::string > &strings)
{
  // number of strings
  uint16_t numberOfEntries{};
  it = Helper::getInt< uint16_t>( it, numberOfEntries);

  for ( uint16_t index = 0U; index < numberOfEntries; ++index)
  {
    // string
    std::string str{};
    it = decodeString( it, str);
    strings.emplace_back( std::move( str) );
  }

  return it;
}

ConstRawFileSpan::iterator Arinc665File::decodeStrings(
  ConstRawFileSpan::iterator it,
  std::set< std::string, std::less<> > &strings )
{
  // number of strings
  uint16_t numberOfEntries{};
  it = Helper::getInt< uint16_t>( it, numberOfEntries);

  for ( uint16_t index = 0U; index < numberOfEntries; ++index)
  {
    // string
    std::string str{};
    it = decodeString( it, str);
    strings.emplace( std::move( str) );
  }

  return it;
}

RawFile Arinc665File::encodeStrings( const std::list< std::string > &strings )
{
  RawFile rawStrings( sizeof( uint16_t));

  // set number of strings
  Helper::setInt< uint16_t>(
    rawStrings.begin(),
    static_cast< uint16_t>( strings.size()));

  for ( const auto &str : strings)
  {
    auto rawStr{ encodeString( str)};
    assert( rawStr.size() % 2 == 0);

    // append string
    rawStrings.insert( rawStrings.end(), rawStr.begin(), rawStr.end());
  }

  return rawStrings;
}

RawFile Arinc665File::encodeStrings(
  const std::set< std::string, std::less<> > &strings )
{
  RawFile rawStrings( sizeof( uint16_t));

  // set number of strings
  Helper::setInt< uint16_t>(
    rawStrings.begin(),
    static_cast< uint16_t>( strings.size()));

  for ( const auto &str : strings )
  {
    auto rawStr{ encodeString( str)};
    assert( rawStr.size() % 2 == 0);

    // append string
    rawStrings.insert( rawStrings.end(), rawStr.begin(), rawStr.end() );
  }

  return rawStrings;
}

std::string Arinc665File::encodePath( const std::filesystem::path &path )
{
  std::string convertedPath{ path.string() };

  std::replace( convertedPath.begin(), convertedPath.end(), '/', '\\' );

  // append final backslash if not present
  if ( !convertedPath.ends_with( '\\' ) )
  {
    convertedPath += '\\';
  }

  return convertedPath;
}

uint32_t Arinc665File::fileLength( const ConstRawFileSpan &file )
{
  // check file size
  if ( file.size() < BaseHeaderSize)
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "length of check code string invalid" } );
  }

  // decode the file length
  uint32_t fileLength{};
  Helper::getInt< uint32_t>( file.begin() + FileLengthFieldOffset, fileLength);

  return fileLength;
}

uint16_t Arinc665File::formatVersion( const ConstRawFileSpan &file )
{
  // check file size
  if ( file.size() < BaseHeaderSize)
  {
    //! @throw InvalidArinc665File
    //!   If the file size is to small to represent an valid ARINC 665 file.
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo( "file content to small"));
  }

  // decode the format version
  uint16_t formatVersion{};
  Helper::getInt< uint16_t>( file.begin() + FileFormatVersionFieldOffset, formatVersion);

  return formatVersion;
}

uint16_t Arinc665File::calculateChecksum(
  const ConstRawFileSpan &file,
  const std::size_t skipLastBytes)
{
  Arinc665Crc16 arincCrc16{};

  arincCrc16.process_block(
    &(*file.begin()),
    &(*file.begin()) + file.size() - skipLastBytes);

  return arincCrc16.checksum();
}

Arinc665::FileClassType Arinc665File::fileType(
  const ConstRawFileSpan &rawFile )
{
  switch (formatVersion( rawFile))
  {
    case static_cast< uint16_t>( LoadFileFormatVersion::Version2):
    case static_cast< uint16_t>( LoadFileFormatVersion::Version345 ):
      return FileClassType::LoadFile;

    case static_cast< uint16_t>( BatchFileFormatVersion::Version2):
    case static_cast< uint16_t>( BatchFileFormatVersion::Version345 ):
      return FileClassType::BatchFile;

    case static_cast< uint16_t>( MediaFileFormatVersion::Version2):
    case static_cast< uint16_t>( MediaFileFormatVersion::Version345 ):
      return FileClassType::MediaFile;

    default:
      return FileClassType::Invalid;
  }
}

Arinc665::LoadFileFormatVersion Arinc665File::loadFileFormatVersion(
  const ConstRawFileSpan &rawFile)
{
  const LoadFileFormatVersion formatVersion{
    Arinc665File::formatVersion( rawFile)};

  switch ( formatVersion)
  {
    case LoadFileFormatVersion::Version2:
    case LoadFileFormatVersion::Version345:
      break;

    default:
      return LoadFileFormatVersion::Invalid;
  }

  return formatVersion;
}

Arinc665::BatchFileFormatVersion Arinc665File::batchFileFormatVersion(
  const ConstRawFileSpan &rawFile )
{
  const BatchFileFormatVersion formatVersion{
    Arinc665File::formatVersion( rawFile)};

  switch ( formatVersion)
  {
    case BatchFileFormatVersion::Version2:
    case BatchFileFormatVersion::Version345:
      break;

    default:
      return BatchFileFormatVersion::Invalid;
  }

  return formatVersion;
}

Arinc665::MediaFileFormatVersion Arinc665File::mediaFileFormatVersion(
  const ConstRawFileSpan &rawFile )
{
  const MediaFileFormatVersion formatVersion{
    Arinc665File::formatVersion( rawFile)};

  switch ( formatVersion)
  {
    case MediaFileFormatVersion::Version2:
    case MediaFileFormatVersion::Version345:
      break;

    default:
      return MediaFileFormatVersion::Invalid;
  }

  return formatVersion;
}

SupportedArinc665Version Arinc665File::arinc665Version(
  const FileType fileType,
  const uint16_t formatVersionField)
{
  switch ( fileType)
  {
    case FileType::BatchFile:
      switch ( BatchFileFormatVersion{ formatVersionField})
      {
        case BatchFileFormatVersion::Version2:
          return SupportedArinc665Version::Supplement2;

        case BatchFileFormatVersion::Version345:
          return SupportedArinc665Version::Supplement345;

        default:
          break;
      }
      break;

    case FileType::LoadUploadHeader:
      switch ( LoadFileFormatVersion{ formatVersionField})
      {
        case LoadFileFormatVersion::Version2:
          return SupportedArinc665Version::Supplement2;

        case LoadFileFormatVersion::Version345:
          return SupportedArinc665Version::Supplement345;

        default:
          break;
      }
      break;

    case FileType::LoadList:
    case FileType::BatchList:
    case FileType::FileList:
      switch ( MediaFileFormatVersion{ formatVersionField})
      {
        case MediaFileFormatVersion::Version2:
          return SupportedArinc665Version::Supplement2;

        case MediaFileFormatVersion::Version345:
          return SupportedArinc665Version::Supplement345;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return SupportedArinc665Version::Invalid;
}

uint16_t Arinc665File::formatVersionField(
  const FileType fileType,
  const SupportedArinc665Version arinc665Version)
{
  switch ( fileType)
  {
    case FileType::BatchFile:
      switch ( arinc665Version)
      {
        case SupportedArinc665Version::Supplement2:
          return static_cast< uint16_t>( BatchFileFormatVersion::Version2);

        case SupportedArinc665Version::Supplement345:
          return static_cast< uint16_t>( BatchFileFormatVersion::Version345 );

        default:
          break;
      }
      break;

    case FileType::LoadUploadHeader:
      switch ( arinc665Version)
      {
        case SupportedArinc665Version::Supplement2:
          return static_cast< uint16_t>( LoadFileFormatVersion::Version2);

        case SupportedArinc665Version::Supplement345:
          return static_cast< uint16_t>( LoadFileFormatVersion::Version345 );

        default:
          break;
      }
      break;

    case FileType::LoadList:
    case FileType::BatchList:
    case FileType::FileList:
      switch (arinc665Version)
      {
        case SupportedArinc665Version::Supplement2:
          return static_cast< uint16_t>( MediaFileFormatVersion::Version2);

        case SupportedArinc665Version::Supplement345:
          return static_cast< uint16_t>( MediaFileFormatVersion::Version345 );

        default:
          break;
      }
      break;

    default:
      break;
  }

  return 0xFFFFU;
}

Arinc665::FileType Arinc665File::fileType(
  const std::filesystem::path &filename )
{
  const auto filenameN{ filename.filename().string()};

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

  const auto extension{ filename.extension().string()};

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

Arinc665File& Arinc665File::operator=( const ConstRawFileSpan &rawFile )
{
  decodeHeader( rawFile, fileType() );
  return *this;
}

Arinc665File::operator RawFile() const
{
  return encode();
}

SupportedArinc665Version Arinc665File::arincVersion() const
{
  return arinc665VersionV;
}

void Arinc665File::arincVersion( const SupportedArinc665Version version)
{
  arinc665VersionV = version;
}

Arinc665File::Arinc665File(
  const SupportedArinc665Version version,
  const ptrdiff_t checksumPosition ) noexcept :
  checksumPosition{ checksumPosition },
  arinc665VersionV{ version }
{
}

Arinc665File::Arinc665File(
  const ConstRawFileSpan &rawFile,
  const FileType expectedFileType,
  ptrdiff_t checksumPosition ) :
  checksumPosition{ checksumPosition },
  arinc665VersionV{ Arinc665Version::Invalid }
{
  decodeHeader( rawFile, expectedFileType );
}

Arinc665File& Arinc665File::operator=( const Arinc665File &other ) noexcept
{
  if ( this == &other)
  {
    return *this;
  }

  // if this assertion fails, we have an error within this library
  assert( this->checksumPosition == other.checksumPosition );

  arinc665VersionV = other.arinc665VersionV;
  return *this;
}

Arinc665File& Arinc665File::operator=( Arinc665File &&other) noexcept
{
  if ( this == &other)
  {
    return *this;
  }

  // if this assertion fails, we have an error within this library
  assert( this->checksumPosition == other.checksumPosition );

  arinc665VersionV = other.arinc665VersionV;
  return *this;
}

void Arinc665File::insertHeader( const RawFileSpan &rawFile) const
{
  // Check file size
  if ( rawFile.size() <= ( BaseHeaderSize + sizeof( uint16_t ) ) )
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo{ "File to small" } );
  }

  // Check file size
  if ( rawFile.size() % 2 != 0 )
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo{ "Invalid size" } );
  }

  // file size
  Helper::setInt< uint32_t>(
    rawFile.begin() + FileLengthFieldOffset,
    Helper::safeCast< uint32_t>( rawFile.size() / 2U ) );

  // format version
  Helper::setInt< uint16_t>(
    rawFile.begin() + FileFormatVersionFieldOffset,
      formatVersionField( fileType(), arinc665VersionV ) );

  // crc
  const uint16_t calculatedCrc{
    calculateChecksum( rawFile, checksumPosition)};

  Helper::setInt< uint16_t>(
    rawFile.begin() + static_cast< ptrdiff_t>( rawFile.size() ) - checksumPosition,
    calculatedCrc );
}

void Arinc665File::decodeHeader(
  const ConstRawFileSpan &rawFile,
  const FileType expectedFileType )
{
  // Check file size
  if ( rawFile.size() <= BaseHeaderSize )
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo{ "File to small" } );
  }

  // check size field
  uint32_t fileLength{};
  Helper::getInt< uint32_t>( rawFile.begin() + FileLengthFieldOffset, fileLength );

  if ( static_cast< size_t >( fileLength ) * 2U != rawFile.size() )
  {
    BOOST_THROW_EXCEPTION(InvalidArinc665File()
      << Helper::AdditionalInfo{ "file size invalid" } );
  }

  // format version
  uint16_t formatVersion{};
  Helper::getInt< uint16_t>(
    rawFile.begin() + FileFormatVersionFieldOffset,
    formatVersion );

  arinc665VersionV = arinc665Version( expectedFileType, formatVersion );

  // check format field version
  if ( arinc665VersionV == SupportedArinc665Version::Invalid )
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo{ "wrong file format" } );
  }

  // Decode checksum field;
  uint16_t crc{};
  Helper::getInt< uint16_t>(
    rawFile.begin() + static_cast< ptrdiff_t>( rawFile.size() ) - checksumPosition,
    crc );


  // calculate checksum and compare against stored
  uint16_t calcCrc = calculateChecksum( rawFile, checksumPosition );
  if ( crc != calcCrc )
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo{ "Invalid Checksum" } );
  }
}

}
