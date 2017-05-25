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
 * @brief Declaration of class Arinc665::File::LoadHeaderFile.
 **/

#ifndef ARINC665_FILE_LOAD_LOADHEADERFILE_HPP
#define ARINC665_FILE_LOAD_LOADHEADERFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>
#include <arinc665/file/LoadFileInfo.hpp>

#include <list>
#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief ARINC 665 Load Header File.
 **/
class LoadHeaderFile: public Arinc665File
{
  public:
    //! Target Hardware ID list type
    using TargetHardwareIds = StringList;
    //! User defined data type
    using UserDefinedData = std::vector< uint8_t>;

    /**
     * @brief Creates an empty load header file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    LoadHeaderFile( Arinc665Version version);

    /**
     * @brief Creates load header file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] partNumber
     *   Load part number
     * @param[in] targetHardwareIds
     *   Target Hardware IDs
     * @param[in] dataFilesInfo
     *   Data files
     * @param[in] supportFilesInfo
     *   Support files
     * @param[in] userDefinedData
     *   User defined data
     * @param[in] loadCrc
     *   Load CRC
     **/
    LoadHeaderFile(
      Arinc665Version version,
      const string &partNumber,
      const TargetHardwareIds &targetHardwareIds,
      const LoadFilesInfo &dataFilesInfo,
      const LoadFilesInfo &supportFilesInfo,
      const UserDefinedData &userDefinedData,
      uint32_t loadCrc);

    /**
     * @brief Creates a load header file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    LoadHeaderFile( const RawFile &rawFile);

    //! @copydoc Arinc665File::operator=
    virtual LoadHeaderFile& operator=( const RawFile &rawFile) override;

    /**
     * @brief Returns the part number of the load header file.
     *
     * @return The part number of the load header file.
     **/
    string getPartNumber() const;

    /**
     * @brief Updates the part number of the load header file.
     *
     * @param[in] partNumber
     *   The new part number.
     **/
    void setPartNumber( const string &partNumber);

    /**
     * @brief Returns the target hardware IDs.
     *
     * @return The target hardware IDs
     **/
    const TargetHardwareIds& getTargetHardwareIds() const;

    //! @copydoc getTargetHardwareIdList() const
    TargetHardwareIds& getTargetHardwareIds();

    /**
     * @brief Sets the target hardware IDs.
     *
     * @param[in] targetHardwareIds
     **/
    void setTargetHardwareIds( const TargetHardwareIds &targetHardwareIds);

    //! @copydoc setTargetHardwareIdList( const TargetHardwareIdList&)
    void setTargetHardwareIds( TargetHardwareIds &&targetHardwareIds);

    /**
     * @brief Add target hardware ID.
     *
     * @param[in] targetHardwareId
     *   Target hardware ID.
     **/
    void addTargetHardwareId( const string &targetHardwareId);

    //! @copydoc addTargetHardwareId( const string&)
    void addTargetHardwareId( string targetHardwareId);

    /**
     * @brief Return the data files information.
     *
     * @return The data files information.
     **/
    const LoadFilesInfo& getDataFiles() const;

    //! @copydoc getDataFiles() const
    LoadFilesInfo& getDataFiles();

    /**
     * @brief Add data file.
     *
     * @param[in] dataFileInfo
     *   The data file information.
     **/
    void addDataFile( const LoadFileInfo &dataFileInfo);

    //! @copydoc addDataFile(const LoadFileInfo&)
    void addDataFile( LoadFileInfo &&dataFileInfo);

    /**
     * @brief Return the support files information.
     *
     * @return The support files information.
     **/
    const LoadFilesInfo& getSupportFiles() const;

    //! @copydoc getDataFiles() const
    LoadFilesInfo& getSupportFiles();

    /**
     * @brief Add support file.
     *
     * @param[in] supportFileInfo
     *   The support file information.
     **/
    void addSupportFile( const LoadFileInfo &supportFileInfo);

    //! @copydoc addSupportFile(const LoadFileInfo&)
    void addSupportFile( LoadFileInfo &&supportFileInfo);

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
     * @brief Returns the load CRC.
     *
     * @return The load CRC
     **/
    uint32_t getLoadCrc() const;

    /**
     * @brief Updates the load CRC.
     *
     * @param[in] loadCrc
     *   The load CRC
     **/
    void setLoadCrc( uint32_t loadCrc);

  private:
    //! @copydoc Arinc665File::encode
    virtual RawFile encode() const override final;

    /**
     * @brief Decodes the body of the load header file.
     *
     * @param[in] rawFile
     *   Raw load header file representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the files information list.
     *
     * @todo split between data and support files - the format differs in later
     * ARINC 665 supplements.
     *
     * @param[in] loadFilesInfo
     *   The load files information to encode.
     *
     * @return Raw representation of files information list.
     **/
    RawFile encodeFileList( const LoadFilesInfo &loadFilesInfo) const;

    /**
     * @brief Decodes the files information list from the raw data.
     *
     * @todo split between data and support files - the format differs in later
     * ARINC 665 supplements.
     *
     * @param[in] rawFile
     *   Raw load header file representation.
     * @param[in] offset
     *   Offset of the files information list.
     *
     * @return The decoded load files information.
     **/
    LoadFilesInfo decodeFileList( const RawFile &rawFile, std::size_t offset);

    //! Part number of the load
    string partNumber;
    //! List of compatible target hardware IDs
    TargetHardwareIds targetHardwareIds;
    //! List of data files
    LoadFilesInfo dataFilesInfo;
    //! List of Support files
    LoadFilesInfo supportFilesInfo;
    //! User defined data
    UserDefinedData userDefinedData;
    //! CRC of the complete load
    uint32_t loadCrc;
};

}
}

#endif
