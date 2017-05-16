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
 * @brief Declaration of class Arinc665::FileTypeDescription.
 **/

#ifndef ARINC665_FILETYPEDESCRIPTION_HPP
#define ARINC665_FILETYPEDESCRIPTION_HPP

#include <arinc665/Arinc665.hpp>

#include <helper/Description.hpp>

#include <iosfwd>

namespace Arinc665{

//! ARINC 615A operation description
class FileTypeDescription : public Description<
  FileTypeDescription,
  FileType,
  FileType::Invalid>
{
  public:
    FileTypeDescription();
};

/**
 * @brief Streaming operator to output file type as string
 *
 * @param[in,out] stream
 *   The output stream
 * @param[in] operation
 *   The operation
 *
 * @return The stream for chaining.
 **/
std::ostream& operator<<( std::ostream &stream, FileType fileType);

}

#endif
