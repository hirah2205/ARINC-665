// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of unit tests for the class Arinc665::MediumNumber.
 **/

#include <arinc_665/MediumNumber.hpp>
#include <arinc_665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( Arinc665Test )
BOOST_AUTO_TEST_SUITE( MediumNumberTest )

//! Medium Number Test
BOOST_AUTO_TEST_CASE( constructor1 )
{
  MediumNumber mediumNumber;
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber )  == 1U );
}

//! Constructor Test
BOOST_AUTO_TEST_CASE( constructor2 )
{
  MediumNumber mediumNumber1{ 1U };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 ) == 1U );

  MediumNumber mediumNumber2{ 255U };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber2 ) == 255U );
}

//! Constructor Test
BOOST_AUTO_TEST_CASE( preIncrement )
{
  MediumNumber mediumNumber1{ 1 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 1U );

  auto mediumNumber2{ ++mediumNumber1 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 2U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber2 )  == 2U );

  MediumNumber mediumNumber3{ 255U };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 255U );

  auto mediumNumber4{ ++mediumNumber3 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 255U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber4 )  == 255U );
}

//! Post Increment Test
BOOST_AUTO_TEST_CASE( postIncrement )
{
  MediumNumber mediumNumber1{ 1 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 1U );

  auto mediumNumber2{ mediumNumber1++ };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 2U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber2 )  == 1U );

  MediumNumber mediumNumber3{ 255U };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 255U );

  auto mediumNumber4{ mediumNumber3++ };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 255U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber4 )  == 255U );
}

//! Pre Increment Test
BOOST_AUTO_TEST_CASE( preDecrement )
{
  MediumNumber mediumNumber1{ 1 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 1U );

  auto mediumNumber2{ --mediumNumber1 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 1U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber2 )  == 1U );

  MediumNumber mediumNumber3{ 255U };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 255U );

  auto mediumNumber4{ --mediumNumber3 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 254U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber4 )  == 254U );
}

//! Post Decrement Test
BOOST_AUTO_TEST_CASE( postDecrement )
{
  MediumNumber mediumNumber1{ 1 };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 1U );

  auto mediumNumber2{ mediumNumber1-- };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber1 )  == 1U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber2 )  == 1U );

  MediumNumber mediumNumber3{ 255U };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 255U );

  auto mediumNumber4{ mediumNumber3-- };
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber3 )  == 254U );
  BOOST_CHECK( static_cast< uint8_t >( mediumNumber4 )  == 255U );
}

//! Pre Decrement Test
BOOST_AUTO_TEST_CASE( toString )
{
  BOOST_CHECK( static_cast< std::string >( MediumNumber{ 1U } )  == "001" );
  BOOST_CHECK( static_cast< std::string >( MediumNumber{ 99U } )  == "099" );
  BOOST_CHECK( static_cast< std::string >( MediumNumber{ 255U } )  == "255" );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}
