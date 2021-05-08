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
#include <arinc665/media/BaseFile.hpp>

#include <map>
#include <string>
#include <string_view>

namespace Arinc665::Media {

/**
 * @brief %Batch of Loads.
 **/
class Batch: public BaseFile
{
  public:
    //! Batch Information (Target Hardware ID -> Loads)
    using BatchInfo = std::map< std::string, WeakLoads, std::less<>>;

    /**
     * @brief Initialises the batch with the given data.
     *
     * @param[in] parent
     *   Parent element.
     * @param[in] name
     *   Name of the batch.
     **/
    Batch( const ContainerEntityPtr& parent, std::string_view name);

    //! @copydoc Batch(const ContainerEntityPtr&,std::string_view)
    Batch( const ContainerEntityPtr& parent, std::string &&name);

    /**
     * @copydoc BaseFile::fileType()
     *
     * @return FileType::BatchFile always.
     **/
    [[nodiscard]] FileType fileType() const final;

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
     *   The comment, which describes the batch.
     **/
    void comment( std::string_view comment);

    //! @copydoc comment(std::string_view)
    void comment( std::string &&comment);

    /**
     * @brief Return batches.
     *
     * @return The batch target information.
     **/
    [[nodiscard]] const BatchInfo& targets() const;

    //! @copydoc targets() const
    BatchInfo& targets();

    /**
     * @brief Return the batch info for the given target hardware ID
     *
     * @param[in] targetHardwareId
     *   Target hardware ID
     *
     * @return The corresponding loads
     **/
    [[nodiscard]] WeakLoads target( std::string_view targetHardwareId) const;

    /**
     * @brief Return the batch info for the given target hardware ID
     *
     * @param[in] targetHardwareId
     *   Target hardware ID
     *
     * @return The corresponding loads
     **/
    WeakLoads target( std::string_view targetHardwareId);

    /**
     * @brief Add batch info for the given target hardware ID.
     *
     * @param[in] targetHardwareId
     *   Target hardware IDs
     * @param[in] loads
     *   Loads.
     **/
    void target( std::string_view targetHardwareId, const WeakLoads &loads);

    //! @copydoc target(std::string_view,const WeakLoads&)
    void target( std::string &&targetHardwareId, WeakLoads &&loads);

  private:
    //! Batch comment
    std::string commentV;
    //! Batch information
    BatchInfo batchesV;
};

}

#endif
