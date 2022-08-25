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
#include <arinc665/media/Medium.hpp>

#include <arinc645/Arinc645.hpp>

#include <string_view>
#include <optional>

namespace Arinc665::Media {

/**
 * @brief ARINC 665 %Media Set.
 *
 * A media set consists of one or more media.
 *
 * Each media consists of one or more files.
 **/
class ARINC665_EXPORT MediaSet : public Base
{
  public:
    //! User Defined Data
    using UserDefinedData = std::vector< uint8_t >;

    /**
     * @brief Creates a new media set.
     *
     * This media set contains no media, data files, loads nor batches.
     **/
    MediaSet() = default;

    //! @copydoc Base::mediaSet() const
    ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::mediaSet()
    MediaSetPtr mediaSet() final;

    //! @copydoc Base::type() const
    Type type() const final;

    /**
     * @name Media Set Part Number
     *
     * A Media Set is identified by its unique Part Number.
     *
     * @{
     **/

    /**
     * @brief Return the Part Number of the Media Set.
     *
     * @return Part Number of the Media Set.
     **/
    std::string_view partNumber() const;

    /**
     * @brief Set the Media Set Part Number.
     *
     * @param[in] partNumber
     *   Media Set Part Number.
     **/
    void partNumber( std::string_view partNumber );

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber );

    /** @} **/

    /**
     * @name Media
     *
     * A Media Set consists of 1 up to 255 Media.
     *
     * @{
     **/

    /**
     * @brief Get the Number of Media within the Media Set.
     *
     * @return Number of Media within the Media Set.
     **/
    uint8_t numberOfMedia() const;

    /**
     * @brief Set the Number of Media.
     *
     * If there are fewer media than requested, the media are created empty,
     * otherwise the media are deleted.
     *
     * @param[in] numberOfMedia
     *   The new number of media.
     *   Must be bigger or equal to 1.
     * @param[in] deleteFiles
     *   Delete files, which are part of the not existing media.
     *   Otherwise, they are moved to the first media.
     **/
    void numberOfMedia( uint8_t numberOfMedia, bool deleteFiles = false );

    /**
     * @brief Returns all Media.
     *
     * @return All Media of the Media Set.
     **/
    ConstMedia media() const;

    //! @copydoc MediaSet::media() const
    Media media();

    /**
     * @brief Return the Medium with the requested Index.
     *
     * @param[in] index
     *   Medium Index.
     *
     * @return Medium with the requested Index.
     **/
    ConstMediumPtr medium( uint8_t index ) const;

    /**
     * @brief Return the medium with the requested Index.
     *
     * @param[in] index
     *   Medium Index.
     *
     * @return Medium with the requested Index.
     **/
    MediumPtr medium( uint8_t index );

    /**
     * @brief Adds a Medium to the Media Set and returns the Medium.
     *
     * @return Media index of the new medium.
     * @retval {}
     *   If maximum number of media (255) reached.
     **/
    MediumPtr addMedium();

    /**
     * @brief Removes the last Medium.
     *
     * @param[in] deleteFiles
     *   Delete files, which are part of the not existing medium.
     *   Otherwise, they are moved to the first media.
     **/
    void removeMedium( bool deleteFiles = false );

    /** @} **/

    /**
     * @name Files
     * @{
     **/

    /**
     * @brief Return the total number of files within the media set.
     *
     * @return Number of files.
     **/
    size_t numberOfFiles() const;

    /**
     * @brief Returns all files present on the media set.
     *
     * @return All files.
     **/
    ConstFiles files() const;

    /**
     * @brief Returns all files present on the media set.
     *
     * @return All files.
     **/
    Files files();

    /**
     * @brief Returns file with the given filename.
     *
     * @todo handle multiple files with same name.
     *
     * @param[in] filename
     *   Filename.
     *
     * @return File with the given filename.
     * @retval ConstFilePtr()
     *   If file is not found.
     **/
    ConstFilePtr file( std::string_view filename ) const;

    /**
     * @brief Returns file with the given filename.
     *
     * @todo handle multiple files with same name.
     *
     * @param[in] filename
     *   Filename.
     *
     * @return File with the given filename.
     * @retval ConstFilePtr()
     *   If file is not found.
     **/
    FilePtr file( std::string_view filename );

    /** @} **/

    /**
     * @name Loads
     * @{
     **/

    /**
     * @brief Return the number of loads within the media set.
     *
     * @return Number of loads within the media set.
     **/
    size_t numberOfLoads() const;

    /**
     * @brief Returns the loads within the media set.
     *
     * @return Loads within the media set.
     **/
    ConstLoads loads() const;

    /**
     * @brief Returns the loads within the media set.
     *
     * @return Loads within the media set.
     **/
    Loads loads();

    /**
     * @brief return the load with the given filename.
     *
     * @todo handle multiple loads with same name.
     *
     * @param[in] filename
     *   The filename of the load.
     *
     * @return Load with the given filename.
     **/
    ConstLoadPtr load( std::string_view filename ) const;

    /**
     * @brief return the load with the given filename.
     *
     * @todo handle multiple loads with same name.
     *
     * @param[in] filename
     *   Filename of the load.
     *
     * @return Load with the given filename.
     **/
    LoadPtr load( std::string_view filename );

    /** @} **/

    /**
     * @name Batches
     * @{
     **/

    /**
     * @brief Return the number of batches within the media set.
     *
     * @return Number of batches within the media set.
     **/
    size_t numberOfBatches() const;

    /**
     * @brief Returns the batches within the media set.
     *
     * @return Batches within the media set.
     **/
    ConstBatches batches() const;

    /**
     * @brief Returns the batches within the media set.
     *
     * @return Batches within the media set.
     **/
    Batches batches();

    /**
     * @brief return the batch with the given filename.
     *
     * @todo handle multiple batches with same name.
     *
     * @param[in] filename
     *   Filename of the batch.
     *
     * @return Batch with the given filename.
     **/
    ConstBatchPtr batch( std::string_view filename ) const;

    /**
     * @brief return the batch with the given filename.
     *
     * @todo handle multiple batches with same name.
     *
     * @param[in] filename
     *   Filename of the batch.
     *
     * @return Batch with the given filename.
     **/
    BatchPtr batch( std::string_view filename );

    /** @} **/

    /**
     * @name User Defined Data of Files List
     * @{
     **/

    /**
     * @brief Returns the user defined data for file list files.
     *
     * @return The user defined data for file list files.
     **/
    const UserDefinedData& filesUserDefinedData() const;

    //! @copydoc filesUserDefinedData() const
    UserDefinedData& filesUserDefinedData();

    /**
     * @brief Updates the user defined data for file list files.
     *
     * @param[in] userDefinedData
     *   User defined data.
     **/
    void filesUserDefinedData( const UserDefinedData &userDefinedData );

    //! @copydoc filesUserDefinedData(const UserDefinedData&)
    void filesUserDefinedData( UserDefinedData &&userDefinedData );

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
    const UserDefinedData& loadsUserDefinedData() const;

    //! @copydoc loadsUserDefinedData() const
    UserDefinedData& loadsUserDefinedData();

    /**
     * @brief Updates the user defined data for load list files.
     *
     * @param[in] userDefinedData
     *   User defined data.
     **/
    void loadsUserDefinedData( const UserDefinedData &userDefinedData );

    //! @copydoc loadsUserDefinedData(const UserDefinedData&)
    void loadsUserDefinedData( UserDefinedData &&userDefinedData );

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
    const UserDefinedData& batchesUserDefinedData() const;

    //! @copydoc batchesUserDefinedData() const
    UserDefinedData& batchesUserDefinedData();

    /**
     * @brief Updates the user defined data for batch list files.
     *
     * @param[in] userDefinedData
     *   User Defined Data.
     **/
    void batchesUserDefinedData( const UserDefinedData &userDefinedData );

    //! @copydoc batchesUserDefinedData(const UserDefinedData&)
    void batchesUserDefinedData( UserDefinedData &&userDefinedData );

    /** @} **/

    /**
     * @name Media Set Check Value Type
     *
     * This information is used to determine the Check Value Type on the Media
     * Set.
     * It can be overridden by specific Check Value Types.
     *
     * @{
     **/

    /**
     * @brief Returns the effective Media Set Check Value Type.
     *
     * If no Media Set Check Value Type is given, CheckValueType::NotUsed is
     * returned.
     *
     * @return Effective Media Set Check Value Type
     **/
    Arinc645::CheckValueType effectiveMediaSetCheckValueType() const;

    /**
     * @brief Returns the Media Set Check Value Type.
     *
     * @return Media Set Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    mediaSetCheckValueType() const;

    /**
     * @brief Updates the Media Set Check Value Type.
     *
     * @param[in] type
     *   New Media Set Check Value Type.
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
     * If not provided, the Media Set Check Value is used.
     *
     * @{
     **/

    /**
     * @brief Returns the Effective List of Files Check Value Type.
     *
     * If no value is set, the Media Set Check Value Type is used.
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
     * If no value is set, the Media Set Check Value Type is used.
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
     * For the List of Loads File and List of Batches File separate functions
     * are provided.
     *
     * If not provided, the Media Set Check Value is used.
     *
     * @sa effectiveMediaSetCheckValueType()
     * @sa effectiveListOfLoadsCheckValueType()
     * @sa effectiveListOfBatchesCheckValueType()
     * @{
     **/

    /**
     * @brief Returns the Effective Files Check Value Type.
     *
     * If no value is set, the Media Set Check Value Type is used.
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
    [[nodiscard]] ConstFiles recursiveFiles(
      const ContainerEntity &container ) const;

    [[nodiscard]] Files recursiveFiles( ContainerEntity &container );

    [[nodiscard]] ConstRegularFiles recursiveRegularFiles(
      const ContainerEntity &container ) const;

    [[nodiscard]] RegularFiles recursiveRegularFiles( ContainerEntity &container );

    [[nodiscard]] ConstLoads recursiveLoads(
      const ContainerEntity &container ) const;

    [[nodiscard]] Loads recursiveLoads( ContainerEntity &container );

    [[nodiscard]] ConstBatches recursiveBatches(
      const ContainerEntity &container ) const;

    [[nodiscard]] Batches recursiveBatches( ContainerEntity &container );

    //! Media
    Media mediaV;
    //! Part Number
    std::string partNumberV;
    //! User Defined Data for Files List Files
    UserDefinedData filesUserDefinedDataV;
    //! User Defined Data for Loads List Files
    UserDefinedData loadsUserDefinedDataV;
    //! User Defined Data for Batches List Files
    UserDefinedData batchesUserDefinedDataV;
    //! ARINC 645 Check Value for Media Set Generation
    std::optional< Arinc645::CheckValueType > mediaSetCheckValueTypeV;
    //! ARINC 645 Check Value Type for List of Files Generation
    std::optional< Arinc645::CheckValueType > listOfFilesCheckValueTypeV;
    //! ARINC 645 Check Value Type for List of Loads Generation
    std::optional< Arinc645::CheckValueType > listOfLoadsCheckValueTypeV;
    //! ARINC 645 Check Value Type for List of Batches Generation
    std::optional< Arinc645::CheckValueType > listOfBatchesCheckValueTypeV;
    //! ARINC 645 Check Value for Media Set File List Generation
    std::optional< Arinc645::CheckValueType > filesCheckValueTypeV;
};

}

#endif
