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

    //! @copydoc BatchListFile(Arinc665Version,const string&,uint8_t,uint8_t,const BatchesInfo&,const UserDefinedData&)
    BatchListFile(
      Arinc665Version version,
      string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      BatchesInfo &&batchesInfo,
      UserDefinedData &&userDefinedData);

    /**
     * @brief Creates a batch list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    BatchListFile( const RawFile &rawFile);

    //! @copydoc ListFile::operator=
    BatchListFile& operator=( const RawFile &rawFile) final;

    //! @copydoc ListFile::mediaSetPn
    string mediaSetPn() const final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( const string &mediaSetPn) final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( string &&mediaSetPn) final;

    //! @copydoc ListFile::mediaSequenceNumber
    uint8_t mediaSequenceNumber() const final;

    //! @copydoc ListFile::mediaSequenceNumber
    void mediaSequenceNumber( uint8_t mediaSequenceNumber) final;

    //! @copydoc ListFile::numberOfMediaSetMembers
    uint8_t numberOfMediaSetMembers() const final;

    //! @copydoc ListFile::numberOfMediaSetMembers
    void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) final;

    /**
     * @brief Return the number of batches.
     *
     * @return The number of batches.
     **/
    size_t numberOfBatches() const;

    /**
     * @brief Returns the batches information.
     *
     * @return The batches information.
     **/
    const BatchesInfo& batchesInfo() const;

    //! @copydoc batchesInfo() const
    BatchesInfo& batchesInfo();

    /**
     * @brief Returns the batches information as map.
     *
     * @return The batches information as map.
     */
    BatchInfoMap batchesInfoAsMap() const;

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
    const UserDefinedData& userDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

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
    string mediaSetPnValue;
    //! The media sequence number
    uint8_t mediaSequenceNumberValue;
    //! The number of media set members
    uint8_t numberOfMediaSetMembersValue;
    //! The stored batches information
    BatchesInfo batchesInfoValue;
    //! user defined data
    UserDefinedData userDefinedDataValue;
};

}
}

#endif
