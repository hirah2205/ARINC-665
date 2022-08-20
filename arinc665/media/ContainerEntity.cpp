/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::ContainerEntity.
 **/

#include "ContainerEntity.hpp"

#include <arinc665/media/Directory.hpp>
#include <arinc665/media/Medium.hpp>

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665::Media {

bool ContainerEntity::hasChildren() const
{
  return !subdirectoriesV.empty() || !filesV.empty();
}

size_t ContainerEntity::numberOfSubdirectories() const
{
  return subdirectoriesV.size();
}

ConstDirectories ContainerEntity::subdirectories() const
{
  return ConstDirectories{ subdirectoriesV.begin(), subdirectoriesV.end() };
}

Directories ContainerEntity::subdirectories()
{
  return subdirectoriesV;
}

ConstDirectoryPtr ContainerEntity::subdirectory( std::string_view name ) const
{
  for ( const auto &subdirectory : subdirectoriesV )
  {
    if ( subdirectory->name() == name )
    {
      return subdirectory;
    }
  }

  return {};
}

DirectoryPtr ContainerEntity::subdirectory( std::string_view name )
{
  for ( const auto &subdirectory : subdirectoriesV )
  {
    if ( subdirectory->name() == name )
    {
      return subdirectory;
    }
  }

  return {};
}

DirectoryPtr ContainerEntity::addSubdirectory( std::string_view name )
{
  if ( subdirectory( name ) || file( name ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "subdirectory or file with name already exists" } );
  }

  // create, emplace and return directory
  return subdirectoriesV.emplace_back( std::make_shared< Directory >(
    std::dynamic_pointer_cast< ContainerEntity >( shared_from_this() ),
    name ) );
}

void ContainerEntity::removeSubdirectory( std::string_view name )
{
  const auto dir{ std::ranges::find_if(
    subdirectoriesV,
    [ &name ]( const DirectoryPtr &dirEnt ){
      return ( name == dirEnt->name() );
    } ) };

  if ( subdirectoriesV.end() == dir )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "subdirectory does not exists" } );
  }

  subdirectoriesV.erase( dir );
}

void ContainerEntity::removeSubdirectory( const DirectoryPtr& subdirectory )
{
  const auto dir{ std::ranges::find( subdirectoriesV, subdirectory ) };

  if ( subdirectoriesV.end() == dir )
   {
     BOOST_THROW_EXCEPTION( Arinc665Exception()
       << Helper::AdditionalInfo{ "subdirectory does not exists" } );
   }

   subdirectoriesV.erase( dir );
}

size_t ContainerEntity::numberOfFiles( const bool recursive ) const
{
  auto fileSize{ filesV.size() };

  // descent to sub directories if requested
  if ( recursive )
  {
    for ( const auto &subdirectory : subdirectoriesV )
    {
      fileSize += subdirectory->numberOfFiles( true );
    }
  }

  return fileSize;
}

ConstFiles ContainerEntity::files( const bool recursive ) const
{
  if ( !recursive )
  {
    return ConstFiles{ filesV.begin(), filesV.end() };
  }

  ConstFiles allfiles{ filesV.begin(), filesV.end() };

  for ( const auto &subdirectory : subdirectoriesV )
  {
    allfiles.splice(
      allfiles.begin(),
      std::const_pointer_cast< const Directory >( subdirectory )->files( true ) );
  }

  return allfiles;
}

Files ContainerEntity::files( const bool recursive )
{
  if ( !recursive )
  {
    return filesV;
  }

  Files allFiles{ filesV };

  for ( const auto &subdirectory : subdirectoriesV )
  {
    allFiles.splice( allFiles.begin(), subdirectory->files( true ) );
  }

  return allFiles;
}

ConstFilePtr ContainerEntity::file(
  std::string_view filename,
  const bool recursive ) const
{
  for ( auto & file : filesV )
  {
    if ( file->name() == filename )
    {
      return file;
    }
  }

  if ( recursive )
  {
    for ( const auto & subdirectory : subdirectoriesV )
    {
      // if file has been found return immediately
      if ( const auto file{ subdirectory->file( filename, true ) }; file )
      {
        return file;
      }
    }
  }

  return {};
}

FilePtr ContainerEntity::file(
  std::string_view filename,
  const bool recursive )
{
  for ( const auto &file : filesV )
  {
    if ( file->name() == filename )
    {
      return file;
    }
  }

  if ( recursive )
  {
    for ( const auto & subdirectory : subdirectoriesV )
    {
      // if file has been found return immediately
      if ( const auto file{ subdirectory->file( filename, true ) }; file )
      {
        return file;
      }
    }
  }

  return {};
}

void ContainerEntity::removeFile( std::string_view filename )
{
  const auto file{ std::ranges::find_if(
    filesV,
    [ &filename ]( const FilePtr &dirEnt ){
      return ( filename == dirEnt->name() );
    } ) };

  if ( filesV.end() == file )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  filesV.erase( file );
}

void ContainerEntity::removeFile( const ConstFilePtr& file )
{
  const auto fileIt{ std::ranges::find( filesV, file ) };

  if ( filesV.end() == fileIt )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ std::string{ file->name() } } );
  }

  filesV.erase( fileIt );
}

RegularFilePtr ContainerEntity::addRegularFile( std::string_view filename )
{
  if ( subdirectory( filename ) || file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File or directory with name already exists" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  // create file
  const auto file{ std::make_shared< RegularFile >(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    filename ) };

  // create, emplace and return file
  filesV.push_back( file );

  // return new file
  return file;
}

size_t ContainerEntity::numberOfLoads( const bool recursive ) const
{
  auto numberOfLoads{ files( File::FileType::LoadFile ).size() };

  // descent to sub directories if requested
  if ( recursive )
  {
    for ( const auto &subdirectory : subdirectoriesV )
    {
      numberOfLoads += subdirectory->numberOfLoads( true );
    }
  }

  return numberOfLoads;
}

ConstLoads ContainerEntity::loads( const bool recursive ) const
{
  auto loadFiles{ files( File::FileType::LoadFile ) };

  ConstLoads loads{};

  for ( const auto &loadFile : loadFiles )
  {
    loads.push_back( std::dynamic_pointer_cast< const Load>( loadFile ) );
  }

  if ( recursive )
  {
    for ( const auto &subdirectory : subdirectoriesV )
    {
      auto subLoads{
        static_cast< const Directory&>(*subdirectory).loads( true ) };

      loads.insert( loads.end(), subLoads.begin(), subLoads.end() );
    }
  }

  return loads;
}

Loads ContainerEntity::loads( const bool recursive )
{
  auto loadFiles{ files( File::FileType::LoadFile ) };

  Loads loads{};

  for ( const auto &loadFile : loadFiles )
  {
    loads.push_back( std::dynamic_pointer_cast< Load>( loadFile ) );
  }

  if ( recursive )
  {
    for ( const auto &subdirectory : subdirectoriesV )
    {
      auto subLoads{ subdirectory->loads( true ) };

      loads.insert( loads.end(), subLoads.begin(), subLoads.end() );
    }
  }

  return loads;
}

ConstLoadPtr ContainerEntity::load(
  std::string_view filename,
  const bool recursive ) const
{
  const auto filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != File::FileType::LoadFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< const Load>( filePtr );
}

LoadPtr ContainerEntity::load( std::string_view filename, const bool recursive )
{
  auto filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != File::FileType::LoadFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< Load>( filePtr );
}

LoadPtr ContainerEntity::addLoad( std::string_view filename )
{
  if ( subdirectory( filename ) || file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File or directory with this name already exists" } );
  }

  // create file
  auto load{ std::make_shared< Load>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    filename ) };

  // insert into map
  filesV.push_back( load );

  // return new file
  return load;
}

size_t ContainerEntity::numberOfBatches( const bool recursive) const
{
  size_t numberOfBatches( files( File::FileType::BatchFile).size() );

  // descent to sub directories if requested
  if ( recursive )
  {
    for ( const auto &subdirectory : subdirectoriesV )
    {
      numberOfBatches += subdirectory->numberOfBatches( true);
    }
  }

  return numberOfBatches;
}

ConstBatches ContainerEntity::batches( const bool recursive) const
{
  const auto batchFiles{ files( File::FileType::BatchFile ) };

  ConstBatches batches;

  // add batch files to list
  for ( const auto &batchFile : batchFiles )
  {
    const auto realBatchFile{ std::dynamic_pointer_cast< const Batch>( batchFile ) };
    assert( realBatchFile );
    batches.push_back( realBatchFile );
  }

  // iterate over subdirectories
  if ( recursive )
  {
    for ( auto &subdirectory : subdirectoriesV )
    {
      const auto subBatches{ std::const_pointer_cast< const Directory>(
        subdirectory )->batches( true ) };

      batches.insert( batches.end(), subBatches.begin(), subBatches.end() );
    }
  }

  return batches;
}

Batches ContainerEntity::batches( const bool recursive)
{
  const auto batchFiles{ files( File::FileType::BatchFile ) };

  Batches batches{};

  // add batch files to list
  for ( const auto &batchFile : batchFiles )
  {
    const auto realBatchFile{ std::dynamic_pointer_cast< Batch>( batchFile ) };
    assert( realBatchFile );
    batches.push_back( realBatchFile );
  }

  // iterate over subdirectories
  if ( recursive )
  {
    for ( const auto &subdirectory : subdirectoriesV )
    {
      const auto subBatches{ subdirectory->batches( true ) };

      batches.insert( batches.end(), subBatches.begin(), subBatches.end() );
    }
  }

  return batches;
}

ConstBatchPtr ContainerEntity::batch(
  std::string_view filename,
  const bool recursive ) const
{
  const auto filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != File::FileType::BatchFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< const Batch>( filePtr );
}

BatchPtr ContainerEntity::batch(
  std::string_view filename,
  const bool recursive )
{
  const FilePtr filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != File::FileType::BatchFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< Batch>( filePtr );
}

BatchPtr ContainerEntity::addBatch( std::string_view filename )
{
  if ( subdirectory( filename ) || file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File or directory with this name already exists" } );
  }

  // create file
  const auto batch{ std::make_shared< Batch>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    filename ) };

  // insert into map
  filesV.push_back( batch );

  // return new file
  return batch;
}

ConstFiles ContainerEntity::files( FileType fileType ) const
{
  ConstFiles result{};

  for ( auto & file : filesV )
  {
    if ( file->fileType() == fileType )
    {
      result.push_back( file );
    }
  }

  return result;
}

Files ContainerEntity::files( FileType fileType )
{
  Files result{};

  for ( const auto &file : filesV )
  {
    if ( file->fileType() == fileType )
    {
      result.push_back( file );
    }
  }

  return result;
}

}
