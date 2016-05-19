/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @date $Date$
 * @version $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief ARINC 665 unit tests execution.
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
