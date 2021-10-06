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
  return !subDirectoriesV.empty() || !filesV.empty();
}

size_t ContainerEntity::numberOfSubDirectories() const
{
  return subDirectoriesV.size();
}

ConstDirectories ContainerEntity::subDirectories() const
{
  return ConstDirectories( subDirectoriesV.begin(), subDirectoriesV.end());
}

Directories ContainerEntity::subDirectories()
{
  return subDirectoriesV;
}

ConstDirectoryPtr ContainerEntity::subDirectory( std::string_view name) const
{
  for (const auto &subDirectory : subDirectoriesV )
  {
    if (subDirectory->name() == name)
    {
      return subDirectory;
    }
  }

  return ConstDirectoryPtr();
}

DirectoryPtr ContainerEntity::subDirectory( std::string_view name)
{
  for (const auto &subDirectory : subDirectoriesV )
  {
    if (subDirectory->name() == name)
    {
      return subDirectory;
    }
  }

  return DirectoryPtr();
}

DirectoryPtr ContainerEntity::addSubDirectory( std::string_view name)
{
  if ( subDirectory( name))
  {
    //! @throw Arinc665Exception() if directory already exists.
    BOOST_THROW_EXCEPTION(Arinc665Exception()
      << Helper::AdditionalInfo( "sub-directory already exists"));
  }

  // create new sub-directory
  auto subDirectory{ std::make_shared< Directory>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this()),
    name)};

  // insert into map
  subDirectoriesV.push_back( subDirectory);

  // return new sub-directory
  return subDirectory;
}

void ContainerEntity::removeSubDirectory( std::string_view name)
{
  auto dir{ std::find_if(
    subDirectoriesV.begin(),
    subDirectoriesV.end(),
    [&name]( const DirectoryPtr &dirEnt){
      return (name == dirEnt->name());
    })};

  if ( subDirectoriesV.end() == dir)
  {
    //! @throw Arinc665Exception() if directory does not exists.
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "sub-directory does not exists"));
  }

  subDirectoriesV.erase( dir);
}

void ContainerEntity::removeSubDirectory( const DirectoryPtr& subDirectory)
{
  auto dir{ std::find( subDirectoriesV.begin(), subDirectoriesV.end(),
    subDirectory)};

  if ( subDirectoriesV.end() == dir)
   {
     //! @throw Arinc665Exception() if directory does not exists.
     BOOST_THROW_EXCEPTION( Arinc665Exception()
       << Helper::AdditionalInfo( "sub-directory does not exists"));
   }

   subDirectoriesV.erase( dir);
}

size_t ContainerEntity::numberOfFiles( const bool recursive) const
{
  auto fileSize{ filesV.size()};

  // descent to sub directories if requested
  if ( recursive)
  {
    for ( const auto &subDirectory : subDirectoriesV )
    {
      fileSize += subDirectory->numberOfFiles( true);
    }
  }

  return fileSize;
}

ConstFiles ContainerEntity::files( const bool recursive) const
{
  if (!recursive)
  {
    return ConstFiles( filesV.begin(), filesV.end() );
  }

  ConstFiles allfiles{ filesV.begin(), filesV.end() };

  for ( const auto &subDirectory : subDirectoriesV )
  {
    ConstFiles subFiles(
      static_cast< const Directory&>(*subDirectory).files( true));

    allfiles.insert( allfiles.begin(), subFiles.begin(), subFiles.end());
  }

  return allfiles;
}

Files ContainerEntity::files( const bool recursive)
{
  if ( !recursive)
  {
    return filesV;
  }

  Files allFiles{ filesV };

  for ( auto &subDirectory : subDirectoriesV )
  {
    Files subFiles( subDirectory->files( true));

    allFiles.insert( allFiles.begin(), subFiles.begin(), subFiles.end());
  }

  return allFiles;
}

ConstFilePtr ContainerEntity::file(
  std::string_view filename,
  const bool recursive) const
{
  for ( auto & file : filesV )
  {
    if ( file->name() == filename)
    {
      return file;
    }
  }

  if ( recursive)
  {
    for (const auto & subDirectory : subDirectoriesV )
    {
      auto file{ subDirectory->file( filename, true)};

      // if file has been found return immediately
      if ( file)
      {
        return file;
      }
    }
  }

  return {};
}

FilePtr ContainerEntity::file( std::string_view filename, const bool recursive)
{
  for ( auto & file : filesV )
  {
    if ( file->name() == filename)
    {
      return file;
    }
  }

  if (recursive)
  {
    for ( const auto & subDirectory : subDirectoriesV )
    {
      FilePtr file = subDirectory->file( filename, true);

      // if file has been found return immediately
      if (file)
      {
        return file;
      }
    }
  }

  return {};
}

FilePtr ContainerEntity::addFile( std::string_view filename )
{
  if ( file( filename ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File already exists" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  // create file
  auto file{ std::make_shared< File>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this() ),
    filename ) };

  // insert into map
  filesV.push_back( file );

  // return new file
  return file;
}

void ContainerEntity::removeFile( std::string_view filename )
{
  auto file{ std::find_if(
    filesV.begin(),
    filesV.end(),
    [&filename]( const FilePtr &dirEnt){
      return (filename == dirEnt->name());
    })};

  if ( filesV.end() == file )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  filesV.erase( file);
}

void ContainerEntity::removeFile( const ConstFilePtr& file )
{
  auto fileIt{
    std::find( filesV.begin(), filesV.end(), file ) };

  if ( filesV.end() == fileIt )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File not found" } );
  }

  filesV.erase( fileIt);
}

size_t ContainerEntity::numberOfLoads( const bool recursive ) const
{
  auto numberOfLoads{ files( BaseFile::FileType::LoadFile).size()};

  // descent to sub directories if requested
  if ( recursive )
  {
    for ( const auto &subDirectory : subDirectoriesV )
    {
      numberOfLoads += subDirectory->numberOfLoads( true );
    }
  }

  return numberOfLoads;
}

ConstLoads ContainerEntity::loads( const bool recursive ) const
{
  auto loadFiles{ files( BaseFile::FileType::LoadFile ) };

  ConstLoads loads{};

  for ( auto &loadFile : loadFiles)
  {
    loads.push_back( std::dynamic_pointer_cast< const Load>( loadFile ) );
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectoriesV )
    {
      auto subLoads{
        static_cast< const Directory&>(*subDirectory).loads( true)};

      loads.insert( loads.end(), subLoads.begin(), subLoads.end());
    }
  }

  return loads;
}

Loads ContainerEntity::loads( const bool recursive )
{
  auto loadFiles{ files( BaseFile::FileType::LoadFile ) };

  Loads loads{};

  for ( auto &loadFile : loadFiles)
  {
    loads.push_back( std::dynamic_pointer_cast< Load>( loadFile));
  }

  if ( recursive)
  {
    for (const auto &subDirectory : subDirectoriesV )
    {
      auto subLoads{ subDirectory->loads( true)};

      loads.insert( loads.end(), subLoads.begin(), subLoads.end());
    }
  }

  return loads;
}

ConstLoadPtr ContainerEntity::load(
  std::string_view filename,
  const bool recursive ) const
{
  auto filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return {};
  }

  if ( filePtr->fileType() != BaseFile::FileType::LoadFile )
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
    return LoadPtr();
  }

  if ( filePtr->fileType() != BaseFile::FileType::LoadFile )
  {
    return {};
  }

  return std::dynamic_pointer_cast< Load>( filePtr );
}

LoadPtr ContainerEntity::addLoad( std::string_view filename)
{
  if ( file( filename))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      Helper::AdditionalInfo( "File with this name already exists"));
  }

  // create file
  auto load{ std::make_shared< Load>(
    std::dynamic_pointer_cast< ContainerEntity>( shared_from_this()),
    filename)};

  // insert into map
  filesV.push_back( load);

  // return new file
  return load;
}

void ContainerEntity::removeLoad( std::string_view filename)
{
  auto loadFile{ file( filename)};

  if ( !loadFile)
  {
    //! @throw Arinc665Exception() if load does not exists.
    BOOST_THROW_EXCEPTION(Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "Load does not exists"));
  }

  if ( BaseFile::FileType::LoadFile != loadFile->fileType())
  {
    //! @throw Arinc665Exception() if filename does not address a load.
    BOOST_THROW_EXCEPTION(Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo( "File does not name a load"));
  }

  removeFile( loadFile);
}

void ContainerEntity::removeLoad( const ConstLoadPtr& load)
{
  removeFile( load);
}

size_t ContainerEntity::numberOfBatches( const bool recursive) const
{
  size_t numberOfBatches( files( BaseFile::FileType::BatchFile).size());

  // descent to sub directories if requested
  if ( recursive)
  {
    for ( const auto &subDirectory : subDirectoriesV )
    {
      numberOfBatches += subDirectory->numberOfBatches( true);
    }
  }

  return numberOfBatches;
}

ConstBatches ContainerEntity::batches( const bool recursive) const
{
  auto batchFiles( files( BaseFile::FileType::BatchFile));

  ConstBatches batches;

  // add batch files to list
  for ( auto &batchFile : batchFiles)
  {
    auto realBatchFile( std::dynamic_pointer_cast< const Batch>( batchFile));
    assert( realBatchFile);
    batches.push_back( realBatchFile);
  }

  // iterate over sub-directories
  if (recursive)
  {
    for (auto &subDirectory : subDirectoriesV )
    {
      auto subBatches(std::const_pointer_cast< const Directory>(
        subDirectory)->batches( true));

      batches.insert( batches.end(), subBatches.begin(), subBatches.end());
    }
  }

  return batches;
}

Batches ContainerEntity::batches( const bool recursive)
{
  auto batchFiles( files( BaseFile::FileType::BatchFile));

  Batches batches{};

  // add batch files to list
  for ( auto &batchFile : batchFiles)
  {
    auto realBatchFile( std::dynamic_pointer_cast< Batch>( batchFile));
    assert( realBatchFile);
    batches.push_back( realBatchFile);
  }

  // iterate over sub-directories
  if (recursive)
  {
    for (auto &subDirectory : subDirectoriesV )
    {
      auto subBatches{ subDirectory->batches( true)};

      batches.insert( batches.end(), subBatches.begin(), subBatches.end());
    }
  }

  return batches;
}

ConstBatchPtr ContainerEntity::batch(
  std::string_view filename,
  const bool recursive) const
{
  auto filePtr{ file( filename, recursive)};

  if ( !filePtr)
  {
    return ConstBatchPtr();
  }

  if (filePtr->fileType() != BaseFile::FileType::BatchFile)
  {
    return ConstBatchPtr();
  }

  return std::dynamic_pointer_cast< const Batch>( filePtr);
}

BatchPtr ContainerEntity::batch(
  std::string_view filename,
  const bool recursive)
{
  FilePtr filePtr{ file( filename, recursive ) };

  if ( !filePtr )
  {
    return BatchPtr();
  }

  if ( filePtr->fileType() != BaseFile::FileType::BatchFile )
  {
    return BatchPtr();
  }

  return std::dynamic_pointer_cast< Batch>( filePtr );
}

BatchPtr ContainerEntity::addBatch( std::string_view filename )
{
  if ( file( filename ) )
  {
    //! @throw Arinc665Exception() if batch already exists.
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File with this name already exists" } );
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

void ContainerEntity::removeBatch( std::string_view filename )
{
  FilePtr batchFile{ file( filename ) };

  if ( !batchFile )
  {
    //! @throw Arinc665Exception() if load does not exists.
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Batch does not exists" } );
  }

  if ( BaseFile::FileType::BatchFile != batchFile->fileType() )
  {
    //! @throw Arinc665Exception() if filename does not address a batch
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File does not name a batch" } );
  }

  removeFile( batchFile );
}

void ContainerEntity::removeBatch( const ConstBatchPtr& batch )
{
  removeFile( batch );
}

ConstFiles ContainerEntity::files( FileType fileType ) const
{
  ConstFiles result;

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
  Files result;

  for ( auto & file : filesV )
  {
    if ( file->fileType() == fileType )
    {
      result.push_back( file );
    }
  }

  return result;
}

}
