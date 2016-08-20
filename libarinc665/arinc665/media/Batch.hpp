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
#include <arinc665/media/BatchInfo.hpp>

#include <list>

namespace Arinc665 {
namespace Media {

/**
 * @brief
 **/
class Batch: public BaseFile
{
  public:
    using BatchInfoList = std::list< BatchInfo>;

    Batch(
      const string &name,
      const string &partNumber,
      const string &comment = string());

    virtual FileType getFileType( void) const override;

    /**
     * @brief Get the comment, which describes the batch.
     *
     * @return The comment, which describes the batch.
     **/
    string getComment( void) const;

    /**
     * @brief Set the comment, which describes the batch.
     *
     * @param[in] comment
     *   The comment, which describes the batch.
     **/
    void setComment( const string &comment);

    const BatchInfoList& getBatchInfos( void);

    BatchInfo& addBatchInfo( const string &targetHardwareId);

  private:
    string comment;
    BatchInfoList batchInfos;
};

}
}

#endif
