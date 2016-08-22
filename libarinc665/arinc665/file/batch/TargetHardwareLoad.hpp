/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class TargetHardwareLoad
 **/

#ifndef ARINC665_FILE_TARGETHARDWARELOAD_HPP
#define ARINC665_FILE_TARGETHARDWARELOAD_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/batch/BatchLoadInfo.hpp>

#include <string>
#include <list>

namespace Arinc665 {
namespace File {

/**
 * @brief The list of loads for the given target hardware
 **/
class TargetHardwareLoad
{
  public:
    using string = std::string;
    //! Short form of the load list info
    using ListType = std::list< BatchLoadInfo>;

    /**
     * @brief Returns the target hardware ID for this loads info.
     *
     * @return The target hardware id
     **/
    string getTargetHardwareId( void) const;

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
    const ListType& getLoadList( void) const;

    /**
     * @brief Returns the list of available loads for this target hardware.
     *
     * @return The list of loads (modifiable version).
     **/
    ListType& getLoadList( void);

  private:
    //! The target hardware id
    string targetHardwareId;
    //! The list of loads
    ListType loadList;
};

}
}

#endif
