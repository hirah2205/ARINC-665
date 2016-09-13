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
 * @brief Definition of class Arinc665::Media::Load.
 **/

#include "Load.hpp"

namespace Arinc665 {
namespace Media {

Load::Load( ContainerEntityPtr parent, const string &name) :
  BaseFile( parent, name)
{
}

Load::FileType Load::getFileType( void) const
{
  return FileType::LoadFile;
}

const Load::ThwIdList& Load::getTargetHardwareIdList( void) const
{
  return targetHardwareIdList;
}

Load::ThwIdList& Load::getTargetHardwareIdList( void)
{
  return targetHardwareIdList;
}

void Load::setTargetHardwareIdList( const ThwIdList& thwIdList)
{
  this->targetHardwareIdList = thwIdList;
}

const Load::FileList& Load::getDataFiles( void) const
{
  return dataFileList;
}

void Load::addDataFile( const WeakFilePtr dataFile)
{
  dataFileList.push_back( dataFile);
}

const Load::FileList& Load::getSupportFiles( void) const
{
  return supportFileList;
}

void Load::addSupportFile( const WeakFilePtr supportFile)
{
  supportFileList.push_back( supportFile);
}

const std::vector< uint8_t>& Load::getUserDefinedData( void) const
{
  return userDefinedData;
}

void Load::setUserDefinedData( const std::vector< uint8_t> &userDefinedData)
{
  this->userDefinedData = userDefinedData;
}

}
}
