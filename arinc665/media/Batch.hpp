/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::Batch
 **/

#ifndef ARINC665_MEDIA_BATCH_HPP
#define ARINC665_MEDIA_BATCH_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/File.hpp>

#include <map>
#include <string>
#include <string_view>

namespace Arinc665::Media {

/**
 * @brief %Batch of Loads.
 *
 * A Batch is used to declare loads for multiple Target Hardware Items at a
 * time.
 * For each Target Hardware (identified by its Target Hardware ID - THW ID) a
 * list of loads is defined.
 **/
class ARINC665_EXPORT Batch : public File
{
  public:
    /**
     * @brief Initialises the batch with the given data.
     *
     * @param[in] parent
     *   Parent element.
     * @param[in] name
     *   Name of the Batch.
     **/
    Batch( const ContainerEntityPtr& parent, std::string_view name );

    //! @copydoc Batch(const ContainerEntityPtr&,std::string_view)
    Batch( const ContainerEntityPtr& parent, std::string &&name );

    /**
     * @copydoc File::fileType()
     *
     * @return FileType::BatchFile always.
     **/
    [[nodiscard]] FileType fileType() const final;

    /**
     * @name Part Number
     * A batch has its own Part Number.
     *
     * @{
     **/

    /**
     * @brief Return the Part Number of the Batch.
     *
     * @return Part Number of the Batch.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the Part Number
     *
     * @param[in] partNumber
     *   New Part Number
     **/
    void partNumber( std::string_view partNumber );

    /** @} **/

    /**
     * @name Comment
     * @{
     **/

    /**
     * @brief Get the comment, which describes the batch.
     *
     * @return The comment, which describes the batch.
     **/
    [[nodiscard]] std::string_view comment() const;

    /**
     * @brief Set the comment, which describes the batch.
     *
     * @param[in] comment
     *   Comment, which describes the batch.
     **/
    void comment( std::string_view comment );

    //! @copydoc comment(std::string_view)
    void comment( std::string &&comment );

    /** @} **/

    /**
     * @name Batch Targets Information
     * @{
     **/

    /**
     * @brief Return Batch Information for Targets.
     *
     * @return Batch Target Information.
     **/
    [[nodiscard]] ConstBatchInformation targets() const;

    //! @copydoc targets() const
    BatchInformation targets();

    /**
     * @brief Return the Loads for the given Target Hardware ID
     *
     * @param[in] targetHardwareId
     *   Target Hardware ID
     *
     * @return Loads corresponding to @p targetHardwareId
     **/
    [[nodiscard]] ConstLoads target( std::string_view targetHardwareId ) const;

    //! @copydoc target(std::string_view) const
    Loads target( std::string_view targetHardwareId );

    /**
     * @brief Add Loads for the given Target Hardware ID.
     *
     * @param[in] targetHardwareId
     *   Target Hardware ID
     * @param[in] loads
     *   Loads for @p targetHardwareId.
     **/
    void target( std::string_view targetHardwareId, const Loads &loads );

    //! @copydoc target(std::string_view,const Loads&)
    void target( std::string &&targetHardwareId, const Loads &loads );

    /** @} **/

  private:
    //! Batch Information (Target Hardware ID -> Weak Loads)
    using WeakBatchInfo = std::map< std::string, WeakLoads, std::less<> >;

    //! Part Number
    std::string partNumberV;
    //! Batch Comment
    std::string commentV;
    //! Batch Information
    WeakBatchInfo batchesV;
};

}

#endif
