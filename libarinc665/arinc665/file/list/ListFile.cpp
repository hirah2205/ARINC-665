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
 * @brief Definition of class Arinc665::File::ListFile.
 **/

#include "ListFile.hpp"

namespace Arinc665 {
namespace File {

ListFile::ListFile( void)
{
}

ListFile::ListFile(
  const RawFile &file,
  const Arinc665FileFormatVersion expectedFormatVersion,
  const unsigned int skipLastBytes) :
  Arinc665File( file, expectedFormatVersion, skipLastBytes)
{
}

}
}
