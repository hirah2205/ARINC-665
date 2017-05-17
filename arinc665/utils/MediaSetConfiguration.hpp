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
 * @brief Declaration of class Arinc665::Utils::MediaSetConfiguration.
 **/

#ifndef ARINC665_UTILS_MEDIASETCONFIGURATION_HPP
#define ARINC665_UTILS_MEDIASETCONFIGURATION_HPP

#include <arinc665/utils/Utils.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include <vector>
#include <map>

namespace Arinc665 {
namespace Utils {

class MediaSetConfiguration
{
  public:
    using path = boost::filesystem::path;
    using string = std::string;

    using MediaPaths = std::vector< path>;
    using MediaSets = std::map< string, MediaPaths>;

    MediaSetConfiguration( const boost::property_tree::ptree &config);

    path mediaSetBase;
    MediaSets mediaSets;
};

}
}

#endif
