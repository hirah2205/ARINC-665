/*
 * $Date$
 * $Revision$
 */
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

namespace Arinc665 {
namespace File {

/**
 * @brief The target hardware information, which is part of the batch file.
 **/
class BatchTargetInfo
{
  public:
    using string = std::string;

    BatchTargetInfo(
      const string &targetHardwareId,
      const BatchLoadsInfo &loads);

    BatchTargetInfo(
      string &&targetHardwareId,
      BatchLoadsInfo &&loads);

    /**
     * @brief Returns the target hardware ID for this loads info.
     *
     * @return The target hardware id
     **/
    string getTargetHardwareId() const;

    /**
     * @brief Updates the target hardware ID for this loads info.
     *
     * @param[in] targetHardwareId
     *   The target hardware ID (THW ID)
     **/
    void setTargetHardwareId( const string &targetHardwareId);

    /**
     * @brief Returns the list of available loads for this target hardware.
     *
     * @return The list of loads (const version).
     **/
    const BatchLoadsInfo& getLoads() const;

    /**
     * @brief Returns the list of available loads for this target hardware.
     *
     * @return The list of loads (modifiable version).
     **/
    BatchLoadsInfo& getLoads();

    void setLoads( const BatchLoadsInfo &loads);

    void setLoads( BatchLoadsInfo &&loads);

    void addLoad( const BatchLoadInfo &load);

    void addLoad( BatchLoadInfo &&load);

  private:
    //! The target hardware id
    string targetHardwareId;
    //! The list of loads
    BatchLoadsInfo loads;
};

}
}

#endif
