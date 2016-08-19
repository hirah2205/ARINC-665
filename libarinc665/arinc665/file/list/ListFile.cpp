/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Definition of class ListFile.
 **/

#include "ListFile.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

ListFile::string ListFile::getMediaSetPn( void) const
{
  return mediaSetPn;
}

void ListFile::setMediaSetPn( const string &mediaSetPn)
{
  this->mediaSetPn = mediaSetPn;
}

uint8_t ListFile::getMediaSequenceNumber( void) const
{
  return mediaSequenceNumber;
}

void ListFile::setMediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  this->mediaSequenceNumber = mediaSequenceNumber;
}

uint8_t ListFile::getNumberOfMediaSetMembers( void) const
{
  return numberOfMediaSetMembers;
}

void ListFile::setNumberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  this->numberOfMediaSetMembers = numberOfMediaSetMembers;
}

ListFile::ListFile( void)
{
}

ListFile::ListFile(
  const RawFile &file,
  const Arinc665FileFormatVersion expectedFormatVersion,
  const unsigned int skipLastBytes) :
  Arinc665File( file, expectedFormatVersion, skipLastBytes)
{
}

}
}
