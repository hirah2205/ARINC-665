/*
 * $Date$
 * $Revision$
 */
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

#include <boost/test/unit_test.hpp>

namespace Arinc665 {
namespace Media {

BOOST_AUTO_TEST_SUITE( MediaSetTest)


BOOST_AUTO_TEST_CASE( constructor)
{
  MediaSet mediaSet( "XXX");

  BOOST_CHECK( mediaSet.getPartNumber() == "XXX");
  BOOST_CHECK( mediaSet.getNumberOfMedia() == 0);
  BOOST_CHECK( mediaSet.getType() == MediaSet::Type::MediaSet);
  BOOST_CHECK( mediaSet.getName() == "");

  BOOST_CHECK_THROW( mediaSet.shared_from_this(), std::bad_weak_ptr);

  BOOST_CHECK( mediaSet.getNumberOfFiles() == 0);
  BOOST_CHECK( mediaSet.getFiles().empty());

  BOOST_CHECK( mediaSet.getNumberOfLoads() == 0);
  BOOST_CHECK( mediaSet.getLoads().empty());

  BOOST_CHECK( mediaSet.getNumberOfBatches() == 0);
  BOOST_CHECK( mediaSet.getBatches().empty());
}

BOOST_AUTO_TEST_CASE( partNumber)
{
  MediaSet mediaSet( "XXX");
  BOOST_CHECK( mediaSet.getPartNumber() == "XXX");

  mediaSet.setPartNumber( "YYY");

  BOOST_CHECK( mediaSet.getPartNumber() == "YYY");
}

BOOST_AUTO_TEST_SUITE_END()

}
}
