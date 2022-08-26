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

size_t ContainerEntity::numberOfFiles() const
{
  return filesV.size();
}

ConstFiles ContainerEntity::files() const
{
  return ConstFiles{ filesV.begin(), filesV.end() };
}

Files ContainerEntity::files()
{
  return filesV;
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
      if ( auto file{ subdirectory->file( filename, true ) }; file )
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

size_t ContainerEntity::numberOfRegularFiles() const
{
  return numberOfFiles( File::FileType::RegularFile );
}

ConstRegularFiles ContainerEntity::regularFiles() const
{
  auto regularFiles{ files( File::FileType::RegularFile ) };

  ConstRegularFiles files{};

  for ( const auto &file : regularFiles )
  {
    files.emplace_back( std::dynamic_pointer_cast< const RegularFile >( file ) );
  }

  return files;
}

RegularFiles ContainerEntity::regularFiles()
{
  auto regularFiles{ files( File::FileType::RegularFile ) };

  RegularFiles files{};

  for ( const auto &file : regularFiles )
  {
    files.emplace_back( std::dynamic_pointer_cast< RegularFile >( file ) );
  }

  return files;
}

ConstRegularFilePtr ContainerEntity::regularFile(
  std::string_view filename,
  const bool recursive ) const
{
  const auto filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != File::FileType::RegularFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< const RegularFile >( filePtr );
}

RegularFilePtr ContainerEntity::regularFile(
  std::string_view filename,
  const bool recursive )
{
  const auto filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != File::FileType::RegularFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< RegularFile >( filePtr );
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
  auto file{ std::make_shared< RegularFile >(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    filename ) };

  // create, emplace and return file
  filesV.push_back( file );

  // return new file
  return file;
}

size_t ContainerEntity::numberOfLoads() const
{
  return numberOfFiles( File::FileType::LoadFile );
}

ConstLoads ContainerEntity::loads() const
{
  auto loadFiles{ files( File::FileType::LoadFile ) };

  ConstLoads loads{};

  for ( const auto &loadFile : loadFiles )
  {
    loads.push_back( std::dynamic_pointer_cast< const Load>( loadFile ) );
  }

  return loads;
}

Loads ContainerEntity::loads()
{
  auto loadFiles{ files( File::FileType::LoadFile ) };

  Loads loads{};

  for ( const auto &loadFile : loadFiles )
  {
    loads.push_back( std::dynamic_pointer_cast< Load>( loadFile ) );
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

size_t ContainerEntity::numberOfBatches() const
{
  return numberOfFiles( File::FileType::BatchFile );
}

ConstBatches ContainerEntity::batches() const
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

  return batches;
}

Batches ContainerEntity::batches()
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
  auto batch{ std::make_shared< Batch>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    filename ) };

  // insert into map
  filesV.push_back( batch );

  // return new file
  return batch;
}

size_t ContainerEntity::numberOfFiles( FileType fileType ) const
{
  size_t numberOfFiles{ 0U };

  for ( auto & file : filesV )
  {
    if ( file->fileType() == fileType )
    {
      numberOfFiles++;
    }
  }

  return numberOfFiles;
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
