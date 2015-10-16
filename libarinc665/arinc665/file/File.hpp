/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of the namespace Arinc665::File.
 **/

#ifndef ARINC665_FILE_FILE_HPP
#define ARINC665_FILE_FILE_HPP

#include <arinc665/Arinc665.hpp>

#include <vector>
#include <cstdint>

namespace Arinc665
{
	/**
	 *
	 **/
	namespace File
	{
		//! Raw file type definition.
		typedef std::vector< uint8_t> RawFile;

		class FileFactory;

		class Arinc665File;
		class BatchFile;
		class LoadHeaderFile;
		class BatchListFile;
		class LoadListFile;
		class FileListFile;
	}
}

#endif
