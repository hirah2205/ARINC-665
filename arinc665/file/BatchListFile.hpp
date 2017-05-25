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
#include <arinc665/file/ListFile.hpp>
#include <arinc665/file/BatchInfo.hpp>

#include <list>
#include <vector>
#include <map>

namespace Arinc665 {
namespace File {

/**
 * @brief ARINC 665 Batch List File.
 **/
class BatchListFile: public ListFile
{
  public:
    //! Batch information map type
    using BatchInfoMap = std::map< std::pair< uint8_t, string>, BatchInfo>;
    //! User defined data type.
    using UserDefinedData = std::vector< uint8_t>;

    /**
     * @brief Creates an empty batch list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    BatchListFile( Arinc665Version version);

    /**
     * @brief Creates batch list file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param mediaSetPn
     * @param mediaSequenceNumber
     * @param numberOfMediaSetMembers
     * @param batchesInfo
     * @param userDefinedData
     **/
    BatchListFile(
      Arinc665Version version,
      const string &mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const BatchesInfo &batchesInfo,
      const UserDefinedData &userDefinedData);

    /**
     * @brief Creates a batch list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
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

    /**
     * @brief Return the number of batches.
     *
     * @return The number of batches.
     **/
    size_t getNumberOfBatches() const;

    /**
     * @brief Returns the batches information.
     *
     * @return The batches information.
     **/
    const BatchesInfo& getBatchesInfo() const;

    //! @copydoc getBatchesInfo() const
    BatchesInfo& getBatchesInfo();

    /**
     * @brief Returns the batches information as map.
     *
     * @return The batches information as map.
     */
    BatchInfoMap getBatchesInfoAsMap() const;

    /**
     * @brief Adds the given batch information.
     *
     * @param[in] batchInfo
     *   The batch information to add.
     **/
    void addBatchInfo( const BatchInfo &batchInfo);

    //! @copydoc addBatchInfo(const BatchInfo&)
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

    /**
     * @brief Returns if the given batch list file belongs to the same media set.
     *
     * @param[in] other
     *   The other batch list file to compare to this.
     *
     * @return If the given batch list file belongs to the same media set.
     **/
    bool belongsToSameMediaSet( const BatchListFile &other) const;

  private:
    //! @copydoc ListFile::encode
    virtual RawFile encode() const override final;

    /**
     * @brief Decodes the body of the batch list file.
     *
     * @param[in] rawFile
     *   Raw batch list file representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the batches information list.
     *
     * @return Raw representation of batches information list.
     **/
    RawFile encodeBatchesInfo() const;

    /**
     * @brief Decodes the batches information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw batch list file representation.
     * @param[in] offset
     *   Offset of the batches information list.
     **/
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
