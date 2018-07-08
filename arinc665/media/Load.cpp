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

namespace Arinc665::Media {

Load::Load( ContainerEntityPtr parent, const std::string &name) :
  BaseFile( parent, name)
{
}

Load::Load( ContainerEntityPtr parent, std::string &&name) :
  BaseFile( parent, std::move( name))
{
}

Load::FileType Load::fileType() const
{
  return FileType::LoadFile;
}

const Load::TargetHardwareIds& Load::targetHardwareIds() const
{
  return targetHardwareIdsValue;
}

Load::TargetHardwareIds& Load::targetHardwareIds()
{
  return targetHardwareIdsValue;
}

void Load::targetHardwareIds( const TargetHardwareIds &thwIds)
{
  targetHardwareIdsValue = thwIds;
}

void Load::targetHardwareIds( TargetHardwareIds &&thwIds)
{
  targetHardwareIdsValue = std::move( thwIds);
}

const Load::Files& Load::dataFiles() const
{
  return dataFilesValue;
}

void Load::addDataFile( const WeakFilePtr dataFile)
{
  dataFilesValue.push_back( dataFile);
}

const Load::Files& Load::supportFiles() const
{
  return supportFilesValue;
}

void Load::addSupportFile( const WeakFilePtr supportFile)
{
  supportFilesValue.push_back( supportFile);
}

const std::vector< uint8_t>& Load::userDefinedData() const
{
  return userDefinedDataValue;
}

void Load::userDefinedData( const std::vector< uint8_t> &userDefinedData)
{
  userDefinedDataValue = userDefinedData;
}

void Load::userDefinedData( Load::UserDefinedData &&userDefinedData)
{
  userDefinedDataValue= std::move( userDefinedData);
}

}
