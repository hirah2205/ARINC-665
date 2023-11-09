// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::MediaSet.
 **/

#ifndef ARINC665_MEDIA_MEDIASET_HPP
#define ARINC665_MEDIA_MEDIASET_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/ContainerEntity.hpp>

#include <arinc645/Arinc645.hpp>

#include <string_view>
#include <span>
#include <optional>

namespace Arinc665::Media {

/**
 * @brief ARINC 665 %Media Set.
 **/
class ARINC665_EXPORT MediaSet final : public ContainerEntity
{
  public:
    /**
     * @brief Creates a new %Media Set.
     *
     * This %Media Set contains no media, data files, loads nor batches.
     *
     * @return Media Set Instance
     **/
    [[nodiscard]] static MediaSetPtr create();

    /**
     * @brief Creates a new %Media Set.
     *
     * This %Media Set contains no media, data files, loads nor batches.
     *
     * @param[in] createKey
     *   Key to protect creation.
     *
     * @sa @ref MediaSet::create()
     **/
    explicit MediaSet( const CreateKey &createKey );

    //! @copydoc Base::mediaSet() const
    [[nodiscard]] ConstMediaSetPtr mediaSet() const override;

    //! @copydoc Base::mediaSet()
    [[nodiscard]] MediaSetPtr mediaSet() override;

    //! @copydoc Base::type() const
    [[nodiscard]] Type type() const override;

    /**
     * @copydoc ContainerEntity::parent() const
     * @retval {} always.
     **/
    [[nodiscard]] ConstContainerEntityPtr parent() const override;

    //! @copydoc ContainerEntity::parent() const
    [[nodiscard]] ContainerEntityPtr parent() override;

    //! @copydoc ContainerEntity::path() const
    [[nodiscard]] std::filesystem::path path() const override;

    /**
     * @name %Media Set Part Number
     *
     * A %Media Set is identified by its unique Part Number.
     *
     * @{
     **/

    /**
     * @brief Return the Part Number of the %Media Set.
     *
     * @return Part Number of the %Media Set.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Set the %Media Set Part Number.
     *
     * @param[in] partNumber
     *   %Media Set Part Number.
     **/
    void partNumber( std::string partNumber );

    /** @} **/

    /**
     * @name Loads
     * @{
     **/

    /**
     * @brief Return all Loads, where @p file is referenced.
     *
     * This operation is used to reverse search for loads, which references the
     * specified file as data or support file.
     *
     * @param[in] file
     *   %File to look for.
     *
     * @return Loads, which contains @p file.
     **/
    [[nodiscard]] ConstLoads loadsWithFile(
      const ConstRegularFilePtr &file ) const;

    /** @} **/

    /**
     * @name Batches
     * @{
     **/

    /**
     * @brief Return all Batches, where @p load is referenced.
     *
     * This operation is used to reverse search for loads, which references the
     * specified load.
     *
     * @param[in] load
     *   Load to look for.
     *
     * @return Batches, which contains @p load.
     **/
    [[nodiscard]] ConstBatches batchesWithLoad(
      const ConstLoadPtr &load ) const;

    /** @} **/

    /**
     * @name User Defined Data of Files List
     * @{
     **/

    /**
     * @brief Returns the User Defined Data for List of Files.
     *
     * @return User Defined Data for List of Files.
     **/
    [[nodiscard]] ConstUserDefinedDataSpan filesUserDefinedData() const;

    //! @copydoc filesUserDefinedData() const
    [[nodiscard]] UserDefinedData& filesUserDefinedData();

    /**
     * @brief Updates the User Defined Data for List of Files.
     *
     * @param[in] userDefinedData
     *   User Defined Data.
     **/
    void filesUserDefinedData( UserDefinedData userDefinedData );

    /** @} **/

    /**
     * @name User Defined Data of Loads List
     * @{
     **/

    /**
     * @brief Returns the user defined data for loads list files.
     *
     * @return User defined data for loads list files.
     **/
    [[nodiscard]] ConstUserDefinedDataSpan loadsUserDefinedData() const;

    //! @copydoc loadsUserDefinedData() const
    [[nodiscard]] UserDefinedData& loadsUserDefinedData();

    /**
     * @brief Updates the user defined data for load list files.
     *
     * @param[in] userDefinedData
     *   User defined data.
     **/
    void loadsUserDefinedData( UserDefinedData userDefinedData );

    /** @} **/

    /**
     * @name User Defined Data of Batches List
     * @{
     **/

    /**
     * @brief Returns the user defined data for batch list files.
     *
     * @return User defined data for batch list files.
     **/
    [[nodiscard]] ConstUserDefinedDataSpan batchesUserDefinedData() const;

    //! @copydoc batchesUserDefinedData() const
    [[nodiscard]] UserDefinedData& batchesUserDefinedData();

    /**
     * @brief Updates the user defined data for batch list files.
     *
     * @param[in] userDefinedData
     *   User Defined Data.
     **/
    void batchesUserDefinedData( UserDefinedData userDefinedData );

    /** @} **/

    /**
     * @name Media Set Check Value Type
     *
     * This information is used to determine the Check Value Type on the %Media
     * Set.
     * It can be overridden by specific Check Value Types.
     *
     * @{
     **/

    /**
     * @brief Returns the effective %Media Set Check Value Type.
     *
     * If no %Media Set Check Value Type is given, CheckValueType::NotUsed is
     * returned.
     *
     * @return Effective %Media Set Check Value Type
     **/
    [[nodiscard]] Arinc645::CheckValueType
    effectiveMediaSetCheckValueType() const;

    /**
     * @brief Returns the %Media Set Check Value Type.
     *
     * @return %Media Set Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    mediaSetCheckValueType() const;

    /**
     * @brief Updates the %Media Set Check Value Type.
     *
     * @param[in] type
     *   New %Media Set Check Value Type.
     **/
    void mediaSetCheckValueType(
      std::optional< Arinc645::CheckValueType > type );

    /** @} **/

    /**
     * @name List of Files Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of List of Files (`FILES.LUM`) Check Value.
     *
     * If not provided, the %Media Set Check Value is used.
     *
     * @{
     **/

    /**
     * @brief Returns the Effective List of Files Check Value Type.
     *
     * If no value is set, the %Media Set Check Value Type is used.
     *
     * @return Effective List of Files Check Value Type
     *
     * @sa effectiveMediaSetCheckValueType()
     **/
    [[nodiscard]] Arinc645::CheckValueType
    effectiveListOfFilesCheckValueType() const;

    /**
     * @brief Returns the List of Files Check Value Type.
     *
     * @return List of Files Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    listOfFilesCheckValueType() const;

    /**
     * @brief Updates the List of Files Check Value Type
     *
     * @param[in] type
     *   New List of Files Check Value Type.
     **/
    void listOfFilesCheckValueType(
      std::optional< Arinc645::CheckValueType > type );

    /** @} **/

    /**
     * @name List of Loads Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of List of Loads (`LOADS.LUM`) Check Value which is stored
     * within the List of Files (`FILES.LUM`).
     *
     * If not provided, the Files Check Value is used.
     *
     * @sa effectiveFilesCheckValueType()
     *
     * @{
     **/

    /**
     * @brief Returns the Effective List of Loads Check Value Type.
     *
     * If not provided, the Files Check Value is used.
     *
     * @return Effective Files Check Value Type
     *
     * @sa effectiveFilesCheckValueType()
     **/
    [[nodiscard]] Arinc645::CheckValueType
    effectiveListOfLoadsCheckValueType() const;

    /**
     * @brief Returns the List of Loads Check Value Type.
     *
     * @return List of Loads Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    listOfLoadsCheckValueType() const;

    /**
     * @brief Updates the List of Loads Check Value Type
     *
     * @param[in] type
     *   New List of Loads Check Value Type.
     **/
    void listOfLoadsCheckValueType(
      std::optional< Arinc645::CheckValueType > type );

    /** @} **/

    /**
     * @name List of Batches Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of List of Batches (`BATCHES.LUM`) Check Value.
     *
     * If not provided, the Files Check Value is used.
     *
     * @sa effectiveFilesCheckValueType()
     * @{
     **/

    /**
     * @brief Returns the Effective List of Batches Check Value Type.
     *
     * If no value is set, the Files Check Value Type is used.
     *
     * @return Effective List of Batches Check Value Type
     *
     * @sa effectiveFilesCheckValueType()
     **/
    [[nodiscard]] Arinc645::CheckValueType
    effectiveListOfBatchesCheckValueType() const;

    /**
     * @brief Returns the List of Batches Check Value Type.
     *
     * @return List of Batches Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    listOfBatchesCheckValueType() const;

    /**
     * @brief Updates the List of Batches Check Value Type
     *
     * @param[in] type
     *   New List of Batches Check Value Type.
     **/
    void listOfBatchesCheckValueType(
      std::optional< Arinc645::CheckValueType > type );

    /** @} **/

    /**
     * @name Files Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of Files Check Values within `FILES.LUM`.
     * It can be overridden on per-file basis.
     * For the List of Loads %File and List of Batches %File separate functions
     * are provided.
     *
     * If not provided, the %Media Set Check Value is used.
     *
     * @sa effectiveMediaSetCheckValueType()
     * @sa effectiveListOfLoadsCheckValueType()
     * @sa effectiveListOfBatchesCheckValueType()
     * @{
     **/

    /**
     * @brief Returns the Effective Files Check Value Type.
     *
     * If no value is set, the %Media Set Check Value Type is used.
     *
     * @return Effective Files Check Value Type
     *
     * @sa effectiveMediaSetCheckValueType()
     **/
    [[nodiscard]] Arinc645::CheckValueType effectiveFilesCheckValueType() const;

    /**
     * @brief Returns the Files Check Value Type.
     *
     * @return Files Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    filesCheckValueType() const;

    /**
     * @brief Updates the Files Check Value Type
     *
     * @param[in] type
     *   New Files Check Value Type.
     **/
    void filesCheckValueType( std::optional< Arinc645::CheckValueType > type );

    /** @} **/

  private:
    //! Part Number
    std::string partNumberV{};
    //! User Defined Data for Files List Files
    UserDefinedData filesUserDefinedDataV{};
    //! User Defined Data for Loads List Files
    UserDefinedData loadsUserDefinedDataV{};
    //! User Defined Data for Batches List Files
    UserDefinedData batchesUserDefinedDataV{};
    //! ARINC 645 Check Value for %Media Set Generation
    std::optional< Arinc645::CheckValueType > mediaSetCheckValueTypeV{};
    //! ARINC 645 Check Value Type for List of Files Generation
    std::optional< Arinc645::CheckValueType > listOfFilesCheckValueTypeV{};
    //! ARINC 645 Check Value Type for List of Loads Generation
    std::optional< Arinc645::CheckValueType > listOfLoadsCheckValueTypeV{};
    //! ARINC 645 Check Value Type for List of Batches Generation
    std::optional< Arinc645::CheckValueType > listOfBatchesCheckValueTypeV{};
    //! ARINC 645 Check Value for %Media Set %File List Generation
    std::optional< Arinc645::CheckValueType > filesCheckValueTypeV{};
};

}

#endif
