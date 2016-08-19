/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class MediaSet.
 **/

#ifndef ARINC665_MEDIA_MEDIASET_HPP
#define ARINC665_MEDIA_MEDIASET_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/PartNumberdEntity.hpp>

#include <arinc665/media/Medium.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <string>

namespace Arinc665 {
namespace Media {

/**
 * @brief ARINC 665 media set definition.
 *
 * A media set consists of one or more medias.
 *
 * Each media consists of one or more files.
 *
 * @par Design considerations
 * @li where to hold the Batches and Load Headers (media or media set)?
 * @li Should be a batch and a load header a normal file? (filename and part
 * number handling) - maybe also added to media file list
 * @li remove media from class hierarchies
 **/
class MediaSet: public PartNumberdEntity
{
  public:
    using string = std::string;

    /**
     * @brief Creates a new media set.
     *
     * An empty media is automatically created.
     * This media set contains no data files, no loads and no batches.
     *
     * @param[in] partNumber
     *   Part number of the media set
     * @param[in] numberOfMedia
     *   The initial number of media in the media set (defaults to 1)
     **/
    MediaSet( const string &partNumber, const unsigned int numberOfMedia = 1);

    /**
     * @brief Get the number of medias within the media set
     *
     * @return The number of medias within the media set.
     **/
    unsigned int getNumberOfMedia( void) const;

    /**
     * @brief Returns all media.
     *
     * @return All media as map.
     **/
    const MediaMap& getMedia( void) const;

    /**
     * @brief Return the medium with the requested index.
     *
     * @param[in] index
     *   The medium index.
     *
     * @return The medium with the requested index.
     **/
    ConstMediumPtr getMedium( const unsigned int index) const;

    /**
     * @brief Return the medium with the requested index.
     *
     * @param[in] index
     *   The medium index.
     *
     * @return The medium with the requested index.
     **/
    MediumPtr getMedium( const unsigned int index);

    /**
     * @brief Adds an medium to the media set and returns the media index.
     *
     * @return The media index of the new medium.
     **/
    unsigned int addMedium( void);

    /**
     * @brief Returns all files present on the media set.
     *
     * @return All files as map.
     **/
    ConstFileMap getFiles( void) const;

    FileMap getFiles( void);

    ConstFilePtr getFile( const string &filename) const;

    FilePtr getFile( const string &filename);

    FilePtr addFile(
      MediumPtr medium,
      const string &filename,
      const string &path,
      const uint16_t crc,
      const uint32_t fileLength = 0,
      const string &partNumber = string());

    FilePtr addFile(
      const unsigned int mediumIndex,
      const string &filename,
      const string &path,
      const uint16_t crc,
      const uint32_t fileLength = 0,
      const string &partNumber = string());

    ConstLoadMap getLoads( void) const;

    LoadPtr addLoad(
      MediumPtr medium,
      const string &filename,
      const string &path,
      const string &partNumber);

    LoadPtr addLoad(
      const unsigned int mediumIndex,
      const string &filename,
      const string &path,
      const string &partNumber);

    ConstBatchMap getBatches( void) const;

    BatchPtr addBatch(
      MediumPtr medium,
      const string &filename,
      const string &path,
      const string &partNumber);

    BatchPtr addBatch(
      const unsigned int mediumIndex,
      const string &filename,
      const string &path,
      const string &partNumber);

  private:
    MediaMap media;
};

}
}

#endif
