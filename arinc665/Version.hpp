/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Version.
 **/

#ifndef ARINC665_VERSION_HPP
#define ARINC665_VERSION_HPP

#include <arinc665/Arinc665.hpp>

#include <cstdint>
#include <string>
#include <string_view>

namespace Arinc665 {

//! ARINC 665 C++ Library %Version Information.
class Version
{
  public:
    // delete constructors
    Version() = delete;

    //! Major Version
    static const uint32_t Major;
    //! Minor Version
    static const uint32_t Minor;
    //! Patch Version
    static const uint32_t Patch;

    //! Additional Version Information
    static const std::string_view Addition;

    //! Version Information as String.
    static const std::string_view VersionInformation;
};

}

#endif
