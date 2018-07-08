/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Media::Directory.
 **/

#include "ContainerEntity.hpp"

#include <arinc665/media/Directory.hpp>
#include <arinc665/media/Medium.hpp>

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665::Media {

bool ContainerEntity::hasChildren() const
{
  return !subDirectoriesValue.empty() || !filesValue.empty();
}

size_t ContainerEntity::numberOfSubDirectories() const
{
  return subDirectoriesValue.size();
}

ConstDirectories ContainerEntity::subDirectories() const
{
  return ConstDirectories( subDirectoriesValue.begin(), subDirectoriesValue.end());
}

Directories ContainerEntity::subDirectories()
{
  return subDirectoriesValue;
}

ConstDirectoryPtr ContainerEntity::subDirectory( const std::string &name) const
{
  for (const auto &subDirectory : subDirectoriesValue)
  {
    if (subDirectory->name() == name)
    {
      return subDirectory;
    }
  }

  return ConstDirectoryPtr();
}

DirectoryPtr ContainerEntity::subDirectory( const std::string &name)
{
  for (const auto &subDirectory : subDirectoriesValue)
  {
    if (subDirectory->name() == name)
    {
      return subDirectory;
    }
  }

  return DirectoryPtr();
}

DirectoryPtr ContainerEntity::addSubDirectory( const std::string &name)
{
  if ( subDirectory( name))
  {
    //! @throw Arinc665Exception() if directory already exists.
    BOOST_THROW_EXCEPTION(
      Arinc665Exception() << AdditionalInfo( "sub-directory already exists"));
  }

  // create new sub-directory
  DirectoryPtr subDirectory( std::make_shared< Directory>(
    shared_from_this(),
    name));

  // insert into map
  subDirectoriesValue.push_back( subDirectory);

  // return new sub-directory
  return subDirectory;
}

void ContainerEntity::removeSubDirectory( const std::string &name)
{
  Directories::iterator dir = std::find_if(
    subDirectoriesValue.begin(),
    subDirectoriesValue.end(),
    [&name]( const DirectoryPtr &dirEnt){
      return (name == dirEnt->name());
    });

  if (subDirectoriesValue.end() == dir)
  {
    //! @throw Arinc665Exception() if directory does not exists.
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "sub-directory does not exists"));
  }

  subDirectoriesValue.erase( dir);
}

void ContainerEntity::removeSubDirectory( DirectoryPtr subDirectory)
{
  Directories::iterator dir = std::find(
    subDirectoriesValue.begin(),
    subDirectoriesValue.end(),
    subDirectory);

  if (subDirectoriesValue.end() == dir)
   {
     //! @throw Arinc665Exception() if directory does not exists.
     BOOST_THROW_EXCEPTION( Arinc665Exception() <<
       AdditionalInfo( "sub-directory does not exists"));
   }

  subDirectoriesValue.erase( dir);
}

size_t ContainerEntity::numberOfFiles( const bool recursive) const
{
  size_t fileSize( filesValue.size());

  // descent to sub directories if requested
  if ( recursive)
  {
    for ( const auto &subDirectory : subDirectoriesValue)
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
    return ConstFiles( filesValue.begin(), filesValue.end());
  }

  ConstFiles allfiles( filesValue.begin(), filesValue.end());

  for (const auto &subDirectory : subDirectoriesValue)
  {
    ConstFiles subFiles(
      static_cast< const Directory&>(*subDirectory).files( true));

    allfiles.insert( allfiles.begin(), subFiles.begin(), subFiles.end());
  }

  return allfiles;
}

Files ContainerEntity::files( const bool recursive)
{
  if (!recursive)
  {
    return filesValue;
  }

  Files allFiles( filesValue);

  for ( auto &subDirectory : subDirectoriesValue)
  {
    Files subFiles( subDirectory->files( true));

    allFiles.insert( allFiles.begin(), subFiles.begin(), subFiles.end());
  }

  return allFiles;
}

ConstFilePtr ContainerEntity::file(
  const std::string &filename,
  const bool recursive) const
{
  for ( auto & file : filesValue)
  {
    if ( file->name() == filename)
    {
      return file;
    }
  }

  if (recursive)
  {
    for (const auto & subDirectory : subDirectoriesValue)
    {
      ConstFilePtr file = subDirectory->file( filename, true);

      // if file has been found return immediately
      if (file)
      {
        return file;
      }
    }
  }

  return {};
}

FilePtr ContainerEntity::file( const std::string &filename, const bool recursive)
{
  for ( auto & file : filesValue)
  {
    if ( file->name() == filename)
    {
      return file;
    }
  }

  if (recursive)
  {
    for (const auto & subDirectory : subDirectoriesValue)
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

FilePtr ContainerEntity::addFile( const std::string &filename)
{
  if ( file( filename))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "File already exists"));
  }

  // create file
  FilePtr file( std::make_shared< File>( shared_from_this(), filename));

  // insert into map
  filesValue.push_back( file);

  // return new file
  return file;
}

void ContainerEntity::removeFile( const std::string &filename)
{
  Files::iterator file = std::find_if(
    filesValue.begin(),
    filesValue.end(),
    [&filename]( const FilePtr &dirEnt){
      return (filename == dirEnt->name());
    });

  if (filesValue.end() == file)
  {
    //! @throw Arinc665Exception() if file does not exists.
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File not found"));
  }

  filesValue.erase( file);
}

void ContainerEntity::removeFile( ConstFilePtr file)
{
  Files::iterator fileIt = std::find(
    filesValue.begin(),
    filesValue.end(),
    file);

  if (filesValue.end() == fileIt)
  {
    //! @throw Arinc665Exception() if file does not exists.
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File not found"));
  }

  filesValue.erase( fileIt);
}

size_t ContainerEntity::numberOfLoads( const bool recursive) const
{
  size_t numberOfLoads( files( BaseFile::FileType::LoadFile).size());

  // descent to sub directories if requested
  if (recursive)
  {
    for (const auto &subDirectory : subDirectoriesValue)
    {
      numberOfLoads += subDirectory->numberOfLoads( true);
    }
  }

  return numberOfLoads;
}

ConstLoads ContainerEntity::loads( const bool recursive) const
{
  ConstFiles loadFiles = files( BaseFile::FileType::LoadFile);

  ConstLoads loads;

  for ( auto &loadFile : loadFiles)
  {
    loads.push_back( std::dynamic_pointer_cast< const Load>( loadFile));
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectoriesValue)
    {
      ConstLoads subLoads(
        static_cast< const Directory&>(*subDirectory).loads( true));

      loads.insert( loads.end(), subLoads.begin(), subLoads.end());
    }
  }

  return loads;
}

Loads ContainerEntity::loads( const bool recursive)
{
  Files loadFiles = files( BaseFile::FileType::LoadFile);

  Loads loads;

  for ( auto &loadFile : loadFiles)
  {
    loads.push_back( std::dynamic_pointer_cast< Load>( loadFile));
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectoriesValue)
    {
      Loads subLoads( subDirectory->loads( true));

      loads.insert( loads.end(), subLoads.begin(), subLoads.end());
    }
  }

  return loads;
}

ConstLoadPtr ContainerEntity::load(
  const std::string &filename,
  const bool recursive) const
{
  ConstFilePtr filePtr = file( filename, recursive);

  if (!filePtr)
  {
    return {};
  }

  if (filePtr->fileType() != BaseFile::FileType::LoadFile)
  {
    return {};
  }

  return std::dynamic_pointer_cast< const Load>( filePtr);
}

LoadPtr ContainerEntity::load( const std::string &filename, const bool recursive)
{
  FilePtr filePtr = file( filename, recursive);

  if (!filePtr)
  {
    return LoadPtr();
  }

  if (filePtr->fileType() != BaseFile::FileType::LoadFile)
  {
    return LoadPtr();
  }

  return std::dynamic_pointer_cast< Load>( filePtr);
}

LoadPtr ContainerEntity::addLoad( const std::string &filename)
{
  if ( file( filename))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File with this name already exists"));
  }

  // create file
  LoadPtr load(
    std::make_shared< Load>( shared_from_this(), filename));

  // insert into map
  filesValue.push_back( load);

  // return new file
  return load;
}

void ContainerEntity::removeLoad( const std::string &filename)
{
  FilePtr loadFile = file( filename);

  if ( !loadFile)
  {
    //! @throw Arinc665Exception() if load does not exists.
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "Load does not exists"));
  }

  if ( BaseFile::FileType::LoadFile != loadFile->fileType())
  {
    //! @throw Arinc665Exception() if filename does not address a load.
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
        << AdditionalInfo( "File does not name a load"));
  }

  removeFile( loadFile);
}

void ContainerEntity::removeLoad( ConstLoadPtr load)
{
  removeFile( load);
}

size_t ContainerEntity::numberOfBatches( const bool recursive) const
{
  size_t numberOfBatches( files( BaseFile::FileType::BatchFile).size());

  // descent to sub directories if requested
  if ( recursive)
  {
    for ( const auto &subDirectory : subDirectoriesValue)
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
    for (auto &subDirectory : subDirectoriesValue)
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

  Batches batches;

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
    for (auto &subDirectory : subDirectoriesValue)
    {
      auto subBatches( subDirectory->batches( true));

      batches.insert( batches.end(), subBatches.begin(), subBatches.end());
    }
  }

  return batches;
}

ConstBatchPtr ContainerEntity::batch(
  const std::string &filename,
  const bool recursive) const
{
  auto filePtr( file( filename, recursive));

  if (!filePtr)
  {
    return ConstBatchPtr();
  }

  if (filePtr->fileType() != BaseFile::FileType::BatchFile)
  {
    return ConstBatchPtr();
  }

  return std::dynamic_pointer_cast< const Batch>( filePtr);
}

BatchPtr ContainerEntity::batch( const std::string &filename, const bool recursive)
{
  FilePtr filePtr = file( filename, recursive);

  if (!filePtr)
  {
    return BatchPtr();
  }

  if (filePtr->fileType() != BaseFile::FileType::BatchFile)
  {
    return BatchPtr();
  }

  return std::dynamic_pointer_cast< Batch>( filePtr);
}

BatchPtr ContainerEntity::addBatch( const std::string &filename)
{
  if ( file( filename))
  {
    //! @throw Arinc665Exception() if batch already exists.
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File with this name already exists"));
  }

  // create file
  BatchPtr batch(
    std::make_shared< Batch>( shared_from_this(), filename));

  // insert into map
  filesValue.push_back( batch);

  // return new file
  return batch;
}

void ContainerEntity::removeBatch( const std::string &filename)
{
  FilePtr batchFile = file( filename);

  if (!batchFile)
  {
    //! @throw Arinc665Exception() if load does not exists.
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "Batch does not exists"));
  }

  if (BaseFile::FileType::BatchFile != batchFile->fileType())
  {
    //! @throw Arinc665Exception() if filename does not address a batch
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File does not name a batch"));
  }

  removeFile( batchFile);
}

void ContainerEntity::removeBatch( ConstBatchPtr batch)
{
  removeFile( batch);
}

ConstContainerEntityPtr ContainerEntity::parent() const
{
  return parentValue.lock();
}

ContainerEntityPtr ContainerEntity::parent()
{
  return parentValue.lock();
}

ConstMediumPtr ContainerEntity::medium() const
{
  if (type() == Type::Medium)
  {
    return std::dynamic_pointer_cast< const Medium>( shared_from_this());
  }

  auto parentPtr( parent());

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

MediumPtr ContainerEntity::medium()
{
  if (type() == Type::Medium)
  {
    return std::dynamic_pointer_cast< Medium>( shared_from_this());
  }

  auto parentPtr( parent());

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

ContainerEntity::ContainerEntity( ContainerEntityPtr parent):
  parentValue( parent)
{
  if (!parent)
  {
    //! @throw Arinc665::Arinc665Exception when parent is not valid
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "parent must be valid"));
  }
}

ConstFiles ContainerEntity::files( FileType fileType) const
{
  ConstFiles result;

  for ( auto & file : filesValue)
  {
    if ( file->fileType() == fileType)
    {
      result.push_back( file);
    }
  }

  return result;
}

Files ContainerEntity::files( FileType fileType)
{
  Files result;

  for ( auto & file : filesValue)
  {
    if ( file->fileType() == fileType)
    {
      result.push_back( file);
    }
  }

  return result;
}

void ContainerEntity::parent( ContainerEntityPtr parent)
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "parent must be valid"));
  }

  if (shared_from_this() == parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "Recursion not allowed"));
  }

  if (this->parent() == parent)
  {
    return;
  }

  this->parentValue = parent;
}

}
