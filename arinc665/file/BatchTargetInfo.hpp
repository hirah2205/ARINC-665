/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::BatchTargetInfo.
 **/

#ifndef ARINC665_FILE_BATCHTARGETINFO_HPP
#define ARINC665_FILE_BATCHTARGETINFO_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/BatchLoadInfo.hpp>

#include <string>

namespace Arinc665::File {

/**
 * @brief The target hardware information, which is part of the batch file.
 **/
class BatchTargetInfo
{
  public:
    /**
     * @brief Initialises the batch target information.
     *
     * @param[in] targetHardwareId
     *   Target hardware ID.
     * @param[in] loads
     *   Batch loads information.
     **/
    BatchTargetInfo(
      const std::string &targetHardwareId,
      const BatchLoadsInfo &loads);

    //! @copydoc BatchTargetInfo::BatchTargetInfo(const std::string&,const BatchLoadsInfo&)
    BatchTargetInfo(
      std::string &&targetHardwareId,
      BatchLoadsInfo &&loads);

    /**
     * @brief Returns the target hardware ID for this loads info.
     *
     * @return The target hardware id
     **/
    std::string targetHardwareId() const;

    /**
     * @brief Updates the target hardware ID for this loads info.
     *
     * @param[in] targetHardwareId
     *   The target hardware ID (THW ID)
     **/
    void targetHardwareId( const std::string &targetHardwareId);

    //! @copydoc targetHardwareId(const std::string&)
    void targetHardwareId( std::string &&targetHardwareId);

    /**
     * @brief Returns the list of available loads for this target hardware.
     *
     * @return The list of loads (const version).
     **/
    const BatchLoadsInfo& loads() const;

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
    //! The target hardware id
    std::string targetHardwareIdV;
    //! The list of loads
    BatchLoadsInfo loadsV;
};

}

#endif
