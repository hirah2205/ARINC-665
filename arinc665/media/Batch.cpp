/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Media::Batch.
 **/

#include "Batch.hpp"

namespace Arinc665::Media {

Batch::Batch( ContainerEntityPtr parent, const std::string &name) :
  BaseFile( parent, name)
{
}

Batch::FileType Batch::fileType() const
{
  return FileType::BatchFile;
}

std::string Batch::comment() const
{
  return commentValue;
}

void Batch::comment( const std::string &comment)
{
  commentValue = comment;
}

void Batch::comment( std::string &&comment)
{
  commentValue = std::move( comment);
}

const Batch::BatchInfo& Batch::targets() const
{
  return batchesValue;
}

Batch::BatchInfo& Batch::targets()
{
  return batchesValue;
}

const WeakLoads Batch::target( const std::string &targetHardwareId) const
{
  auto it( batchesValue.find( targetHardwareId));

  if (it == batchesValue.end())
  {
    return {};
  }

  return it->second;
}

WeakLoads Batch::target( const std::string &targetHardwareId)
{
  auto it( batchesValue.find( targetHardwareId));

  if (it == batchesValue.end())
  {
    return {};
  }

  return it->second;
}

void Batch::target( const std::string &targetHardwareId, WeakLoads loads)
{
  batchesValue.insert( {targetHardwareId, loads});
}

void Batch::target( std::string &&targetHardwareId, WeakLoads loads)
{
  batchesValue.insert( {std::move( targetHardwareId), loads});
}

}
