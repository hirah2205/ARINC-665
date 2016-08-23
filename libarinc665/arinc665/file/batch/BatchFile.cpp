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
 * @brief Definition of class Arinc665::File::BatchFile.
 **/

#include "BatchFile.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665 {
namespace File {

BatchFile::BatchFile( const RawFile &file):
  Arinc665File( file, Arinc665FileFormatVersion::LOAD_FILE_VERSION_2)
{
  // set processing start to position after spare
  RawFile::const_iterator it = file.begin() + BaseHeaderOffset;

  uint32_t batchPartNumberPtr;
  it = getInt< uint32_t>( it, batchPartNumberPtr);

  uint32_t targetHardwareIdListPtr;
  it = getInt< uint32_t>( it, targetHardwareIdListPtr);

  // load part number
  it = file.begin() + batchPartNumberPtr * 2;
  it = getString( it, partNumber);

  // target hardware id list
  it = file.begin() + targetHardwareIdListPtr * 2;

  // file crc decoded and checked within base class
}

Arinc665Version BatchFile::getArincVersion( void) const
{
  return Arinc665Version::ARINC_665_2;
}

BatchFile::string BatchFile::getPartNumber( void) const
{
  return partNumber;
}

void BatchFile::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

BatchFile::string BatchFile::getComment( void) const
{
  return comment;
}

void BatchFile::setComment( const string &comment)
{
  this->comment = comment;
}

const BatchFile::ListType& BatchFile::getTargetHardwareLoadList( void) const
{
  return targetHardwareLoadList;
}

BatchFile::ListType& BatchFile::getTargetHardwareLoadList( void)
{
  return targetHardwareLoadList;
}

}
}
