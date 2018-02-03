/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::FileCreationPolicyDescription.
 **/

#ifndef ARINC665_UTILS_FILECREATIONPOLOCYDESCRIPTION_HPP
#define ARINC665_UTILS_FILECREATIONPOLOCYDESCRIPTION_HPP

#include <arinc665/utils/Utils.hpp>

#include <helper/Description.hpp>

#include <iosfwd>

namespace Arinc665{
namespace Utils{

//! File creation policy description
class FileCreationPolicyDescription : public Description<
  FileCreationPolicyDescription,
  FileCreationPolicy,
  FileCreationPolicy::Invalid>
{
  public:
    FileCreationPolicyDescription();
};

/**
 * @brief Streaming operator to output File creation policy as string
 *
 * @param[in,out] stream
 *   The output stream
 * @param[in] fileCreationPolicy
 *   The File Creation Policy.
 *
 * @return The stream for chaining.
 **/
std::ostream& operator<<(
  std::ostream &stream,
  FileCreationPolicy fileCreationPolicy);

/**
 * @brief Streaming operator for parsing a string as file creation policy.
 *
 * @param[in,out] stream
 *   The input stream
 * @param[out] fileCreationPolicy
 *   The decoded file creation policy
 *
 * @return The stream for chaining.
 **/
std::istream& operator>>(
  std::istream& stream,
  FileCreationPolicy &fileCreationPolicy);

}
}
#endif
