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
#include <arinc665/file/load/LoadFileInfo.hpp>

#include <list>
#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief Representation of the content of an Load Upload Header file (.LUH)
 **/
class LoadHeaderFile: public Arinc665File
{
  public:
    using TargetHardwareIdList = std::list< string>;
    using UserDefinedData = std::vector< uint8_t>;

    LoadHeaderFile( Arinc665Version version);

    LoadHeaderFile( const RawFile &file);

    //! @copydoc Arinc665File::operator=
    virtual LoadHeaderFile& operator=( const RawFile &rawFile) override;

    string getPartNumber() const;

    void setPartNumber( const string &partNumber);

    const TargetHardwareIdList& getTargetHardwareIdList() const;

    TargetHardwareIdList& getTargetHardwareIdList();

    const LoadFilesInfo& getDataFiles() const;

    LoadFilesInfo& getDataFiles();

    const LoadFilesInfo& getSupportFiles() const;

    LoadFilesInfo& getSupportFiles();

    const UserDefinedData& getUserDefinedData() const;

    void setUserDefinedData( const UserDefinedData &userDefinedData);

    uint32_t getLoadCrc() const;

    void setLoadCrc( uint32_t loadCrc);

  private:
    //! @copydoc Arinc665File::encode
    virtual RawFile encode() const override final;

    void decodeBody( const RawFile &rawFile);

    LoadFilesInfo decodeFileList( const RawFile &rawFile, std::size_t offset);

    //! Part number of the load
    string partNumber;
    //! List of compatible target hardware IDs
    TargetHardwareIdList targetHardwareIdList;
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
