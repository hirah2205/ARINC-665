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
 * @brief Declaration of class Arinc665::Media::Batch
 **/

#ifndef ARINC665_MEDIA_BATCH_HPP
#define ARINC665_MEDIA_BATCH_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/BaseFile.hpp>

#include <map>

namespace Arinc665 {
namespace Media {

/**
 * @brief &Batch of loads.
 **/
class Batch: public BaseFile
{
  public:
    using BatchInfo = std::map< string, WeakLoads>;

    Batch( ContainerEntityPtr parent, const string &name);

    /**
     * @copydoc BaseFile::getFileType()
     *
     * @return FileType::BatchFile always.
     **/
    virtual FileType getFileType() const override final;

    /**
     * @brief Get the comment, which describes the batch.
     *
     * @return The comment, which describes the batch.
     **/
    string getComment() const;

    /**
     * @brief Set the comment, which describes the batch.
     *
     * @param[in] comment
     *   The comment, which describes the batch.
     **/
    void setComment( const string &comment);

    /**
     * @brief Return the batch info for the given target hardware ID
     *
     * @param[in] targetHardwareId
     *   Target hardware ID
     *
     * @return The corresponding loads
     **/
    const WeakLoads getTarget( const string &targetHardwareId) const;

    /**
     * @brief Return the batch info for the given target hardware ID
     *
     * @param[in] targetHardwareId
     *   Target hardware ID
     *
     * @return The corresponding loads
     **/
    WeakLoads getTarget( const string &targetHardwareId);

    /**
     * @brief Add  batch info for the given target hardware ID.
     *
     * @param[in] targetHardwareId
     * @param[in] loads
     */
    void addTarget( const string &targetHardwareId, WeakLoads loads);

    /**
     * @brief Return batches.
     *
     * @return The batch target information.
     **/
    const BatchInfo& getTargets() const;

    //! @copydoc getTargets() const
    BatchInfo& getTargets();

  private:
    //! Batch comment
    string comment;
    //! Batch informations
    BatchInfo batches;
};

}
}

#endif
