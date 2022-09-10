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
#include <arinc665/media/File.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc645/Arinc645.hpp>

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
class ARINC665_EXPORT Load : public File
{
  public:
    //! Positions List
    using Positions = std::set< std::string, std::less<> >;
    //! Target Hardware ID / Positions
    using TargetHardwareIdPositions =
      std::map< std::string, Positions, std::less<> >;
    //! Target Hardware ID List
    using TargetHardwareIds = std::set< std::string, std::less<> >;
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
    Load( const ContainerEntityPtr& parent, std::string_view name );

    //! @copydoc Load()
    Load( const ContainerEntityPtr& parent, std::string &&name );

    /**
     * @copydoc File::fileType()
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
     * @brief Return the Part Number of the Load.
     *
     * @return Part Number of the Media Set.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the Part Number of the Load.
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
     * A (data) file is a tuple of filename, part number and check value type.
     *
     * @{
     **/

    /**
     * @brief Returns the data files.
     *
     * @param[in] effective
     *   If set to true the effective check value type is returned for each file.
     *
     * @return Data files.
     **/
    [[nodiscard]] ConstLoadFiles dataFiles( bool effective = false ) const;

    /**
     * @brief Sets Data Files.
     *
     * @param[in] files
     *   Files to set.
     **/
    void dataFiles( const ConstLoadFiles &files );

    /**
     * @brief Add the given file as data file.
     *
     * @param[in] file
     *   Data File.
     * @param[in] partNumber
     *   Part Number.
     * @param[in] checkValueType
     *   Check Value Type used for this File Entry.
     *   If set to {} the inherited value will be used.
     *
     * @sa dataFilesCheckValueType()
     **/
    void dataFile(
      const ConstRegularFilePtr& file,
      std::string_view partNumber,
      const std::optional< Arinc645::CheckValueType >& checkValueType );

    //! @copydoc dataFile(const ConstRegularFilePtr&,std::string_view,const std::optional<Arinc645::CheckValueType>&)
    void dataFile(
      const ConstRegularFilePtr &file,
      std::string &&partNumber,
      std::optional< Arinc645::CheckValueType >&& checkValueType = {} );

    /** @} **/

    /**
     * @name Support Files
     * A (support) file is a tuple of filename, part number and check value
     * type.
     *
     * @{
     **/

    /**
     * @brief Returns the Support Files.
     *
     * @param[in] effective
     *   If set to true the effective check value type is returned for each
     *   file.
     *
     * @return Support Files.
     **/
    [[nodiscard]] ConstLoadFiles supportFiles( bool effective = false ) const;

    /**
     * @brief Sets Support Files.
     *
     * @param[in] files
     *   Files to set.
     **/
    void supportFiles( const ConstLoadFiles &files );

    /**
     * @brief Add the given file as support file.
     *
     * @param[in] file
     *   Support File.
     * @param[in] partNumber
     *   Part Number.
     * @param[in] checkValueType
     *   Check Value Type used for this File Entry.
     *   Uf set to {} the inherited value will be used.
     *
     * @sa supportFilesCheckValueType()
     **/
    void supportFile(
      const ConstRegularFilePtr &file,
      std::string_view partNumber,
      const std::optional< Arinc645::CheckValueType >& checkValueType );

    //! @copydoc supportFile(const ConstRegularFilePtr&,std::string_view,const std::optional<Arinc645::CheckValueType>&)
    void supportFile(
      const ConstRegularFilePtr &file,
      std::string &&partNumber,
      std::optional< Arinc645::CheckValueType >&& checkValueType = {} );

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
    [[nodiscard]] ConstUserDefinedDataSpan userDefinedData() const;

    //! @copydoc Load::userDefinedData() const
    [[nodiscard]] UserDefinedData& userDefinedData();

    /**
     * @brief Sets the user-defined data stored in the load header.
     *
     * @param[in] userDefinedData
     *   Updated user-defined data.
     **/
    void userDefinedData( ConstUserDefinedDataSpan userDefinedData );

    //! @copydoc Load::userDefinedData(ConstUserDefinedDataSpan)
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

    /**
     * @name Load Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of Load Check Value.
     * If not provided, the Media Set Check Value is used.
     *
     * @{
     **/

    /**
     * @brief Returns the effective Load Check Value Type.
     *
     * if no check value is given for Load Check Value Type, the Media Set Check
     * Value Type is used.
     *
     * @return Effective Load Check Value Type
     *
     * @sa MediaSet::mediaSetCheckValueType
     **/
    [[nodiscard]] Arinc645::CheckValueType effectiveLoadCheckValueType() const;

    /**
     * @brief Returns the Load Check Value Type.
     *
     * @return Load Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    loadCheckValueType() const;

    /**
     * @brief Updates the Load Check Value Type
     *
     * @param[in] checkValueType
     *   New Load Check Value Type.
     **/
    void loadCheckValueType(
      std::optional< Arinc645::CheckValueType > checkValueType );

    /** @} **/

    /**
     * @name Data Files Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of Data Files Check Value.
     * If not provided, the Media Set Check Value is used.
     *
     * @{
     **/

    /**
     * @brief Returns the effective Data Files Check Value Type.
     *
     * if no check value is given for Data Files Check Value Type, the Media Set
     * Check Value Type is used.
     *
     * @return Effective Data Files Check Value Type
     *
     * @sa MediaSet::mediaSetCheckValueType
     **/
    [[nodiscard]] Arinc645::CheckValueType
    effectiveDataFilesCheckValueType() const;

    /**
     * @brief Returns the Data Files Check Value Type.
     *
     * @return Data Files Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    dataFilesCheckValueType() const;

    /**
     * @brief Updates the Data Files Check Value Type
     *
     * @param[in] checkValueType
     *   New Data Files Check Value Type.
     **/
    void dataFilesCheckValueType(
      std::optional< Arinc645::CheckValueType > checkValueType );

    /** @} **/

    /**
     * @name Support Files Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of Support Files Check Value.
     * If not provided, the Media Set Check Value is used.
     *
     * @{
     **/

    /**
     * @brief Returns the effective Support Files Check Value Type.
     *
     * if no check value is given for Support Files Check Value Type, the Media
     * Set Check Value Type is used.
     *
     * @return Effective Support Files Check Value Type
     *
     * @sa MediaSet::mediaSetCheckValueType
     **/
    [[nodiscard]] Arinc645::CheckValueType
    effectiveSupportFilesCheckValueType() const;

    /**
     * @brief Returns the Support Files Check Value Type.
     *
     * @return Support Files Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    supportFilesCheckValueType() const;

    /**
     * @brief Updates the Support Files Check Value Type
     *
     * @param[in] checkValueType
     *   New Support Files Check Value Type.
     **/
    void supportFilesCheckValueType(
      std::optional< Arinc645::CheckValueType > checkValueType );

    /** @} **/

  private:
    //! Weak %Load %File ( file, Part Number, Check Value Type ).
    using WeakLoadFile = std::tuple<
      ConstRegularFilePtr::weak_type,
      std::string,
      std::optional< Arinc645::CheckValueType > >;
    //! Weak %Load %File List.
    using WeakLoadFiles = std::list< WeakLoadFile >;

    //! Part Flags
    uint16_t partFlagsV{};
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
    //! Load Check Value Type
    std::optional< Arinc645::CheckValueType > loadCheckValueTypeV;
    //! Data Files Default Check Value Type
    std::optional< Arinc645::CheckValueType > dataFilesCheckValueTypeV;
    //! Support Files Default Check Value Type
    std::optional< Arinc645::CheckValueType > supportFilesCheckValueTypeV;
};

}

#endif
