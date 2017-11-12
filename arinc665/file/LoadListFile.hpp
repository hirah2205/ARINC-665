/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::LoadListFile.
 **/

#ifndef ARINC665_LOADLISTFILE_HPP
#define ARINC665_LOADLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/ListFile.hpp>
#include <arinc665/file/LoadInfo.hpp>

#include <vector>
#include <map>

namespace Arinc665 {
namespace File {

/**
 * @brief ARINC 665 Load List File..
 **/
class LoadListFile: public ListFile
{
  public:
    //! Loads information map
    using LoadsInfoMap = std::map< std::pair< uint8_t, string>, LoadInfo>;
    //! User defined data type.
    using UserDefinedData = std::vector< uint8_t>;

    /**
     * @brief Creates an empty load list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    LoadListFile( Arinc665Version version);

    /**
     * @brief Creates load list file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param mediaSetPn
     * @param mediaSequenceNumber
     * @param numberOfMediaSetMembers
     * @param loadsInfo
     * @param userDefinedData
     */
    LoadListFile(
      Arinc665Version version,
      const string &mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const LoadsInfo &loadsInfo,
      const UserDefinedData &userDefinedData);

    /**
     * @brief Creates a load list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    LoadListFile( const RawFile &rawFile);

    //! @copydoc ListFile::operator=
    virtual LoadListFile& operator=( const RawFile &rawFile) override;

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
    virtual void setNumberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) override;

    /**
     * @brief Returns the number of loads.
     *
     * @return The number of loads.
     **/
    size_t getNumberOfLoads() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    const LoadsInfo& getLoadsInfo() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    LoadsInfo& getLoadsInfo();

    /**
     * @brief Returns the loads as map for easy access.
     *
     * @return The loads as map.
     */
    LoadsInfoMap getLoadsInfoAsMap() const;

    /**
     * @brief Adds the given load information.
     *
     * @param[in] loadInfo
     *   Load information.
     **/
    void addLoadInfo( const LoadInfo &loadInfo);

    //! @copydoc addLoadInfo(const LoadInfo&)
    void addLoadInfo( LoadInfo &&loadInfo);

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    const UserDefinedData& getUserDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @note The user defined data must be multiple of 2-bytes in size.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void setUserDefinedData( const UserDefinedData &userDefinedData);

    /**
     * @brief Returns if the given load list file belongs to the same media set.
     *
     * Compares:
     * - Part number,
     * - number of media set members,
     * - the load list, and
     * - the user-defined data
     * for match.
     *
     * @param[in] other
     *   The other load list file
     *
     * @return If the given load list files belongs to the same media set
     **/
    bool belongsToSameMediaSet( const LoadListFile &other) const;

  private:
    //! @copydoc ListFile::encode
    virtual RawFile encode() const override final;

    /**
     * @brief Decodes the body of the batch file.
     *
     * @param[in] rawFile
     *   Raw load list file representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the batches information list.
     *
     * @return Raw representation of loads information list.
     **/
    RawFile encodeLoadsInfo() const;

    /**
     * @brief Decodes the loads information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw load list file representation.
     * @param[in] offset
     *   Offset of the loads information list.
     **/
    void decodeLoadsInfo( const RawFile &rawFile, std::size_t offset);

    //! Media set part number
    string mediaSetPn;
    //! media sequence number
    uint8_t mediaSequenceNumber;
    //! number of media set members
    uint8_t numberOfMediaSetMembers;
    //! The load list
    LoadsInfo loadsInfo;
    //! user defined data
    UserDefinedData userDefinedData;
};

}
}

#endif
