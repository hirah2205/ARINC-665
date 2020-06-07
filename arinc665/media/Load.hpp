/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::Load.
 **/

#ifndef ARINC665_MEDIA_LOAD_HPP
#define ARINC665_MEDIA_LOAD_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/BaseFile.hpp>
#include <arinc665/media/File.hpp>

#include <string>
#include <list>
#include <map>
#include <vector>
#include <optional>
#include <cstdint>

namespace Arinc665::Media {

/**
 * @brief %Load within %Media Set.
 **/
class Load: public BaseFile
{
  public:
    //! File List
    using Files = std::list< WeakFilePtr>;
    //! Target Hardware ID / Positions
    using TargetHardwareIdPositions =
      std::map< std::string, std::list< std::string>>;
    //! Target Hardware ID List
    using TargetHardwareIds = std::list< std::string>;
    //! Positions List
    using Positions = std::list< std::string>;
    //! User Defined Data Type
    using UserDefinedData = std::vector< uint8_t>;
    //! Load Type (Description + ID)
    using Type = std::optional< std::pair< std::string, uint16_t>>;

    /**
     * @brief Initialises a ARINC 665 Load
     *
     * @param[in] parent
     *   The parent of this load.
     * @param[in] name
     *   Name of the Load
     **/
    Load( ContainerEntityPtr parent, std::string_view name);

    //! @copydoc Load()
    Load( ContainerEntityPtr parent, std::string &&name);

    /**
     * @copydoc BaseFile::fileType()
     *
     * @return FileType::LoadFile always.
     **/
    [[nodiscard]] FileType fileType() const final;

    /**
     * @brief Returns the List of Target HW IDs.
     *
     * @return The list of Target HW IDs.
     **/
    [[nodiscard]] const TargetHardwareIdPositions& targetHardwareIdPositions() const;

    //! @copydoc targetHardwareIdPositions() const
    TargetHardwareIdPositions& targetHardwareIdPositions();

    /**
     * @brief Updates the Target Hardware ID/ Position.
     *
     * @param[in] targetHardwareIdPositions
     *   Target Hardware ID/ Positions.
     **/
    void targetHardwareIdPositions(
      const TargetHardwareIdPositions &targetHardwareIdPositions);

    //! @copydoc targetHardwareIdPositions(const TargetHardwareIdPositions&)
    void targetHardwareIdPositions(
      TargetHardwareIdPositions &&targetHardwareIdPositions);

    /**
     * @brief Returns the List of Target HW IDs without position information.
     *
     * @return The list of Target HW IDs.
     **/
    [[nodiscard]] TargetHardwareIds targetHardwareIds() const;

    /**
     * @brief Adds a list of Target HW IDs without position information.
     *
     * @param[in] thwIds
     *   The list of Target HW IDs.
     **/
    void targetHardwareIds( const TargetHardwareIds &thwIds);

    /**
     * @brief Adds a Target Hardware ID to the List of THW IDs.
     *
     * @param[in] targetHardwareId
     *   New Target Hardware ID.
     * @param[in] positions
     *   Optional additional Position specification.
     **/
    void targetHardwareId(
      std::string_view targetHardwareId,
      const Positions &positions = {});

    //! @copydoc targetHardwareId(std::string_view,const Positions&)
    void targetHardwareId(
      std::string &&targetHardwareId,
      Positions &&positions = {});

    /**
     * @brief Returns the data files.
     *
     * @return The data files.
     **/
    [[nodiscard]] const Files& dataFiles() const;

    /**
     * @brief Add the given file as data file.
     *
     * @param[in] dataFile
     *   The data file.
     **/
    void dataFile( const WeakFilePtr& dataFile);

    /**
     * @brief Returns the support files.
     *
     * @return The support files.
     **/
    [[nodiscard]] const Files& supportFiles() const;

    /**
     * @brief Add the given file as support file.
     *
     * @param[in] supportFile
     *   The support file.
     **/
    void supportFile( const WeakFilePtr& supportFile);

    /**
     * @brief Returns the user-defined data stored in the load header.
     *
     * @return The user-defined data.
     **/
    [[nodiscard]] const UserDefinedData& userDefinedData() const;

    //! @copydoc Load::userDefinedData() const
    [[nodiscard]] UserDefinedData& userDefinedData();

    /**
     * @brief Sets the user-defined data stored in the load header.
     *
     * @param[in] userDefinedData
     *   The updated user-defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc Load::userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

    /**
     * @brief Returns the Load Type.
     *
     * @return The Load Type.
     **/
    [[nodiscard]] const Type& loadType() const;

    /**
     * @brief Updates the Load Type.
     *
     * @param[in] type
     *   New Load Type.
     **/
    void loadType( const Type &type);

    //! @copydoc loadType(const Type&)
    void loadType( Type &&type);

  private:
    //! Target Hardware ID/ Positions
    TargetHardwareIdPositions targetHardwareIdPositionsValue;
    //! Data Files
    Files dataFilesValue;
    //! Support Files
    Files supportFilesValue;
    //! User Defined Data
    UserDefinedData userDefinedDataValue;
    //! Load Type
    Type typeValue;
};

}

#endif
