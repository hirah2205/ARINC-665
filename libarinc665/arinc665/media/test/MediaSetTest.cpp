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
 * @brief Definition of unit tests for class MediaSet.
 **/


#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {
namespace Media {

BOOST_AUTO_TEST_SUITE( MediaSetTest)

BOOST_AUTO_TEST_CASE( testMediaSet1)
{
  MediaSet mediaSet( "XXX");

  BOOST_CHECK( mediaSet.getPartNumber() == "XXX");

  mediaSet.setPartNumber( "YYY");

  BOOST_CHECK( mediaSet.getPartNumber() == "YYY");

  BOOST_CHECK( mediaSet.getNumberOfMedia() == 1);

  MediumPtr medium = mediaSet.getMedium( 0);

  BOOST_CHECK( medium->getNumberOfFiles() == 0);

  BOOST_CHECK( medium->getNumberOfFiles() == 1);

  FilePtr file2 = medium->getFile( "FILE1");

  BOOST_CHECK( file2);

  BOOST_CHECK( file2->getPartNumber() == "PART1");
}

BOOST_AUTO_TEST_SUITE_END()

}
}
