/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::BatchTargetInfo.
 **/

#ifndef ARINC665_FILE_BATCHTARGETINFO_HPP
#define ARINC665_FILE_BATCHTARGETINFO_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/BatchLoadInfo.hpp>

#include <string>
#include <string_view>

namespace Arinc665::File {

/**
 * @brief The target hardware information, which is part of the batch file.
 *
 * Within ARINC 665-2 The Target Hardware ID Position Field is named
 * Target HW ID.
 **/
class BatchTargetInfo
{
  public:
    /**
     * @brief Initialises the batch target information.
     *
     * @param[in] targetHardwareIdPosition
     *   The target hardware ID (THW ID)
     * @param[in] loads
     *   Batch loads information.
     **/
    BatchTargetInfo(
      std::string_view targetHardwareIdPosition,
      const BatchLoadsInfo &loads);

    //! @copydoc BatchTargetInfo::BatchTargetInfo(std::string_view,const BatchLoadsInfo&)
    BatchTargetInfo(
      std::string &&targetHardwareIdPosition,
      BatchLoadsInfo &&loads);

    /**
     * @brief Returns the Target Hardware ID + Position for this batch
     *   information.
     *
     * @return The Target Hardware ID + Position
     **/
    [[nodiscard]] std::string_view targetHardwareIdPosition() const;

    /**
     * @brief Updates the target hardware ID for this loads info.
     *
     * @param[in] targetHardwareIdPosition
     *   The target hardware ID (THW ID)
     **/
    void targetHardwareIdPosition( std::string_view targetHardwareIdPosition);

    //! @copydoc targetHardwareIdPosition(std::string_view)
    void targetHardwareIdPosition( std::string &&targetHardwareIdPosition);

    /**
     * @brief Returns the list of available loads for this target hardware.
     *
     * @return The list of loads (const version).
     **/
    [[nodiscard]] const BatchLoadsInfo& loads() const;

    /**
     * @brief Returns the list of available loads for this target hardware.
     *
     * @return The list of loads (modifiable version).
     **/
    BatchLoadsInfo& loads();

    /**
     * @brief Replaces the stored load information.
     *
     * @param[in] loads
     *   Batch loads information.
     **/
    void loads( const BatchLoadsInfo &loads);

    //! @copydoc loads(const BatchLoadsInfo&)
    void loads( BatchLoadsInfo &&loads);

    /**
     * @brief Adds a load.
     *
     * @param[in] load
     *   The load.
     **/
    void load( const BatchLoadInfo &load);

    //! @copydoc load(const BatchLoadInfo&)
    void load( BatchLoadInfo &&load);

  private:
    //! Target hardware id
    std::string targetHardwareIdPositionV;
    //! List of loads
    BatchLoadsInfo loadsV;
};

}

#endif
