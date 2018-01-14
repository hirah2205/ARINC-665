/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Media::MediaSet.
 **/

#ifndef ARINC665_MEDIA_MEDIASET_HPP
#define ARINC665_MEDIA_MEDIASET_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/Base.hpp>
#include <arinc665/media/Medium.hpp>

#include <memory>

namespace Arinc665 {
namespace Media {

/**
 * @brief ARINC 665 media set definition.
 *
 * A media set consists of one or more medias.
 *
 * Each media consists of one or more files.
 *
 **/
class MediaSet:
  public Base,
  public std::enable_shared_from_this< MediaSet>
{
  public:
    /**
     * @brief Creates a new media set.
     *
     * An empty media is automatically created.
     * This media set contains no data files, no loads and no batches.
     *
     * @param[in] name
     *   name of the media set
     **/
    MediaSet( const string &name);

    MediaSet( string &&name);

    //! @copydoc Base::mediaSet
    ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::mediaSet
    MediaSetPtr mediaSet() final;

    //! @copydoc Base::type
    Type type() const final;

    //! @copydoc Base::name
    const string& name() const final;

    /**
     * @brief Sets the name of the media set.
     *
     * @param[in] name
     **/
    void name( const string &name);

    void name( string &&name);

    //! @copydoc Base::partNumber
    string partNumber() const final;

    /**
     * @brief Set the part number of the entity.
     *
     * @param[in] partNumber
     *   The part number
     **/
    void partNumber( const string &partNumber);

    void partNumber( string &&partNumber);

    /**
     * @brief Get the number of medias within the media set
     *
     * @return The number of medias within the media set.
     **/
    uint8_t numberOfMedia() const;

    /**
     * @brief Set the number of media.
     *
     * If there are fewer media than requested, the media are created empty,
     * otherwise the media are deleted.
     *
     * @param[in] numberOfMedia
     *   The new number of media. Must be bigger or equal to 1.
     * @param[in] deleteFiles
     *   Delete files, which are part of the not existing files.
     *   Otherwise, they are moved to the first media.
     **/
    void numberOfMedia( uint8_t numberOfMedia, bool deleteFiles = false);

    /**
     * @brief Returns all media.
     *
     * @return All media of the media set.
     **/
    ConstMedia media() const;

    //! @copydoc MediaSet::getMedia() const
    Media media();

    /**
     * @brief Return the medium with the requested index.
     *
     * @param[in] index
     *   The medium index.
     *
     * @return The medium with the requested index.
     **/
    ConstMediumPtr medium( uint8_t index) const;

    /**
     * @brief Return the medium with the requested index.
     *
     * @param[in] index
     *   The medium index.
     *
     * @return The medium with the requested index.
     **/
    MediumPtr medium( uint8_t index);
    
    /**
     * @brief Adds an medium to the media set and returns the media index.
     *
     * @return The media index of the new medium.
     **/
    MediumPtr addMedium();

    /**
     * @brief Removes the last medium.
     *
     * @param[in] deleteFiles
     *   Delete files, which are part of the not existing medium.
     *   Otherwise, they are moved to the first media.
     **/
    void removeMedium( bool deleteFiles = false);

    /**
     * @brief Return the total number of files within the media set.
     *
     * @return The number of files.
     **/
    size_t numberOfFiles() const;

    /**
     * @brief Returns all files present on the media set.
     *
     * @return All files.
     **/
    ConstFiles files() const;

    /**
     * @brief Returns all files present on the media set.
     *
     * @return All files.
     **/
    Files files();

    /**
     * @brief Returns file with the given filename.
     *
     * @param[in] filename
     *   The filename.
     *
     * @return File with the given filename.
     * @retval ConstFilePtr()
     *   If file is not found.
     **/
    ConstFilePtr file( const string &filename) const;

    /**
     * @brief Returns file with the given filename.
     *
     * @param[in] filename
     *   The filename.
     *
     * @return File with the given filename.
     * @retval ConstFilePtr()
     *   If file is not found.
     **/
    FilePtr file( const string &filename);

    /**
     * @brief Return the number of loads within the media set.
     *
     * @return The number of loads within the media set.
     **/
    size_t numberOfLoads() const;

    /**
     * @brief Returns the loads within the media set.
     *
     * @return The loads within the media set.
     **/
    ConstLoads loads() const;

    /**
     * @brief Returns the loads within the media set.
     *
     * @return The loads within the media set.
     **/
    Loads loads();

    /**
     * @brief return the load with the given filename.
     *
     * @param[in] filename
     *   The filename of the load.
     *
     * @return The load with the given filename.
     **/
    ConstLoadPtr load( const string &filename) const;

    /**
     * @brief return the load with the given filename.
     *
     * @param[in] filename
     *   The filename of the load.
     *
     * @return The load with the given filename.
     **/
    LoadPtr load( const string &filename);

    /**
     * @brief Removes the load with the given filename.
     *
     * @param[in] filename
     *   The filename of the load.
     **/
    void removeLoad( const string &filename);

    /**
     * @brief Removes the given load from the media set.
     *
     * @param[in] load
     *   The load to remove.
     **/
    void removeLoad( ConstLoadPtr load);

    /**
     * @brief Return the number of batches within the media set.
     *
     * @return The number of batches within the media set.
     **/
    size_t numberOfBatches() const;

    /**
     * @brief Returns the batches within the media set.
     *
     * @return The batches within the media set.
     **/
    ConstBatches batches() const;

    /**
     * @brief Returns the batches within the media set.
     *
     * @return The batches within the media set.
     **/
    Batches batches();

    /**
     * @brief return the batch with the given filename.
     *
     * @param[in] filename
     *   The filename of the batch.
     *
     * @return The batch with the given filename.
     **/
    ConstBatchPtr batch( const string &filename) const;

    /**
     * @brief return the batch with the given filename.
     *
     * @param[in] filename
     *   The filename of the batch.
     *
     * @return The batch with the given filename.
     **/
    BatchPtr batch( const string &filename);

    /**
     * @brief Removes the batch with the given filename.
     *
     * @param[in] filename
     *   The filename of the batch.
     **/
    void removeBatch( const string &filename);

    /**
     * @brief Removes the given batch from the media set.
     *
     * @param[in] batch
     *   The batch to remove.
     **/
    void removeBatch( ConstBatchPtr batch);

  private:
    //! the media
    Media mediaValue;
    //! name
    string nameValue;
    //! The part number
    string partNumberValue;
};

}
}

#endif
