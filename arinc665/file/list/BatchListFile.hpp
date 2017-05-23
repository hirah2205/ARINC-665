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
 * @brief Declaration of class Arinc665::File::BatchListFile.
 **/

#ifndef ARINC665_BATCHLISTFILE_HPP
#define ARINC665_BATCHLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/BatchInfo.hpp>

#include <list>
#include <vector>
#include <map>

namespace Arinc665 {
namespace File {

/**
 * @brief The load list represents each BATCHES.LUM file on each media of
 *   the media set.
 **/
class BatchListFile: public ListFile
{
  public:
    using BatchInfoMap = std::map< std::pair< uint8_t, string>, BatchInfo>;
    using UserDefinedData = std::vector< uint8_t>;

    BatchListFile( Arinc665Version version);

    BatchListFile( const RawFile &rawFile);

    //! @copydoc ListFile::operator=
    virtual BatchListFile& operator=( const RawFile &rawFile) override;

    //! @copydoc ListFile::getMediaSetPn
    virtual string getMediaSetPn() const override;

    //! @copydoc ListFile::setMediaSetPn
    virtual void setMediaSetPn( const string &mediaSetPn) override;

    //! @copydoc ListFile::getMediaSequenceNumber
    virtual uint8_t getMediaSequenceNumber() const override;

    //! @copydoc ListFile::setMediaSequenceNumber
    virtual void setMediaSequenceNumber( uint8_t mediaSequenceNumber) override;

    //! @copydoc ListFile::getNumberOfMediaSetMembers
    virtual uint8_t getNumberOfMediaSetMembers() const override;

    //! @copydoc ListFile::setNumberOfMediaSetMembers
    virtual void setNumberOfMediaSetMembers(
      uint8_t numberOfMediaSetMembers) override;

    unsigned int getNumberOfBatches() const;

    const BatchesInfo& getBatchesInfo() const;

    BatchesInfo& getBatchesInfo();

    BatchInfoMap getBatchesInfoAsMap() const;

    void addBatchInfo( const BatchInfo &batchInfo);

    void addBatchInfo( BatchInfo &&batchInfo);

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    const UserDefinedData& getUserDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void setUserDefinedData( const UserDefinedData &userDefinedData);

    bool belongsToSameMediaSet( const BatchListFile &other) const;

  private:
    //! @copydoc ListFile::encode
    virtual RawFile encode() const override final;

    void decodeBody( const RawFile &rawFile);

    RawFile encodeBatchesInfo() const;

    void decodeBatchesInfo( const RawFile &rawFile, std::size_t offset);

    //! The media set part number
    string mediaSetPn;
    //! The media sequence number
    uint8_t mediaSequenceNumber;
    //! The number of media set members
    uint8_t numberOfMediaSetMembers;
    //! The stored batches information
    BatchesInfo batchesInfo;
    //! user defined data
    UserDefinedData userDefinedData;
};

}
}

#endif
