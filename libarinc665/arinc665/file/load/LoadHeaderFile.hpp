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

#include <string>
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
    using string = std::string;
    using LoadFileInfoList = std::list< LoadFileInfo>;
    using TargetHardwareIdList = std::list< string>;
    using UserDefinedData = std::vector< uint8_t>;

    LoadHeaderFile( void);

    LoadHeaderFile( const RawFile &file);

    virtual Arinc665Version getArincVersion( void) const override;

    string getPartNumber( void) const;

    void setPartNumber( const string &partNumber);

    const TargetHardwareIdList& getTargetHardwareIdList( void) const;

    TargetHardwareIdList& getTargetHardwareIdList( void);

    const LoadFileInfoList& getDataFileList( void) const;

    LoadFileInfoList& getDataFileList( void);

    const LoadFileInfoList& getSupportFileList( void) const;

    LoadFileInfoList& getSupportFileList( void);

    const UserDefinedData& getUserDefinedData( void) const;

    void setUserDefinedData( const UserDefinedData &userDefinedData);

    uint32_t getLoadCrc( void) const;

    void setLoadCrc( const uint32_t loadCrc);

  private:
    //! Part number of the load
    string partNumber;
    //! List of compatible target hardware IDs
    TargetHardwareIdList targetHardwareIdList;
    //! List of data files
    LoadFileInfoList dataFileList;
    //! List of Support files
    LoadFileInfoList supportFileList;
    //! User defined data
    UserDefinedData userDefinedData;
    //! CRC of the complete load
    uint32_t loadCrc;
};

}
}

#endif
