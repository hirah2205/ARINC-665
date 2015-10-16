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
 * @brief Declaration of class Arinc665::CheckCode.
 **/

#ifndef ARINC665_CHECKCODE_HPP
#define ARINC665_CHECKCODE_HPP

#include <arinc665/Arinc665.hpp>
#include <string>
#include <cstdint>

namespace Arinc665
{
	class ManufacturerCode;
	class ProductIdentifier;

	using std::string;

	/**
	 * @brief Represents the check code of an ARINC 665 part number.
	 **/
	class CheckCode
	{
		public:
			//! The length of the check code when represented as string
			static constexpr unsigned int LENGTH = 2;

			/**
			 * @brief Generates CheckCode class with the given check code.
			 *
			 * @param[in] checkCode
			 *   The check code.
			 **/
			explicit CheckCode( const uint8_t checkCode);

			/**
			 * @brief Generates CheckCode class for the given check code string.
			 *
			 * The string is converted to a number and stored.
			 *
			 * @param[in] checkCode
			 *   The check code string.
			 *
			 * @throw Arinc665Exception
			 *   When checkCode is not a valid check code string.
			 **/
			explicit CheckCode( const string &checkCode);

			/**
			 * @brief Calculates the checkcode from the given manufacturer code and
			 *   product identifier.
			 *
			 * @param[in] manufacturerCode
			 *   The manufacturer code.
			 * @param[in] productIdentifier
			 *   The product identifier.
			 **/
			CheckCode(
				const ManufacturerCode &manufacturerCode,
				const ProductIdentifier &productIdentifier);

			/**
			 * @brief Returns the check code
			 *
			 * @return The check code.
			 **/
			uint8_t get( void) const;

			/**
			 * @brief Returns the check code as string.
			 *
			 * @return The check code as string
			 **/
			string getStr( void) const;

			/**
			 * @brief Set the check code to the given value.
			 *
			 * @param[in] checkCode
			 *   The check code value.
			 **/
			void set( const uint8_t checkCode);

			/**
			 * @brief Decodes the check code from the given string.
			 *
			 * @param[in] checkCode
			 *   Check code represented as string
			 *
			 * @throw Arinc665Exception
			 *   When the length of the string is invalid
			 **/
			void set( const string &checkCode);

			/**
			 * @brief Re-calculates the check code from the given input data.
			 *
			 * @param[in] manufacturerCode
			 *   The manufacturer code
			 * @param[in] productIdentifier
			 *   The product identifier.
			 **/
			void calculate(
				const ManufacturerCode &manufacturerCode,
				const ProductIdentifier &productIdentifier);

			/**
			 * @brief Check code comparison operator
			 *
			 * @param[in] rhs
			 *   The other check code against which it is compared.
			 *
			 * @return The equality of the both check codes.
			 **/
			bool operator == ( const CheckCode &rhs) const;

			/**
			 * @brief Check code comparison operator
			 *
			 * @param[in] rhs
			 *   The other check code against which it is compared.
			 *
			 * @return The inequality of the both check codes.
			 **/
			bool operator != ( const CheckCode &rhs) const;

		private:
			//! The check code value
			uint8_t checkCode;
	};
}

#endif
