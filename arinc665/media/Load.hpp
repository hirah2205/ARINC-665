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
#include <arinc665/media/RegularFile.hpp>

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <optional>
#include <cstdint>

namespace Arinc665::Media {

/**
 * @brief %Load within %Media Set.
 **/
class Load : public BaseFile
{
  public:
    //! Positions List
    using Positions = std::set< std::string, std::less<> >;
    //! Target Hardware ID / Positions
    using TargetHardwareIdPositions =
      std::map< std::string, Positions, std::less<> >;
    //! Target Hardware ID List
    using TargetHardwareIds = std::set< std::string, std::less<> >;
    //! User Defined Data Type
    using UserDefinedData = std::vector< uint8_t >;
    //! Load Type (Description + ID)
    using Type = std::optional< std::pair< std::string, uint16_t > >;

    /**
     * @brief Initialises a ARINC 665 Load
     *
     * @param[in] parent
     *   Parent of this Load.
     * @param[in] name
     *   Name of the Load
     **/
    Load( ContainerEntityPtr parent, std::string_view name );

    //! @copydoc Load()
    Load( ContainerEntityPtr parent, std::string &&name );

    /**
     * @copydoc BaseFile::fileType()
     *
     * @return FileType::LoadFile always.
     **/
    [[nodiscard]] FileType fileType() const final;

    /**
     * @name Part Flags.
     * @{
     **/
    /**
     * @brief Obtains the Part Flags Field.
     *
     * @return The part Flags Field.
     **/
    uint16_t partFlags() const;

    /**
     * @brief Updates the Part Flags Field
     *
     * @param[in] partFlags
     *   Part Flags
     **/
    void partFlags( uint16_t partFlags );

    /** @} **/

    /**
     * @name Load Part Number
     * @{
     **/

    /**
     * @brief Return the Part Number of the Media Set.
     *
     * @return Part Number of the Media Set.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the Part Number
     *
     * @param[in] partNumber
     *   New Part number
     **/
    void partNumber( std::string_view partNumber );

    //! @copydoc partNumber( std::string_view)
    void partNumber( std::string &&partNumber );

    /** @} **/

    /**
     * @name Target Hardware IDs with Positions.
     * @{
     **/

    /**
     * @brief Returns the List of Target HW IDs.
     *
     * @return List of Target HW IDs.
     **/
    [[nodiscard]] const TargetHardwareIdPositions&
    targetHardwareIdPositions() const;

    //! @copydoc targetHardwareIdPositions() const
    TargetHardwareIdPositions& targetHardwareIdPositions();

    /**
     * @brief Updates the Target Hardware ID/ Position.
     *
     * @param[in] targetHardwareIdPositions
     *   Target Hardware ID/ Positions.
     **/
    void targetHardwareIdPositions(
      const TargetHardwareIdPositions &targetHardwareIdPositions );

    //! @copydoc targetHardwareIdPositions(const TargetHardwareIdPositions&)
    void targetHardwareIdPositions(
      TargetHardwareIdPositions &&targetHardwareIdPositions );

    /** @} **/

    /**
     * @name Target Hardware IDs without Positions Information.
     * @{
     **/

    /**
     * @brief Returns the List of Target HW IDs without position information.
     *
     * @return Target HW IDs.
     **/
    [[nodiscard]] TargetHardwareIds targetHardwareIds() const;

    /**
     * @brief Adds a list of Target HW IDs without position information.
     *
     * @param[in] thwIds
     *   Target HW IDs.
     **/
    void targetHardwareIds( const TargetHardwareIds &thwIds );

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
      const Positions &positions = {} );

    //! @copydoc targetHardwareId(std::string_view,const Positions&)
    void targetHardwareId(
      std::string &&targetHardwareId,
      Positions &&positions = {} );

    /** @} **/

    /**
     * @name Data Files
     * A (data) file is a pair of file and part number.
     *
     * @{
     **/

    /**
     * @brief Returns the data files.
     *
     * @return Data files.
     **/
    [[nodiscard]] LoadFiles dataFiles() const;

    /**
     * @brief Add the given file as data file.
     *
     * @param[in] file
     *   Data File.
     * @param[in] partNumber
     *   Part Number.
     **/
    void dataFile( const FilePtr &file, std::string_view partNumber );

    //! @copydoc dataFile(const FilePtr&,std::string_view)
    void dataFile( const FilePtr &file, std::string &&partNumber );

    /** @} **/

    /**
     * @name Support Files
     * A (Support) file is a pair of file and part number.
     *
     * @{
     **/

    /**
     * @brief Returns the support files.
     *
     * @return Support Files.
     **/
    [[nodiscard]] LoadFiles supportFiles() const;

    /**
     * @brief Add the given file as support file.
     *
     * @param[in] file
     *   Support File.
     * @param[in] partNumber
     *   Part Number.
     **/
    void supportFile( const FilePtr &file, std::string_view partNumber );

    //! @copydoc supportFile(const FilePtr&,std::string_view)
    void supportFile( const FilePtr &file, std::string &&partNumber );

    /** @} **/

    /**
     * @name User Defined Data
     * @{
     **/

    /**
     * @brief Returns the user-defined data stored in the load header.
     *
     * @return User-defined data.
     **/
    [[nodiscard]] const UserDefinedData& userDefinedData() const;

    //! @copydoc Load::userDefinedData() const
    [[nodiscard]] UserDefinedData& userDefinedData();

    /**
     * @brief Sets the user-defined data stored in the load header.
     *
     * @param[in] userDefinedData
     *   Updated user-defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData );

    //! @copydoc Load::userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData );

    /** @} **/

    /**
     * @name Load Type
     * @{
     **/

    /**
     * @brief Returns the Load Type.
     *
     * @return Load Type.
     **/
    [[nodiscard]] const Type& loadType() const;

    /**
     * @brief Updates the Load Type.
     *
     * @param[in] type
     *   New Load Type.
     **/
    void loadType( const Type &type );

    //! @copydoc loadType(const Type&)
    void loadType( Type &&type );

    /** @} **/

  private:
    //! Part Flags
    uint16_t partFlagsV;
    //! Part Number
    std::string partNumberV;
    //! Target Hardware ID/ Positions
    TargetHardwareIdPositions targetHardwareIdPositionsV;
    //! Data Files
    WeakLoadFiles dataFilesV;
    //! Support Files
    WeakLoadFiles supportFilesV;
    //! User Defined Data
    UserDefinedData userDefinedDataV;
    //! Load Type
    Type typeV;
};

}

#endif
