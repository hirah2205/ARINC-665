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
#include <arinc665/media/Base.hpp>

#include <arinc645/Arinc645.hpp>

#include <string_view>
#include <span>
#include <optional>

namespace Arinc665::Media {

/**
 * @brief ARINC 665 %Media Set.
 *
 * A %Media Set consists of one or more media.
 *
 * Each media consists of one or more files.
 **/
class ARINC665_EXPORT MediaSet : public Base
{
  public:
    /**
     * @brief Creates a new %Media Set.
     *
     * This %Media Set contains no media, data files, loads nor batches.
     **/
    MediaSet() = default;

    //! @copydoc Base::mediaSet() const
    [[nodiscard]] ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::mediaSet()
    [[nodiscard]] MediaSetPtr mediaSet() final;

    //! @copydoc Base::type() const
    [[nodiscard]] Type type() const final;

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
     * @name Media
     *
     * A %Media Set consists of 1 up to 255 %Media.
     *
     * @{
     **/

    /**
     * @brief Get the Number of %Media within the %Media Set.
     *
     * @return Number of %Media within the %Media Set.
     **/
    [[nodiscard]] uint8_t numberOfMedia() const;

    /**
     * @brief Returns all %Media.
     *
     * @return All %Media of the %Media Set.
     **/
    [[nodiscard]] ConstMedia media() const;

    //! @copydoc MediaSet::media() const
    [[nodiscard]] Media media();

    /**
     * @brief Set the Number of %Media.
     *
     * @param[in] numberOfMedia
     *   Number of media add.
     **/
    void addMedia( uint8_t numberOfMedia );

    /**
     * @brief Return the %Medium with the requested Medium Number.
     *
     * @param[in] number
     *   %Medium Number [1..255].
     *
     * @return %Medium with the requested number.
     * @retval {}
     *   if @p number is invalid
     **/
    [[nodiscard]] ConstMediumPtr medium( uint8_t number ) const;

    //! @copydoc medium(uint8_t) const
    [[nodiscard]] MediumPtr medium( uint8_t number );

    /**
     * @brief Adds a %Medium to the %Media Set and returns the %Medium.
     *
     * @return %Media index of the new medium.
     * @retval {}
     *   If maximum number of media (255) reached.
     **/
    [[nodiscard]] MediumPtr addMedium();

    /**
     * @brief Removes the last %Medium.
     **/
    void removeMedium();

    /** @} **/

    /**
     * @name Files
     *
     * All files regardless of file type (regular file, load, batch).
     *
     * @{
     **/

    /**
     * @brief Return the total number of files within the %Media Set.
     *
     * @return Number of files.
     **/
    [[nodiscard]] size_t numberOfFiles() const;

    /**
     * @brief Returns all files present on the %Media Set.
     *
     * @return All files.
     **/
    [[nodiscard]] ConstFiles files() const;

    /**
     * @brief Returns all files present on the %Media Set.
     *
     * @return All files.
     **/
    [[nodiscard]] Files files();

    /**
     * @brief Returns files with the given filename.
     *
     * @param[in] filename
     *   Filename.
     *
     * @return Files with the given filename.
     * @retval {}
     *   If no file with @p filename is found.
     **/
    [[nodiscard]] ConstFiles files( std::string_view filename ) const;

    //! @copydoc files(std::string_view) const
    [[nodiscard]] Files files( std::string_view filename );

    /** @} **/

    /**
     * @name Regular Files
     *
     * Files, which are not Load or Batch.
     *
     * @{
     **/

    /**
     * @brief Return the number of Regular %Files within the %Media Set.
     *
     * @return Number of loads within the %Media Set.
     **/
    [[nodiscard]] size_t numberOfRegularFiles() const;

    /**
     * @brief Returns the Regular %Files within the %Media Set.
     *
     * @return Loads within the %Media Set.
     **/
    [[nodiscard]] ConstRegularFiles regularFiles() const;

    /**
     * @brief Returns the Regular %Files within the %Media Set.
     *
     * @return Loads within the %Media Set.
     **/
    [[nodiscard]] RegularFiles regularFiles();

    /**
     * @brief return the Regular %File with the given filename.
     *
     * @param[in] filename
     *   Filename of the load.
     *
     * @return Load with the given filename.
     **/
    [[nodiscard]] ConstRegularFiles regularFiles(
      std::string_view filename ) const;

    //! @copydoc regularFiles(std::string_view) const
    [[nodiscard]] RegularFiles regularFiles( std::string_view filename );

    /** @} **/

    /**
     * @name Loads
     * @{
     **/

    /**
     * @brief Return the number of loads within the %Media Set.
     *
     * @return Number of loads within the %Media Set.
     **/
    [[nodiscard]] size_t numberOfLoads() const;

    /**
     * @brief Returns the loads within the %Media Set.
     *
     * @return Loads within the %Media Set.
     **/
    [[nodiscard]] ConstLoads loads() const;

    /**
     * @brief Returns the loads within the %Media Set.
     *
     * @return Loads within the %Media Set.
     **/
    [[nodiscard]] Loads loads();

    /**
     * @brief return the load with the given filename.
     *
     * @param[in] filename
     *   The filename of the load.
     *
     * @return Load with the given filename.
     **/
    [[nodiscard]] ConstLoads loads( std::string_view filename ) const;

    //! @copydoc loads(std::string_view) const
    [[nodiscard]] Loads loads( std::string_view filename );

    /**
     * @brief Return all Loads, the @p file is referenced.
     *
     * %File could be a data or support file.
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
     * @brief Return the number of batches within the %Media Set.
     *
     * @return Number of batches within the %Media Set.
     **/
    [[nodiscard]] size_t numberOfBatches() const;

    /**
     * @brief Returns the batches within the %Media Set.
     *
     * @return Batches within the %Media Set.
     **/
    [[nodiscard]] ConstBatches batches() const;

    /**
     * @brief Returns the batches within the %Media Set.
     *
     * @return Batches within the %Media Set.
     **/
    [[nodiscard]] Batches batches();

    /**
     * @brief return the batch with the given filename.
     *
     * @param[in] filename
     *   Filename of the batch.
     *
     * @return Batches with the given filename.
     **/
    [[nodiscard]] ConstBatches batches( std::string_view filename ) const;

    //! @copydoc batches(std::string_view) const
    [[nodiscard]] Batches batches( std::string_view filename );

    /**
     * @brief Return all Batches, the @p load is referenced.
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
     * @param[in] effective
     *   If set to true the effective value is returned.
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
    //! %Media
    Media mediaV;
    //! Part Number
    std::string partNumberV;
    //! User Defined Data for Files List Files
    UserDefinedData filesUserDefinedDataV;
    //! User Defined Data for Loads List Files
    UserDefinedData loadsUserDefinedDataV;
    //! User Defined Data for Batches List Files
    UserDefinedData batchesUserDefinedDataV;
    //! ARINC 645 Check Value for %Media Set Generation
    std::optional< Arinc645::CheckValueType > mediaSetCheckValueTypeV;
    //! ARINC 645 Check Value Type for List of Files Generation
    std::optional< Arinc645::CheckValueType > listOfFilesCheckValueTypeV;
    //! ARINC 645 Check Value Type for List of Loads Generation
    std::optional< Arinc645::CheckValueType > listOfLoadsCheckValueTypeV;
    //! ARINC 645 Check Value Type for List of Batches Generation
    std::optional< Arinc645::CheckValueType > listOfBatchesCheckValueTypeV;
    //! ARINC 645 Check Value for %Media Set %File List Generation
    std::optional< Arinc645::CheckValueType > filesCheckValueTypeV;
};

}

#endif
