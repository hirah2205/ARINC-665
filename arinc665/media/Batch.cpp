/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::Batch.
 **/

#include "Batch.hpp"

namespace Arinc665::Media {

Batch::Batch( const ContainerEntityPtr& parent, std::string_view name ) :
  File{ parent, name }
{
}

Batch::Batch( const ContainerEntityPtr& parent, std::string &&name ) :
  File{ parent, std::move( name ) }
{
}

Batch::FileType Batch::fileType() const
{
  return FileType::BatchFile;
}

std::string_view Batch::partNumber() const
{
  return partNumberV;
}

void Batch::partNumber( std::string_view partNumber )
{
  partNumberV = partNumber;
}

std::string_view Batch::comment() const
{
  return commentV;
}

void Batch::comment( std::string_view comment )
{
  commentV = comment;
}

void Batch::comment( std::string &&comment )
{
  commentV = std::move( comment);
}

const ConstBatchInfo Batch::targets() const
{
  ConstBatchInfo batchInfo{};

  for ( const auto &[ targetHardwareId, loads ] : batchesV )
  {
    batchInfo.emplace(
      targetHardwareId,
      ConstLoads{ loads.begin(), loads.end() } );
  }

  return batchInfo;
}

BatchInfo Batch::targets()
{
  BatchInfo batchInfo{};

  for ( const auto &[ targetHardwareId, loads ] : batchesV )
  {
    batchInfo.emplace(
      targetHardwareId,
      Loads{ loads.begin(), loads.end() } );
  }

  return batchInfo;
}

ConstLoads Batch::target( std::string_view targetHardwareId ) const
{
  auto it{ batchesV.find( targetHardwareId ) };

  if ( it == batchesV.end() )
  {
    return {};
  }

  return { it->second.begin(), it->second.end() };
}

Loads Batch::target( std::string_view targetHardwareId )
{
  auto it{ batchesV.find( targetHardwareId ) };

  if (it == batchesV.end())
  {
    return {};
  }

  return { it->second.begin(), it->second.end() };
}

void Batch::target( std::string_view targetHardwareId, const Loads &loads )
{
  WeakLoads weakLoads{ loads.begin(), loads.end() };
  batchesV.emplace( std::string{ targetHardwareId}, std::move( weakLoads ) );
}

void Batch::target( std::string &&targetHardwareId, const Loads &loads )
{
  WeakLoads weakLoads{ loads.begin(), loads.end() };
  batchesV.emplace( std::move( targetHardwareId), std::move( weakLoads ) );
}

}
