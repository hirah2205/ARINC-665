/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of unit tests for class Arinc665::Media::MediaSet.
 **/

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>

#include <helper/Logger.hpp>

#include <boost/test/unit_test.hpp>

using namespace std::string_view_literals;

namespace Arinc665::Media {

BOOST_AUTO_TEST_SUITE( Arinc665Test)
BOOST_AUTO_TEST_SUITE( MediaSetTest)

//! Constructor test
BOOST_AUTO_TEST_CASE( constructor)
{
  MediaSet mediaSet;

  BOOST_CHECK( mediaSet.partNumber() == "");
  BOOST_CHECK( mediaSet.numberOfMedia() == 0);
  BOOST_CHECK( mediaSet.type() == MediaSet::Type::MediaSet);

  BOOST_CHECK_THROW( auto ptr( mediaSet.shared_from_this()), std::bad_weak_ptr);

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
  BOOST_CHECK( mediaSet.partNumber() == "");

  mediaSet.partNumber( "YYY"sv);

  BOOST_CHECK( mediaSet.partNumber() == "YYY");
}

//! medium test
BOOST_AUTO_TEST_CASE( medium)
{
  MediaSetPtr mediaSet = std::make_shared< MediaSet>();
  BOOST_CHECK( mediaSet->numberOfMedia() == 0);

  BOOST_CHECK( mediaSet->addMedium());
  BOOST_CHECK( mediaSet->numberOfMedia() == 1);

  BOOST_CHECK( mediaSet->addMedium());
  BOOST_CHECK( mediaSet->numberOfMedia() == 2);

  BOOST_CHECK_NO_THROW( mediaSet->numberOfMedia( 5, false));
  BOOST_CHECK( mediaSet->numberOfMedia() == 5);

#if 0
  BOOST_CHECK_NO_THROW( mediaSet->setNumberOfMedia( 1, false));
  BOOST_CHECK( mediaSet->getNumberOfMedia() == 1);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}
