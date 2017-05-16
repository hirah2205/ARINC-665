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
 * @brief Definition of class Arinc665::Media::Batch.
 **/

#include "Batch.hpp"

namespace Arinc665 {
namespace Media {

Batch::Batch( ContainerEntityPtr parent, const string &name) :
  BaseFile( parent, name)
{
}

Batch::FileType Batch::getFileType() const
{
  return FileType::BatchFile;
}

Batch::string Batch::getComment() const
{
  return comment;
}

void Batch::setComment( const string &comment)
{
  this->comment = comment;
}

const WeakLoads Batch::getTarget( const string &targetHardwareId) const
{
  auto it( batches.find( targetHardwareId));

  if (it == batches.end())
  {
    return {};
  }

  return it->second;
}

WeakLoads Batch::getTarget( const string &targetHardwareId)
{
  auto it( batches.find( targetHardwareId));

  if (it == batches.end())
  {
    return {};
  }

  return it->second;
}

void Batch::addTarget( const string &targetHardwareId, WeakLoads loads)
{
  batches.insert( {targetHardwareId, loads});
}

const Batch::BatchInfo& Batch::getTargets() const
{
  return batches;
}

Batch::BatchInfo& Batch::getTargets()
{
  return batches;
}

}
}
