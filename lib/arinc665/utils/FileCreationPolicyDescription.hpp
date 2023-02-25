/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FileCreationPolicyDescription.
 **/

#ifndef ARINC665_UTILS_FILECREATIONPOLOCYDESCRIPTION_HPP
#define ARINC665_UTILS_FILECREATIONPOLOCYDESCRIPTION_HPP

#include <arinc665/utils/Utils.hpp>

#include <helper/Description.hpp>

#include <iosfwd>

namespace Arinc665::Utils {

//! %File Creation Policy Description
class ARINC665_EXPORT FileCreationPolicyDescription :
  public Helper::Description<
    FileCreationPolicyDescription,
    FileCreationPolicy >
{
  public:
    //! Constructs and adds the entries
    FileCreationPolicyDescription();
};

/**
 * @brief Streaming operator to output File creation policy as string
 *
 * @param[in,out] stream
 *   Output Stream
 * @param[in] fileCreationPolicy
 *   File Creation Policy.
 *
 * @return Output Stream for chaining.
 **/
ARINC665_EXPORT std::ostream& operator<<(
  std::ostream &stream,
  FileCreationPolicy fileCreationPolicy );

/**
 * @brief Streaming operator for parsing a string as file creation policy.
 *
 * @param[in,out] stream
 *   Input stream
 * @param[out] fileCreationPolicy
 *   Decoded file creation policy
 *
 * @return Input Stream for chaining.
 *
 * @throw boost::program_options
 *   When @p stream cannot be decoded to FileCreationPolicy.
 **/
ARINC665_EXPORT std::istream& operator>>(
  std::istream& stream,
  FileCreationPolicy &fileCreationPolicy );

}

#endif
