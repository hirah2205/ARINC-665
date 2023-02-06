/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of unit tests for the class Arinc665::MediumNumber.
 **/

#include <arinc665/MediumNumber.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( Arinc665Test )
BOOST_AUTO_TEST_SUITE( MediumNumberTest )

//! Medium Number Test
BOOST_AUTO_TEST_CASE( constructor1 )
{
  MediumNumber mediumNumber{ 1 };

  BOOST_CHECK( static_cast< uint8_t >( mediumNumber )  == 1 );
  BOOST_CHECK( mediumNumber == MediumNumber{ 1 } );
  BOOST_CHECK( static_cast< std::string >( mediumNumber )  == "001" );

  ++mediumNumber;
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber )  == 2 );
  BOOST_CHECK( mediumNumber == MediumNumber{ 2 } );
  BOOST_CHECK( static_cast< std::string >( mediumNumber )  == "002" );

  auto old{ mediumNumber++ };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber )  == 3 );
  BOOST_CHECK( mediumNumber == MediumNumber{ 3U } );
  BOOST_CHECK( static_cast< std::string >( mediumNumber )  == "003" );
  BOOST_CHECK( static_cast< uint8_t >( old )  == 2U );
  BOOST_CHECK( old == MediumNumber{ 2U } );

  mediumNumber = mediumNumber + 4;
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber )  == 7 );
  BOOST_CHECK( static_cast< std::string >( mediumNumber )  == "007" );
  BOOST_CHECK( mediumNumber == MediumNumber{ 7 } );

  mediumNumber = 17;
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber )  == 17 );
  BOOST_CHECK( static_cast< std::string >( mediumNumber )  == "017" );
  BOOST_CHECK( mediumNumber == MediumNumber{ 17 } );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}
