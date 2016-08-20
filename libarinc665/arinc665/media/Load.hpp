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
 * @brief  Declaration of class Arinc665::Media::Load.
 **/

#ifndef ARINC665_MEDIA_LOAD_HPP
#define ARINC665_MEDIA_LOAD_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/BaseFile.hpp>
#include <arinc665/media/File.hpp>

#include <string>
#include <list>
#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace Media {
using std::string;

/**
 * @brief
 **/
class Load: public BaseFile
{
  public:
    using FileList = std::list< WeakFilePtr>;
    using ThwIdList = std::list< string>;
    using UserDefinedData = std::vector< uint8_t>;

    /**
     * @brief
     *
     * @param[in] partNumber
     *   Part number of Load
     **/
    Load( const string &name, const string &partNumber);

    virtual FileType getFileType( void) const override;

    const ThwIdList& getTargetHardwareIdList( void) const;

    ThwIdList& getTargetHardwareIdList( void);

    void setTargetHardwareIdList( const ThwIdList& thwIdList);

    const FileList& getDataFiles( void) const;

    void addDataFile( const WeakFilePtr dataFile);

    const FileList& getSupportFiles( void) const;

    void addSupportFile( const WeakFilePtr supportFile);

    const UserDefinedData& getUserDefinedData( void) const;

    UserDefinedData& getUserDefinedData( void);

    void setUserDefinedData( const UserDefinedData &userDefinedData);

  private:
    ThwIdList targetHardwareIdList;
    FileList dataFileList;
    FileList supportFileList;
    UserDefinedData userDefinedData;
};
}
}

#endif
