/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::FileListFile.
 **/

#include "FileListFile.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc665/file/CheckValueUtils.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665::File {

FileListFile::FileListFile( SupportedArinc665Version version):
  ListFile{ version}
{
}

FileListFile::FileListFile(
  SupportedArinc665Version version,
  std::string_view mediaSetPn,
  uint8_t mediaSequenceNumber,
  uint8_t numberOfMediaSetMembers,
  const FilesInfo &files,
  const UserDefinedData &userDefinedData ):
  ListFile{ version, mediaSetPn, mediaSequenceNumber, numberOfMediaSetMembers },
  filesV{ files },
  userDefinedDataV{ userDefinedData }
{
  checkUserDefinedData();
}

FileListFile::FileListFile(
  SupportedArinc665Version version,
  std::string &&mediaSetPn,
  uint8_t mediaSequenceNumber,
  uint8_t numberOfMediaSetMembers,
  FilesInfo &&files,
  UserDefinedData &&userDefinedData ):
  ListFile{
    version,
    std::move( mediaSetPn ),
    mediaSequenceNumber,
    numberOfMediaSetMembers },
  filesV{ std::move( files ) },
  userDefinedDataV{ std::move( userDefinedData ) }
{
  checkUserDefinedData();
}

FileListFile::FileListFile( const ConstRawFileSpan &rawFile):
  ListFile{ rawFile, FileType::FileList }
{
  decodeBody( rawFile );
}

FileListFile& FileListFile::operator=( const ConstRawFileSpan &rawFile )
{
  Arinc665File::operator =( rawFile );
  decodeBody( rawFile );

  return *this;
}

FileType FileListFile::fileType() const noexcept
{
  return FileType::FileList;
}

size_t FileListFile::numberOfFiles() const
{
  return filesV.size();
}

const FilesInfo& FileListFile::files() const
{
  return filesV;
}

FilesInfo& FileListFile::files()
{
  return filesV;
}

void FileListFile::file( const FileInfo &file )
{
  filesV.push_back( file);
}

void FileListFile::file( FileInfo &&file )
{
  filesV.push_back( std::move( file ) );
}


const FileListFile::UserDefinedData& FileListFile::userDefinedData() const
{
  return userDefinedDataV;
}

void FileListFile::userDefinedData( const UserDefinedData &userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = userDefinedData;

  checkUserDefinedData();
}

void FileListFile::userDefinedData( UserDefinedData &&userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = std::move( userDefinedData);

  checkUserDefinedData();
}

const std::optional< CheckValue>& FileListFile::checkValue() const
{
  return checkValueValue;
}

void FileListFile::checkValue( const std::optional< CheckValue> &value )
{
  checkValueValue = value;
}

void FileListFile::checkValue( std::optional< CheckValue> &&value )
{
  checkValueValue = std::move( value);
}

bool FileListFile::belongsToSameMediaSet( const FileListFile &other ) const
{
  BOOST_LOG_FUNCTION()

  if ( ( mediaSetPn() != other.mediaSetPn() )
    || ( numberOfMediaSetMembers() != other.numberOfMediaSetMembers() )
    || ( userDefinedDataV != other.userDefinedData() ) )
  {
    return false;
  }

  const auto &otherFileList( other.files() );

  if ( filesV.size() != otherFileList.size() )
  {
    return false;
  }

  for ( size_t i = 0; i < filesV.size(); ++i )
  {
    if (
      ( filesV[i].filename != otherFileList[i].filename ) ||
      ( filesV[i].pathName != otherFileList[i].pathName ) )
    {
      return false;
    }

    switch ( Arinc665File::fileType( filesV[i].filename ) )
    {
      case FileType::LoadList:
      case FileType::BatchList:
        // skip test of CRC and Member Sequence Number
        break;

      default:
        if ( ( filesV[i].crc != otherFileList[i].crc )
          || ( filesV[i].checkValue != otherFileList[i].checkValue )
          || ( filesV[i].memberSequenceNumber != otherFileList[i].memberSequenceNumber ) )
        {
          return false;
        }

        break;
    }
  }

  return true;
}

RawFile FileListFile::encode() const
{
  BOOST_LOG_FUNCTION()

  bool encodeV3Data{ false };
  std::size_t baseSize{};

  switch ( arincVersion() )
  {
    case SupportedArinc665Version::Supplement2:
      baseSize = FileHeaderSizeV2;
      break;

    case SupportedArinc665Version::Supplement345:
      encodeV3Data = true;
      baseSize = FileHeaderSizeV3;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Unsupported ARINC 665 Version" } );
  }

  RawFile rawFile( baseSize );


  // spare field
  Helper::setInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, 0U );

  // Next free Offset (used for optional pointer calculation)
  ptrdiff_t nextFreeOffset{ static_cast< ptrdiff_t>( rawFile.size() ) };


  // media set information
  const auto rawMediaInformation{ encodeMediaInformation() };
  assert( rawMediaInformation.size() % 2 == 0);
  rawFile.insert( rawFile.end(), rawMediaInformation.begin(), rawMediaInformation.end() );

  // Update Pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += static_cast< ptrdiff_t >( rawMediaInformation.size() );


  // media set files list
  const auto rawFilesInfo{ encodeFilesInfo( encodeV3Data ) };
  assert( rawFilesInfo.size() % 2 == 0 );

  Helper::setInt< uint32_t>(
    rawFile.begin() + MediaSetFilesPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += static_cast< ptrdiff_t>( rawFilesInfo.size() );

  rawFile.insert( rawFile.end(), rawFilesInfo.begin(), rawFilesInfo.end() );


  // user defined data
  assert( userDefinedDataV.size() % 2 == 0);
  uint32_t userDefinedDataPtr = 0;

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = static_cast< uint32_t >( nextFreeOffset / 2U );
    nextFreeOffset += static_cast< ptrdiff_t>( userDefinedDataV.size() );

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end() );
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr);


  // Load Check Value (only in V3 mode)
  if ( encodeV3Data )
  {
    // Alternative implementation set File Check Pointer to zero

    const auto rawCheckValue{ CheckValueUtils_encode( checkValueValue) };
    assert( rawCheckValue.size() % 2 == 0);

    Helper::setInt< uint32_t>(
      rawFile.begin() + FileCheckValuePointerFieldOffsetV3,
      static_cast< uint32_t >( nextFreeOffset / 2U ) );

    rawFile.insert(
      rawFile.end(),
      rawCheckValue.begin(),
      rawCheckValue.end() );
  }


  // Resize to final size ( File CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t ) );

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void FileListFile::decodeBody( const ConstRawFileSpan &rawFile )
{
  BOOST_LOG_FUNCTION()

  bool decodeV3Data{ false };

  switch ( arincVersion() )
  {
    case SupportedArinc665Version::Supplement2:
      break;

    case SupportedArinc665Version::Supplement345:
      decodeV3Data = true;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo( "Unsupported ARINC 665 Version"));
  }


  // Spare Field
  uint16_t spare{};
  Helper::getInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, spare);

  if ( 0U != spare )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo( "Spare is not 0" ) );
  }


  // media information pointer
  uint32_t mediaInformationPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    mediaInformationPtr);


  // file list pointer
  uint32_t fileListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + MediaSetFilesPointerFieldOffsetV2,
    fileListPtr);


  // user defined data pointer
  uint32_t userDefinedDataPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr);

  uint32_t fileCheckValuePtr = 0;

  // only decode this pointers in V3 mode
  if ( decodeV3Data )
  {
    Helper::getInt< uint32_t>(
      rawFile.begin() + FileCheckValuePointerFieldOffsetV3,
      fileCheckValuePtr);
  }

  // decode Media Information
  decodeMediaInformation( rawFile, mediaInformationPtr );

  // file list
  decodeFilesInfo(
    rawFile,
    static_cast< ptrdiff_t >( fileListPtr ) * 2,
    decodeV3Data );


  // user defined data
  if ( 0 != userDefinedDataPtr )
  {
    ptrdiff_t endOfUserDefinedData{ static_cast< ptrdiff_t>( rawFile.size() ) - DefaultChecksumPosition };

    if ( fileCheckValuePtr != 0 )
    {
      if ( fileCheckValuePtr <= userDefinedDataPtr )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "Invalid Pointers" } );
      }

      endOfUserDefinedData = static_cast< ptrdiff_t >( fileCheckValuePtr ) * 2;
    }

    userDefinedDataV.assign(
      rawFile.begin() + static_cast< ptrdiff_t >( userDefinedDataPtr ) * 2,
      rawFile.begin() + endOfUserDefinedData );
  }


  // File Check Value Field (ARINC 665-3)
  checkValueValue.reset();
  if ( decodeV3Data && ( 0U != fileCheckValuePtr ) )
  {
    checkValueValue = CheckValueUtils_decode(
      rawFile,
      2 * static_cast< ptrdiff_t >( fileCheckValuePtr ) );
  }


  // file crc decoded and checked within base class
}

RawFile FileListFile::encodeFilesInfo( const bool encodeV3Data ) const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFilesInfo( sizeof( uint16_t) );

  // Number of files must not exceed field
  if ( filesV.size() > std::numeric_limits< uint16_t>::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "More files than allowed" } );
  }

  // number of files
  Helper::setInt< uint16_t>(
    rawFilesInfo.begin(),
    static_cast< uint16_t >( filesV.size() ) );

  // iterate over files
  uint16_t fileCounter{ 0 };
  for ( auto const &fileInfo : filesV )
  {
    ++fileCounter;

    RawFile rawFileInfo( sizeof( uint16_t));

    // filename
    auto const rawFilename{ encodeString( fileInfo.filename )};
    assert( rawFilename.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawFilename.begin(),
      rawFilename.end());

    // path name
    auto const rawPathname{ encodeString( fileInfo.pathName )};
    assert( rawPathname.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawPathname.begin(),
      rawPathname.end());

    rawFileInfo.resize( rawFileInfo.size() + 2 * sizeof( uint16_t));

    // member sequence number
    auto fileInfoIt{ Helper::setInt< uint16_t>(
      rawFileInfo.begin() + static_cast< ptrdiff_t>( rawFileInfo.size() ) - ( 2 * sizeof( uint16_t ) ),
      fileInfo.memberSequenceNumber ) };

    // crc
    Helper::setInt< uint16_t>( fileInfoIt, fileInfo.crc );

    // following fields are available in ARINC 665-3 ff
    if ( encodeV3Data)
    {
      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue ) };
      assert( rawCheckValue.size() % 2 == 0);
      rawFileInfo.insert(
        rawFileInfo.end(),
        rawCheckValue.begin(),
        rawCheckValue.end());
    }

    // next file pointer (is set to 0 for last file)
    Helper::setInt< uint16_t>(
      rawFileInfo.begin(),
      (fileCounter == numberOfFiles()) ?
      0U :
      static_cast< uint16_t >( rawFileInfo.size() / 2U ) );

    // add file info to files info
    rawFilesInfo.insert( rawFilesInfo.end(), rawFileInfo.begin(), rawFileInfo.end());
  }

  return rawFilesInfo;
}

void FileListFile::decodeFilesInfo(
  const ConstRawFileSpan &rawFile,
  const ptrdiff_t offset,
  const bool decodeV3Data )
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() + offset };

  // clear potentially data
  filesV.clear();

  // number of files
  uint16_t numberOfFiles{};
  it = Helper::getInt< uint16_t>( it, numberOfFiles );

  // iterate over index
  for ( unsigned int fileIndex = 0U; fileIndex < numberOfFiles; ++fileIndex)
  {
    auto listIt{ it};

    // next file pointer
    uint16_t filePointer{};
    listIt = Helper::getInt< uint16_t>( listIt, filePointer);

    // check file pointer for validity
    if ( fileIndex != numberOfFiles - 1U )
    {
      if ( filePointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next file pointer is 0" } );
      }
    }
    else
    {
      if ( filePointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next file pointer is not 0" } );
      }
    }

    // filename
    std::string filename{};
    listIt = decodeString( listIt, filename);

    // path name
    std::string pathName{};
    listIt = decodeString( listIt, pathName);

    // member sequence number
    uint16_t memberSequenceNumber{};
    listIt = Helper::getInt< uint16_t>( listIt, memberSequenceNumber);
    if ( ( memberSequenceNumber < 1U ) || ( memberSequenceNumber > 255U ) )
    {
      BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "member sequence number out of range" } );
    }

    // crc
    uint16_t crc{};
    listIt = Helper::getInt< uint16_t>( listIt, crc);

    // CheckValue (keep default initialised if not V3 File Info
    std::optional< CheckValue> checkValue{};

    // following fields are available in ARINC 665-3 ff
    if ( decodeV3Data )
    {
      // check Value
      checkValue = CheckValueUtils_decode(
        rawFile,
        std::distance( rawFile.begin(), listIt));
    }


    // set it to begin of next file
    it += static_cast< ptrdiff_t >( filePointer ) * 2;

    filesV.emplace_back( FileInfo{
      filename,
      pathName,
      static_cast< uint8_t >( memberSequenceNumber ),
      crc,
      checkValue } );
  }
}

void FileListFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back( 0U );
  }
}

}
