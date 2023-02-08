/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Unit Tests for Class Arinc665::Media::MediaSet.
 **/

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <helper/Logger.hpp>

#include <boost/test/unit_test.hpp>

using namespace std::string_view_literals;

namespace Arinc665::Media {

BOOST_AUTO_TEST_SUITE( Arinc665Test)
BOOST_AUTO_TEST_SUITE( MediaTest)
BOOST_AUTO_TEST_SUITE( MediaSetTest)

//! Constructor test
BOOST_AUTO_TEST_CASE( constructor)
{
  MediaSet mediaSet;

  BOOST_CHECK( mediaSet.partNumber().empty() );
  BOOST_CHECK( mediaSet.lastMediumNumber() == MediumNumber{ 1U } );
  BOOST_CHECK( mediaSet.type() == Type::MediaSet );

  BOOST_CHECK_THROW( auto ptr( mediaSet.shared_from_this()), std::bad_weak_ptr );

  BOOST_CHECK( mediaSet.numberOfFiles() == 0);
  BOOST_CHECK( mediaSet.files().empty());

  BOOST_CHECK( mediaSet.numberOfLoads() == 0);
  BOOST_CHECK( mediaSet.loads().empty());

  BOOST_CHECK( mediaSet.numberOfBatches() == 0);
  BOOST_CHECK( mediaSet.batches().empty());
}

//! part number test
BOOST_AUTO_TEST_CASE( partNumber)
{
  MediaSet mediaSet;
  BOOST_CHECK( mediaSet.partNumber().empty() );

  mediaSet.partNumber( "YYY" );

  BOOST_CHECK( mediaSet.partNumber() == "YYY" );
}

//! medium test
BOOST_AUTO_TEST_CASE( medium)
{
  auto mediaSet{ std::make_shared< MediaSet >() };
  BOOST_CHECK( mediaSet->lastMediumNumber() == MediumNumber{ 1U } );

  BOOST_CHECK( mediaSet->addRegularFile( "Medium1File", Arinc665::MediumNumber{ 1U } ) );
  BOOST_CHECK( mediaSet->lastMediumNumber() == MediumNumber{ 1U } );

  BOOST_CHECK( mediaSet->addRegularFile( "Medium2File", Arinc665::MediumNumber{ 2U } ) );
  BOOST_CHECK( mediaSet->lastMediumNumber() == MediumNumber{ 2U } );

  BOOST_CHECK( mediaSet->addRegularFile( "Medium2aFile", Arinc665::MediumNumber{ 2U } ) );
  BOOST_CHECK( mediaSet->lastMediumNumber() == MediumNumber{ 2U } );

  BOOST_CHECK( mediaSet->addRegularFile( "Medium6File", Arinc665::MediumNumber{ 6U } ) );
  BOOST_CHECK( mediaSet->lastMediumNumber() == MediumNumber{ 6U } );

  BOOST_CHECK_NO_THROW( mediaSet->removeFile( "Medium6File") );
  BOOST_CHECK( mediaSet->lastMediumNumber() == MediumNumber{ 2U } );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}
