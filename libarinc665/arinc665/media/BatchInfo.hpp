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
 * @brief Declaration of class Arinc665::Media::BatchInfo.
 **/

#ifndef ARINC665_MEDIA_BATCHINFO_HPP
#define ARINC665_MEDIA_BATCHINFO_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/Load.hpp>

#include <string>
#include <list>

namespace Arinc665 {
namespace Media {

/**
 * @brief
 **/
class BatchInfo
{
  public:
    using string = std::string;
    using LoadList = std::list< WeakLoadPtr>;

    BatchInfo( const string &targetHardwareId);

    string getTargetHardwareId( void) const;

    void setTargetHardwareId( const string &targetHardwareId);

    LoadList& getLoadList( void);

    void addLoad( const WeakLoadPtr load);

  private:
    string targetHardwareId;
    LoadList loadList;
};

}
}

#endif
