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
 * @brief Definition of class Arinc665::Media::Directory.
 **/

#include "ContainerEntity.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/media/Directory.hpp>

namespace Arinc665{
namespace Media {

bool ContainerEntity::hasChildren( void) const
{
  return !subDirectories.empty() || !files.empty();
}

size_t ContainerEntity::getNumberOfSubDirectories( void) const
{
  return subDirectories.size();
}

ConstDirectories ContainerEntity::getSubDirectories( void) const
{
  return ConstDirectories( subDirectories.begin(), subDirectories.end());
}

Directories ContainerEntity::getSubDirectories( void)
{
  return subDirectories;
}

ConstDirectoryPtr ContainerEntity::getSubDirectory( const string &name) const
{
  for (const auto &subDirectory : subDirectories)
  {
    if (subDirectory->getName() == name)
    {
      return subDirectory;
    }
  }

  return ConstDirectoryPtr();
}

DirectoryPtr ContainerEntity::getSubDirectory( const string &name)
{
  for (const auto &subDirectory : subDirectories)
  {
    if (subDirectory->getName() == name)
    {
      return subDirectory;
    }
  }

  return DirectoryPtr();
}

DirectoryPtr ContainerEntity::addSubDirectory( const string &name)
{
  if ( getSubDirectory( name))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "sub-directory already exists"));
  }

  // create new sub-directory
  DirectoryPtr subDirectory( std::make_shared< Directory>(
    shared_from_this(),
    name));

  // insert into map
  subDirectories.push_back( subDirectory);

  // return new sub-directory
  return subDirectory;
}

void ContainerEntity::removeSubDirectory( const string &name)
{
  Directories::iterator dir = std::find_if(
    subDirectories.begin(),
    subDirectories.end(),
    [&name]( const DirectoryPtr &dirEnt){
      return (name == dirEnt->getName());
    });

  if (subDirectories.end() == dir)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "sub-directory does not exists"));
  }

  subDirectories.erase( dir);
}

void ContainerEntity::removeSubDirectory( DirectoryPtr subDirectory)
{
  Directories::iterator dir = std::find(
    subDirectories.begin(),
    subDirectories.end(),
    subDirectory);

  if (subDirectories.end() == dir)
   {
     BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
       AdditionalInfo( "sub-directory does not exists"));
   }

   subDirectories.erase( dir);
}

size_t ContainerEntity::getNumberOfFiles( bool recursive) const
{
  size_t fileSize( files.size());

  // descent to sub directories if requested
  if ( recursive)
  {
    for ( const auto &subDirectory : subDirectories)
    {
      fileSize += subDirectory->getNumberOfFiles( true);
    }
  }

  return fileSize;
}

ConstFiles ContainerEntity::getFiles( bool recursive) const
{
  if (!recursive)
  {
    return ConstFiles( files.begin(), files.end());
  }

  ConstFiles allfiles( files.begin(), files.end());

  for (const auto &subDirectory : subDirectories)
  {
    ConstFiles subFiles( static_cast< const Directory>(*subDirectory).getFiles( true));

    allfiles.insert( allfiles.begin(), subFiles.begin(), subFiles.end());
  }

  return allfiles;
}

Files ContainerEntity::getFiles( bool recursive)
{
  if (!recursive)
  {
    return files;
  }

  Files allFiles( files);

  for ( auto &subDirectory : subDirectories)
  {
    Files subFiles( subDirectory->getFiles( true));

    allFiles.insert( allFiles.begin(), subFiles.begin(), subFiles.end());
  }

  return allFiles;
  }

  ConstFilePtr ContainerEntity::getFile( const string &filename, bool recursive) const
  {
  for ( auto & file : files)
  {
    if ( file->getName() == filename)
    {
      return file;
    }
  }

  if (recursive)
  {
    for (const auto & subDirectory : subDirectories)
    {
      ConstFilePtr file = subDirectory->getFile( filename, true);

      // if file has been found return immediately
      if (file)
      {
        return file;
      }
    }
  }

  return ConstFilePtr();
}

FilePtr ContainerEntity::getFile( const string &filename, bool recursive)
{
  for ( auto & file : files)
  {
    if ( file->getName() == filename)
    {
      return file;
    }
  }

  if (recursive)
  {
    for (const auto & subDirectory : subDirectories)
    {
      FilePtr file = subDirectory->getFile( filename, true);

      // if file has been found return immediately
      if (file)
      {
        return file;
      }
    }
  }

  return FilePtr();
}

FilePtr ContainerEntity::addFile( const string &filename)
{
  if ( getFile( filename))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "File already exists"));
  }

  // create file
  FilePtr file( std::make_shared< File>( shared_from_this(), filename));

  // insert into map
  files.push_back( file);

  // return new file
  return file;
}

void ContainerEntity::removeFile( const string &filename)
{
  Files::iterator file = std::find_if(
    files.begin(),
    files.end(),
    [&filename]( const FilePtr &dirEnt){
      return (filename == dirEnt->getName());
    });

  if (files.end() == file)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File not found"));
  }

  files.erase( file);
}

void ContainerEntity::removeFile( ConstFilePtr file)
{
  Files::iterator fileIt = std::find(
    files.begin(),
    files.end(),
    file);

  if (files.end() == fileIt)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File not found"));
  }

  files.erase( fileIt);
}

size_t ContainerEntity::getNumberOfLoads( bool recursive) const
{
  size_t numberOfLoads( getFiles( BaseFile::FileType::LoadFile).size());

  // descent to sub directories if requested
  if (recursive)
  {
    for (const auto &subDirectory : subDirectories)
    {
      numberOfLoads += subDirectory->getNumberOfLoads( true);
    }
  }

  return numberOfLoads;
}

ConstLoads ContainerEntity::getLoads( bool recursive) const
{
  ConstFiles loadFiles = getFiles( BaseFile::FileType::LoadFile);

  ConstLoads loads;

  for ( auto &loadFile : loadFiles)
  {
    loads.push_back( std::dynamic_pointer_cast< const Load>( loadFile));
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectories)
    {
      ConstLoads subLoads( static_cast< const Directory>(*subDirectory).getLoads( true));

      loads.insert( loads.end(), subLoads.begin(), subLoads.end());
    }
  }

  return loads;
}

Loads ContainerEntity::getLoads( bool recursive)
{
  Files loadFiles = getFiles( BaseFile::FileType::LoadFile);

  Loads loads;

  for ( auto &loadFile : loadFiles)
  {
    loads.push_back( std::dynamic_pointer_cast< Load>( loadFile));
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectories)
    {
      Loads subLoads( subDirectory->getLoads( true));

      loads.insert( loads.end(), subLoads.begin(), subLoads.end());
    }
  }

  return loads;
}

ConstLoadPtr ContainerEntity::getLoad( const string &filename, bool recursive) const
{
  ConstFilePtr file = getFile( filename, recursive);

  if (!file)
  {
    return ConstLoadPtr();
  }

  if (file->getFileType() != BaseFile::FileType::LoadFile)
  {
    return ConstLoadPtr();
  }

	return std::dynamic_pointer_cast< const Load>( file);
}

LoadPtr ContainerEntity::getLoad( const string &filename, bool recursive)
{
  FilePtr file = getFile( filename, recursive);

  if (!file)
  {
    return LoadPtr();
  }

  if (file->getFileType() != BaseFile::FileType::LoadFile)
  {
    return LoadPtr();
  }

  return std::dynamic_pointer_cast< Load>( file);
}

LoadPtr ContainerEntity::addLoad( const string &filename)
{
  if ( getFile( filename))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File with this name already exists"));
  }

  // create file
  LoadPtr load(
    std::make_shared< Load>( shared_from_this(), filename));

  // insert into map
  files.push_back( load);

  // return new file
  return load;
}

void ContainerEntity::removeLoad( const string &filename)
{
  FilePtr loadFile = getFile( filename);

  if ( !loadFile)
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "Load does not exists"));
  }

  if ( BaseFile::FileType::LoadFile != loadFile->getFileType())
  {
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

size_t ContainerEntity::getNumberOfBatches( bool recursive) const
{
  size_t numberOfBatches( getFiles( BaseFile::FileType::BatchFile).size());

  // descent to sub directories if requested
  if ( recursive)
  {
    for ( const auto &subDirectory : subDirectories)
    {
      numberOfBatches += subDirectory->getNumberOfBatches( true);
    }
  }

  return numberOfBatches;
}

ConstBatches ContainerEntity::getBatches( bool recursive) const
{
  ConstFiles batchFiles = getFiles( BaseFile::FileType::LoadFile);

  ConstBatches batches;

  for ( auto &batchFile : batchFiles)
  {
    batches.push_back( std::dynamic_pointer_cast< const Batch>( batchFile));
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectories)
    {
      ConstBatches subBatches( static_cast< const Directory>(*subDirectory).getBatches( true));

      batches.insert( batches.end(), subBatches.begin(), subBatches.end());
    }
  }

  return batches;
}

Batches ContainerEntity::getBatches( bool recursive)
{
  Files batchFiles = getFiles( BaseFile::FileType::LoadFile);

  Batches batches;

  for ( auto &batchFile : batchFiles)
  {
    batches.push_back( std::dynamic_pointer_cast< Batch>( batchFile));
  }

  if (recursive)
  {
    for (const auto &subDirectory : subDirectories)
    {
      Batches subBatches( subDirectory->getBatches( true));

      batches.insert( batches.end(), subBatches.begin(), subBatches.end());
    }
  }

  return batches;
}

ConstBatchPtr ContainerEntity::getBatch( const string &filename, bool recursive) const
{
  ConstFilePtr file = getFile( filename, recursive);

  if (!file)
  {
    return ConstBatchPtr();
  }

  if (file->getFileType() != BaseFile::FileType::BatchFile)
  {
    return ConstBatchPtr();
  }

  return std::dynamic_pointer_cast< const Batch>( file);
}

BatchPtr ContainerEntity::getBatch( const string &filename, bool recursive)
{
  FilePtr file = getFile( filename, recursive);

  if (!file)
  {
    return BatchPtr();
  }

  if (file->getFileType() != BaseFile::FileType::BatchFile)
  {
    return BatchPtr();
  }

  return std::dynamic_pointer_cast< Batch>( file);
}

BatchPtr ContainerEntity::addBatch( const string &filename)
{
  if ( getFile( filename))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File with this name already exists"));
  }

  // create file
  BatchPtr batch(
    std::make_shared< Batch>( shared_from_this(), filename));

  // insert into map
  files.push_back( batch);

  // return new file
  return batch;
}

void ContainerEntity::removeBatch( const string &filename)
{
  FilePtr batchFile = getFile( filename);

  if (!batchFile)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "Batch does not exists"));
  }

  if (BaseFile::FileType::BatchFile != batchFile->getFileType())
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "File does not name a batch"));
  }

  removeFile( batchFile);
}

void ContainerEntity::removeBatch( ConstBatchPtr batch)
{
  removeFile( batch);
}

ContainerEntityPtr ContainerEntity::getParent( void)
{
  return parent.lock();
}

ConstContainerEntityPtr ContainerEntity::getParent( void) const
{
  return parent.lock();
}

ContainerEntity::ContainerEntity( ContainerEntityPtr parent):
  parent( parent)
{
}

ConstFiles ContainerEntity::getFiles( BaseFile::FileType fileType) const
{
  ConstFiles result;

  for ( auto & file : files)
  {
    if ( file->getFileType() == fileType)
    {
      result.push_back( file);
    }
  }

  return result;
}

Files ContainerEntity::getFiles( BaseFile::FileType fileType)
{
  Files result;

  for ( auto & file : files)
  {
    if ( file->getFileType() == fileType)
    {
      result.push_back( file);
    }
  }

  return result;
}

void ContainerEntity::setParent( ContainerEntityPtr parent)
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

  if (this->parent.lock() == parent)
  {
    return;
  }

  this->parent = parent;
}

}
}
