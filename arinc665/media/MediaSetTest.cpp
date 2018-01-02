/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of unit tests for class Arinc665::Media::MediaSet.
 **/


#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>

#include <helper/Logger.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {
namespace Media {

BOOST_AUTO_TEST_SUITE( MediaSetTest)

//! Constructor test
BOOST_AUTO_TEST_CASE( constructor)
{
  MediaSet mediaSet( "XXX");

  BOOST_CHECK( mediaSet.getName() == "XXX");
  BOOST_CHECK( mediaSet.getPartNumber() == "");
  BOOST_CHECK( mediaSet.getNumberOfMedia() == 0);
  BOOST_CHECK( mediaSet.getType() == MediaSet::Type::MediaSet);

  BOOST_CHECK_THROW( mediaSet.shared_from_this(), std::bad_weak_ptr);

  BOOST_CHECK( mediaSet.getNumberOfFiles() == 0);
  BOOST_CHECK( mediaSet.getFiles().empty());

  BOOST_CHECK( mediaSet.getNumberOfLoads() == 0);
  BOOST_CHECK( mediaSet.getLoads().empty());

  BOOST_CHECK( mediaSet.getNumberOfBatches() == 0);
  BOOST_CHECK( mediaSet.getBatches().empty());
}

//! part number test
BOOST_AUTO_TEST_CASE( partNumber)
{
  MediaSet mediaSet( "XXX");
  BOOST_CHECK( mediaSet.getPartNumber() == "");

  mediaSet.setPartNumber( "YYY");

  BOOST_CHECK( mediaSet.getPartNumber() == "YYY");
}

//! name test
BOOST_AUTO_TEST_CASE( name)
{
  MediaSet mediaSet( "XXX");
  BOOST_CHECK( mediaSet.getName() == "XXX");

  mediaSet.setName( "YYY");

  BOOST_CHECK( mediaSet.getName() == "YYY");
}

//! medium test
BOOST_AUTO_TEST_CASE( medium)
{
  MediaSetPtr mediaSet = std::make_shared< MediaSet>( "XXX");
  BOOST_CHECK( mediaSet->getNumberOfMedia() == 0);

  BOOST_CHECK( mediaSet->addMedium());
  BOOST_CHECK( mediaSet->getNumberOfMedia() == 1);

  BOOST_CHECK( mediaSet->addMedium());
  BOOST_CHECK( mediaSet->getNumberOfMedia() == 2);

  BOOST_CHECK_NO_THROW( mediaSet->setNumberOfMedia( 5, false));
  BOOST_CHECK( mediaSet->getNumberOfMedia() == 5);

#if 0
  BOOST_CHECK_NO_THROW( mediaSet->setNumberOfMedia( 1, false));
  BOOST_CHECK( mediaSet->getNumberOfMedia() == 1);
#endif
}

BOOST_AUTO_TEST_SUITE_END()

}
}
