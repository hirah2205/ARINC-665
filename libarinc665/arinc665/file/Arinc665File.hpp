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
 * @brief Declaration of class Arinc665File
 **/

#ifndef ARINC665_ARINC665FILE_HPP
#define ARINC665_ARINC665FILE_HPP

#include <arinc665/file/File.hpp>

#include <cstdint>

namespace Arinc665
{
	namespace File
	{
		/**
		 * @brief Base class of ARIC 665 file
		 **/
		class Arinc665File
		{
			public:
				static constexpr unsigned int BaseHeaderOffset = 8;

				/**
				 * @brief Decode the file length information from the given file.
				 *
				 * @param[in] file
				 *   The raw ARINC 665 file.
				 *
				 * @return The encoded file length.
				 *
				 * @throw InvalidArinc665File
				 *   If the file size is to small to represent an valid ARINC 665 file.
				 **/
				static uint32_t getFileLength( const RawFile &file);

				/**
				 * @brief Decode the format version information from the given file.
				 *
				 * @param[in] file
				 *   The raw ARINC 665 file.
				 *
				 * @return The encoded format version.
				 *
				 * @throw InvalidArinc665File
				 *   If the file size is to small to represent an valid ARINC 665 file.
				 **/
				static uint16_t getFormatVersion( const RawFile &file);

				/**
				 * @brief Calculates the checksum over the given file.
				 *
				 * @param[in] file
				 *   The raw ARINC 665 file.
				 * @param[in] skipLastBytes
				 *   Defines the number of bytes, which are skipped, e.g. contains the
				 *   stored checksum.
				 *
				 * @return The calculated checksum.
				 **/
				static uint16_t calculateChecksum(
					const RawFile &file,
					const unsigned int skipLastBytes = 2);

				//! Default destructor
				virtual ~Arinc665File( void) noexcept = default;

				/**
				 * @brief Returns the ARINC 665 version of this file.
				 *
				 * @return The ARINC 665 version of this file.
				 **/
				virtual Arinc665Version getArincVersion( void) const = 0;

				uint16_t getCrc( void) const;

				void setCrc( const uint16_t crc);

				// virtual void decode( const RawFile &data);

				// virtual RawFile encode( void) const = 0;

			protected:
				Arinc665File( void);

				Arinc665File(
					const RawFile &file,
					const Arinc665FileFormatVersion expectedFormatVersion,
					const unsigned int checksumPosition = 2);

			private:
				uint16_t crc;
		};
	}
}

#endif
