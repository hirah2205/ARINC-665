/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::FileTypeDescription.
 **/

#ifndef ARINC665_FILETYPEDESCRIPTION_HPP
#define ARINC665_FILETYPEDESCRIPTION_HPP

#include <arinc665/Arinc665.hpp>

#include <helper/Description.hpp>

#include <iosfwd>

namespace Arinc665{

//! ARINC 665 %File Type Description.
class ARINC665_EXPORT FileTypeDescription : public Helper::Description<
  FileTypeDescription,
  FileType,
  FileType::Invalid >
{
  public:
    //! Initialises Description
    FileTypeDescription();
};

/**
 * @brief Streaming operator to output ARINC 665 file type as string
 *
 * @param[in,out] stream
 *   The output stream
 * @param[in] fileType
 *   The ARINC 665 file type.
 *
 * @return The stream for chaining.
 **/
ARINC665_EXPORT std::ostream& operator<<(
  std::ostream &stream,
  FileType fileType );

}

#endif
