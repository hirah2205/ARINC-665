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

/**
 * @brief Load within media set.
 **/
class Load: public BaseFile
{
  public:
    //! File list
    using Files = std::list< WeakFilePtr>;
    //! Target hardware ID list
    using TargetHardwareIds = std::list< string>;
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

    //! @copydoc Load()
    Load( ContainerEntityPtr parent, string &&name);

    /**
     * @copydoc BaseFile::fileType()
     *
     * @return FileType::LoadFile always.
     **/
    FileType fileType() const final;

    /**
     * @brief Returns the List of Target HW IDs.
     *
     * @return The list of Target HW IDs.
     **/
    const TargetHardwareIds& targetHardwareIds() const;

    /**
     * @brief Returns the List of Target HW IDs.
     *
     * @return The list of Target HW IDs (modifiable).
     **/
    TargetHardwareIds& targetHardwareIds();

    /**
     * @brief Updates the list of Target HW IDs.
     *
     * @param[in] thwIds
     *   The list of Target HW IDs.
     **/
    void targetHardwareIds( const TargetHardwareIds &thwIds);

    //! @copydoc targetHardwareIds(const TargetHardwareIds&)
    void targetHardwareIds( TargetHardwareIds &&thwIds);

    /**
     * @brief Returns the data files.
     *
     * @return The data files.
     **/
    const Files& dataFiles() const;

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
    const Files& supportFiles() const;

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
    const UserDefinedData& userDefinedData() const;

    //! @copydoc Load::userDefinedData() const
    UserDefinedData& userDefinedData();

    /**
     * @brief Sets the user-defined data stored in the load header.
     *
     * @param[in] userDefinedData
     *   The updated user-defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc Load::userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

  private:
    //! target hardware IDs
    TargetHardwareIds targetHardwareIdsValue;
    //! Data files
    Files dataFilesValue;
    //! Support files
    Files supportFilesValue;
    //! user defined data
    UserDefinedData userDefinedDataValue;
};

}
}

#endif
