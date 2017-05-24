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
 * @brief Load within media set.
 **/
class Load: public BaseFile
{
  public:
    //! File list
    using FileList = std::list< WeakFilePtr>;
    //! Target hardware ID list
    using ThwIdList = std::list< string>;
    //! User defined data type
    using UserDefinedData = std::vector< uint8_t>;

    /**
     * @brief
     *
     * @param[in] parent
     *   The parent of this load.
     * @param[in] name
     *   Name of the Load
     **/
    Load( ContainerEntityPtr parent, const string &name);

    /**
     * @copydoc BaseFile::getFileType()
     *
     * @return FileType::LoadFile always.
     **/
    virtual FileType getFileType() const override final;

    /**
     * @brief Returns the List of Target HW IDs.
     *
     * @return The list of Target HW IDs.
     **/
    const ThwIdList& getTargetHardwareIdList() const;

    /**
     * @brief Returns the List of Target HW IDs.
     *
     * @return The list of Target HW IDs (modifiable).
     **/
    ThwIdList& getTargetHardwareIdList();

    /**
     * @brief Updates the list of Target HW IDs.
     *
     * @param[in] thwIdList
     *   The list of Target HW IDs.
     **/
    void setTargetHardwareIdList( const ThwIdList& thwIdList);

    /**
     * @brief Returns the data files.
     *
     * @return The data files.
     **/
    const FileList& getDataFiles() const;

    /**
     * @brief Add the given file as data file.
     *
     * @param[in] dataFile
     *   The data file.
     **/
    void addDataFile( const WeakFilePtr dataFile);

    /**
     * @brief Returns the support files.
     *
     * @return The support files.
     **/
    const FileList& getSupportFiles() const;

    /**
     * @brief Add the given file as support file.
     *
     * @param[in] supportFile
     *   The support file.
     **/
    void addSupportFile( const WeakFilePtr supportFile);

    /**
     * @brief Returns the user-defined data stored in the load header.
     *
     * @return The user-defined data.
     **/
    const UserDefinedData& getUserDefinedData() const;

    //! @copydoc Load::getUserDefinedData() const
    UserDefinedData& getUserDefinedData();

    /**
     * @brief Sets the user-defined data stored in the load header.
     *
     * @param[in] userDefinedData
     *   The updated user-defined data.
     **/
    void setUserDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc Load::setUserDefinedData(const UserDefinedData&)
    void setUserDefinedData( UserDefinedData &&userDefinedData);

  private:
    ThwIdList targetHardwareIdList;
    FileList dataFileList;
    FileList supportFileList;
    UserDefinedData userDefinedData;
};

}
}

#endif
