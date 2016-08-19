/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief  Declaration of class Load.
 **/

#ifndef ARINC665_MEDIA_LOAD_HPP
#define ARINC665_MEDIA_LOAD_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/PartNumberdEntity.hpp>
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
class Load: public PartNumberdEntity
{
  public:
    typedef std::list< WeakFilePtr> FileList;
    typedef std::list< string> ThwIdList;

    /**
     * @brief
     *
     * @param[in] partNumber
     *   Part number of Load
     **/
    Load( const string &partNumber);

    const ThwIdList& getTargetHardwareIdList( void) const;

    ThwIdList& getTargetHardwareIdList( void);

    void setTargetHardwareIdList( const ThwIdList& thwIdList);

    const FileList& getDataFiles( void) const;

    void addDataFile( const WeakFilePtr dataFile);

    const FileList& getSupportFiles( void) const;

    void addSupportFile( const WeakFilePtr supportFile);

    const std::vector< uint8_t>& getUserDefinedData( void) const;

    void setUserDefinedData( const std::vector< uint8_t> &userDefinedData);

  private:
    ThwIdList targetHardwareIdList;
    FileList dataFileList;
    FileList supportFileList;
    std::vector< uint8_t> userDefinedData;
};
}
}

#endif
