/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @brief unit test start-up.
 **/

#include <helper/Logger.hpp>

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

void testCheckCode( void);
void testPartNumber( void);
void testMediaSet( void);

boost::unit_test::test_suite* init_unit_test_suite( int, char*[])
{
	testCheckCode();
	testPartNumber();
	testMediaSet();

	return 0;
}
