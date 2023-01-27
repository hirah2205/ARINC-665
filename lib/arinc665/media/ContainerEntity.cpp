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
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

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

DirectoryPtr ContainerEntity::addSubdirectory( std::string name )
{
  if ( subdirectory( name ) || file( name ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "subdirectory or file with name already exists" } );
  }

  // create, emplace and return directory
  return subdirectoriesV.emplace_back( std::make_shared< Directory >(
    std::dynamic_pointer_cast< ContainerEntity >( shared_from_this() ),
    std::move( name ) ) );
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
      << Helper::AdditionalInfo{ "subdirectory does not exists" }
      << boost::errinfo_file_name{ std::string{ name } } );
  }

  if ( (*dir)->hasChildren() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "subdirectory is not empty" }
      << boost::errinfo_file_name{ std::string{ name } } );
  }

  subdirectoriesV.erase( dir );
}

void ContainerEntity::removeSubdirectory( const DirectoryPtr& subdirectory )
{
  const auto dir{ std::ranges::find( subdirectoriesV, subdirectory ) };

  if ( subdirectoriesV.end() == dir )
   {
     BOOST_THROW_EXCEPTION( Arinc665Exception()
       << Helper::AdditionalInfo{ "subdirectory does not exists" }
       << boost::errinfo_file_name{ std::string{ subdirectory->name() } } );
   }

   if ( (*dir)->hasChildren() )
   {
     BOOST_THROW_EXCEPTION( Arinc665Exception()
       << Helper::AdditionalInfo{ "subdirectory is not empty" }
       << boost::errinfo_file_name{ std::string{ subdirectory->name() } } );
   }

   subdirectoriesV.erase( dir );
}

size_t ContainerEntity::numberOfFiles() const
{
  return filesV.size();
}

size_t ContainerEntity::recursiveNumberOfFiles() const
{
  size_t numberOfFilesRecursive{ numberOfFiles() };

  for ( const auto &subdirectory : subdirectories() )
  {
    numberOfFilesRecursive += subdirectory->recursiveNumberOfFiles();
  }

  return numberOfFilesRecursive;
}

ConstFiles ContainerEntity::files() const
{
  return ConstFiles{ filesV.begin(), filesV.end() };
}

Files ContainerEntity::files()
{
  return filesV;
}

ConstFiles ContainerEntity::recursiveFiles() const
{
  ConstFiles filesRecursive{ files() };

  for ( const auto &subdirectory : subdirectories() )
  {
    filesRecursive.splice(
      filesRecursive.begin(),
      subdirectory->recursiveFiles() );
  }

  return filesRecursive;
}

Files ContainerEntity::recursiveFiles()
{
  Files filesRecursive{ files() };

  for ( const auto &subdirectory : subdirectories() )
  {
    filesRecursive.splice(
      filesRecursive.begin(),
      subdirectory->recursiveFiles() );
  }

  return filesRecursive;
}

ConstFiles ContainerEntity::recursiveFiles( std::string_view filename ) const
{
  ConstFiles filesRecursive{};

  if ( auto fileFound{ file( filename ) }; fileFound )
  {
    filesRecursive.emplace_back( std::move( fileFound ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    filesRecursive.splice(
      filesRecursive.end(),
      subdirectory->recursiveFiles( filename ) );
  }

  return filesRecursive;
}

Files ContainerEntity::recursiveFiles( std::string_view filename )
{
  Files filesRecursive{};

  if ( auto fileFound{ file( filename ) }; fileFound )
  {
    filesRecursive.emplace_back( std::move( fileFound ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    filesRecursive.splice(
      filesRecursive.end(),
      subdirectory->recursiveFiles( filename ) );
  }

  return filesRecursive;
}

ConstFilePtr ContainerEntity::file( std::string_view filename ) const
{
  for ( auto & file : filesV )
  {
    if ( file->name() == filename )
    {
      return file;
    }
  }

  return {};
}

FilePtr ContainerEntity::file( std::string_view filename )
{
  for ( const auto &file : filesV )
  {
    if ( file->name() == filename )
    {
      return file;
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

  // check when file is load, if it is part of batch
  if ( ( FileType::LoadFile == (*file)->fileType() )
    && !mediaSet()->batchesWithLoad(
      std::dynamic_pointer_cast< const Load >( *file) ).empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Load is part of Batch" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  // check when file is regular file, if it is part of load
  if ( ( FileType::RegularFile == (*file)->fileType() )
    && !mediaSet()->loadsWithFile(
      std::dynamic_pointer_cast< const RegularFile >( *file ) ).empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Regular File is part of Load" }
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

  // check when file is load, if it is part of batch
  if ( ( FileType::LoadFile == (*fileIt)->fileType() )
    && !mediaSet()->batchesWithLoad(
      std::dynamic_pointer_cast< const Load >( *fileIt ) ).empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Load is part of Batch" }
      << boost::errinfo_file_name{ std::string{ file->name() } } );
  }

  // check when file is regular file, if it is part of load
  if ( ( FileType::RegularFile == (*fileIt)->fileType() )
    && !mediaSet()->loadsWithFile(
      std::dynamic_pointer_cast< const RegularFile >( *fileIt ) ).empty() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Regular File is part of Load" }
      << boost::errinfo_file_name{ std::string{ file->name() } } );
  }

  filesV.erase( fileIt );
}

size_t ContainerEntity::numberOfRegularFiles() const
{
  return numberOfFiles( FileType::RegularFile );
}

size_t ContainerEntity::recursiveNumberOfRegularFiles() const
{
  size_t numberOfRegularFilesRecursive{ numberOfRegularFiles() };

  for ( const auto &subdirectory : subdirectories() )
  {
    numberOfRegularFilesRecursive +=
      subdirectory->recursiveNumberOfRegularFiles();
  }

  return numberOfRegularFilesRecursive;
}

ConstRegularFiles ContainerEntity::regularFiles() const
{
  return filesPerType< ConstRegularFiles, FileType::RegularFile>();
}

RegularFiles ContainerEntity::regularFiles()
{
  return filesPerType< RegularFiles, FileType::RegularFile>();
}

ConstRegularFiles ContainerEntity::recursiveRegularFiles() const
{
  ConstRegularFiles regularFilesRecursive{ regularFiles() };

  for ( const auto &subdirectory : subdirectories() )
  {
    regularFilesRecursive.splice(
      regularFilesRecursive.begin(),
      subdirectory->recursiveRegularFiles() );
  }

  return regularFilesRecursive;
}

RegularFiles ContainerEntity::recursiveRegularFiles()
{
  RegularFiles regularFilesRecursive{ regularFiles() };

  for ( const auto &subdirectory : subdirectories() )
  {
    regularFilesRecursive.splice(
      regularFilesRecursive.begin(),
      subdirectory->recursiveRegularFiles() );
  }

  return regularFilesRecursive;
}

ConstRegularFiles ContainerEntity::recursiveRegularFiles(
  std::string_view filename ) const
{
  ConstRegularFiles regularFilesRecursive{};

  if ( auto foundRegularFiles{ regularFile( filename ) };
    foundRegularFiles )
  {
    regularFilesRecursive.emplace_back( std::move( foundRegularFiles ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    regularFilesRecursive.splice(
      regularFilesRecursive.end(),
      subdirectory->recursiveRegularFiles( filename ) );
  }

  return regularFilesRecursive;
}

RegularFiles ContainerEntity::recursiveRegularFiles( std::string_view filename )
{
  RegularFiles regularFilesRecursive{};

  if ( auto foundRegularFiles{ regularFile( filename ) };
    foundRegularFiles )
  {
    regularFilesRecursive.emplace_back( std::move( foundRegularFiles ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    regularFilesRecursive.splice(
      regularFilesRecursive.end(),
      subdirectory->recursiveRegularFiles( filename ) );
  }

  return regularFilesRecursive;
}

ConstRegularFilePtr ContainerEntity::regularFile(
  std::string_view filename ) const
{
  return
    filePerType< ConstRegularFilePtr, FileType::RegularFile >( filename );
}

RegularFilePtr ContainerEntity::regularFile( std::string_view filename )
{
  return filePerType< RegularFilePtr, FileType::RegularFile >( filename );
}

RegularFilePtr ContainerEntity::addRegularFile( std::string filename )
{
  if ( subdirectory( filename ) || file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File or directory with name already exists" }
      << boost::errinfo_file_name{ filename } );
  }

  // create file
  auto file{ std::make_shared< RegularFile >(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    std::move( filename ) ) };

  // create, emplace and return file
  filesV.push_back( file );

  // return new file
  return file;
}

size_t ContainerEntity::numberOfLoads() const
{
  return numberOfFiles( FileType::LoadFile );
}

size_t ContainerEntity::recursiveNumberOfLoads() const
{
  size_t numberOfLoadsRecursive{ numberOfLoads() };

  for ( const auto &subdirectory : subdirectories() )
  {
    numberOfLoadsRecursive += subdirectory->recursiveNumberOfLoads();
  }

  return numberOfLoadsRecursive;
}

ConstLoads ContainerEntity::loads() const
{
  return filesPerType< ConstLoads, FileType::LoadFile>();
}

Loads ContainerEntity::loads()
{
  return filesPerType< Loads, FileType::LoadFile>();
}

ConstLoads ContainerEntity::recursiveLoads() const
{
  ConstLoads loadsRecursive{ loads() };

  for ( const auto &subdirectory : subdirectories() )
  {
    loadsRecursive.splice(
      loadsRecursive.begin(),
      subdirectory->recursiveLoads() );
  }

  return loadsRecursive;
}

Loads ContainerEntity::recursiveLoads()
{
  Loads loadsRecursive{ loads() };

  for ( const auto &subdirectory : subdirectories() )
  {
    loadsRecursive.splice(
      loadsRecursive.begin(),
      subdirectory->recursiveLoads() );
  }

  return loadsRecursive;
}

ConstLoads ContainerEntity::recursiveLoads( std::string_view filename ) const
{
  ConstLoads loadsRecursive{};

  if ( auto foundLoad{ load( filename ) }; foundLoad )
  {
    loadsRecursive.emplace_back( std::move( foundLoad ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    loadsRecursive.splice(
      loadsRecursive.end(),
      subdirectory->recursiveLoads( filename ) );
  }

  return loadsRecursive;
}

Loads ContainerEntity::recursiveLoads( std::string_view filename )
{
  Loads loadsRecursive{};

  if ( auto foundLoad{ load( filename ) }; foundLoad )
  {
    loadsRecursive.emplace_back( std::move( foundLoad ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    loadsRecursive.splice(
      loadsRecursive.end(),
      subdirectory->recursiveLoads( filename ) );
  }

  return loadsRecursive;
}

ConstLoadPtr ContainerEntity::load( std::string_view filename ) const
{
  return filePerType< ConstLoadPtr, FileType::LoadFile >( filename );
}

LoadPtr ContainerEntity::load( std::string_view filename )
{
  return filePerType< LoadPtr, FileType::LoadFile >( filename );
}

LoadPtr ContainerEntity::addLoad( std::string filename )
{
  if ( subdirectory( filename ) || file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File or directory with this name already exists" } );
  }

  // create file
  auto load{ std::make_shared< Load>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    std::move( filename ) ) };

  // insert into map
  filesV.push_back( load );

  // return new file
  return load;
}

size_t ContainerEntity::numberOfBatches() const
{
  return numberOfFiles( FileType::BatchFile );
}

size_t ContainerEntity::recursiveNumberOfBatches() const
{
  size_t numberOfBatchesRecursive{ numberOfBatches() };

  for ( const auto &subdirectory : subdirectories() )
  {
    numberOfBatchesRecursive += subdirectory->recursiveNumberOfBatches();
  }

  return numberOfBatchesRecursive;
}

ConstBatches ContainerEntity::batches() const
{
  return filesPerType< ConstBatches, FileType::BatchFile>();
}

Batches ContainerEntity::batches()
{
  return filesPerType< Batches, FileType::BatchFile>();
}

ConstBatches ContainerEntity::recursiveBatches() const
{
  ConstBatches batchesRecursive{ batches() };

  for ( const auto &subdirectory : subdirectories() )
  {
    batchesRecursive.splice(
      batchesRecursive.begin(),
      subdirectory->recursiveBatches() );
  }

  return batchesRecursive;
}

Batches ContainerEntity::recursiveBatches()
{
  Batches batchesRecursive{ batches() };

  for ( const auto &subdirectory : subdirectories() )
  {
    batchesRecursive.splice(
      batchesRecursive.begin(),
      subdirectory->recursiveBatches() );
  }

  return batchesRecursive;
}

ConstBatches ContainerEntity::recursiveBatches(
  std::string_view filename ) const
{
  ConstBatches batchesRecursive{};

  if ( auto foundBatch{ batch( filename ) }; foundBatch )
  {
    batchesRecursive.emplace_back( std::move( foundBatch ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    batchesRecursive.splice(
      batchesRecursive.end(),
      subdirectory->recursiveBatches( filename ) );
  }

  return batchesRecursive;
}

Batches ContainerEntity::recursiveBatches( std::string_view filename )
{
  Batches batchesRecursive{};

  if ( auto foundBatch{ batch( filename ) }; foundBatch )
  {
    batchesRecursive.emplace_back( std::move( foundBatch ) );
  }

  for ( const auto &subdirectory : subdirectories() )
  {
    batchesRecursive.splice(
      batchesRecursive.end(),
      subdirectory->recursiveBatches( filename ) );
  }

  return batchesRecursive;
}

ConstBatchPtr ContainerEntity::batch( std::string_view filename ) const
{
  return filePerType< ConstBatchPtr, FileType::BatchFile >( filename );
}

BatchPtr ContainerEntity::batch( std::string_view filename )
{
  return filePerType< BatchPtr, FileType::BatchFile >( filename );
}

BatchPtr ContainerEntity::addBatch( std::string filename )
{
  if ( subdirectory( filename ) || file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File or directory with this name already exists" } );
  }

  // create file
  auto batch{ std::make_shared< Batch>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    std::move( filename ) ) };

  // insert into map
  filesV.push_back( batch );

  // return new file
  return batch;
}

size_t ContainerEntity::numberOfFiles( const FileType fileType ) const
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

template< typename FilesT, FileType fileType >
FilesT ContainerEntity::filesPerType() const
{
  FilesT result{};

  for ( auto & file : filesV )
  {
    if ( file->fileType() == fileType )
    {
      result.push_back(
        std::dynamic_pointer_cast< typename FilesT::value_type::element_type >(
          file ) );
    }
  }

  return result;
}

template< typename FilesT, FileType fileType >
FilesT ContainerEntity::filesPerType()
{
  FilesT result{};

  for ( const auto &file : filesV )
  {
    if ( file->fileType() == fileType )
    {
      result.push_back(
        std::dynamic_pointer_cast< typename FilesT::value_type::element_type >(
          file ) );
    }
  }

  return result;
}

template< typename FilesT, FileType fileType >
FilesT ContainerEntity::filePerType( std::string_view filename ) const
{
  const auto filePtr{ file( filename ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != fileType )
  {
    return {};
  }

  return std::dynamic_pointer_cast< typename FilesT::element_type >( filePtr );
}

template< typename FilesT, FileType fileType >
FilesT ContainerEntity::filePerType( std::string_view filename )
{
  const auto filePtr{ file( filename ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != fileType )
  {
    return {};
  }

  return std::dynamic_pointer_cast< typename FilesT::element_type >( filePtr );
}

}
