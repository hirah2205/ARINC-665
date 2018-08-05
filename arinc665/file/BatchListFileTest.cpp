/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of unit tests for the class Arinc665::File::BatchListFile.
 **/

#include <arinc665/file/BatchListFile.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Dump.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Arinc665::File {

BOOST_AUTO_TEST_SUITE( BatchListFileTest)


BOOST_AUTO_TEST_CASE( constructor1)
{
  RawFile rawFile{
    // header file length
    0x00, 0x00, 0x00, 0x28,
    // Format version
    0xA0, 0x03,
    // spare
    0x00, 0x00,
    // Pointer to Media Information
    0x00, 0x00, 0x00, 0x0A,
    // Pointer to Number of Batches
    0x00, 0x00, 0x00, 0x0F,
    // Pointer to User Defined Data
    0x00, 0x00, 0x00, 0x24,
    // Expansion Point No.1

    /* 20 */
    // Media Set PN Length
    0x00, 0x05,
    // Media Set PN
    'P', 'N', '1', '2', '3', 0x00,
    // Media Sequnece Number
    0x01,
    // Number of media set members
    0x01,

    /* 30 */
    // Number of Batches
    0x00, 0x02,

    /* 32 */
    // Batch pointer
    0x00, 0x0A,
    // Batch PN length
    0x00, 0x05,
    // Batch PN
    'P', 'N', '0', '0', '1', 0x00,
    // Batch Name Length
    0x00, 0x06,
    // Batch Name
    'F', 'N', '_', '0', '0', '1',
    // Member Sequence Number
    0x00, 0x01,
    // Expansion Point No. 2

    /* 52 */
    // Batch pointer
    0x00, 0x00,
    // Batch PN length
    0x00, 0x05,
    // Batch PN
    'P', 'N', '0', '0', '2', 0x00,
    // Batch File Name Length
    0x00, 0x06,
    // Batch File Name
    'F', 'N', '_', '0', '0', '2',
    // Member Sequence Number
    0x00, 0x01,
    // Expansion Point No. 2

    // Expansion Point No. 3

    /* 72 */
    // User Defined Data
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,

    // FILE CRC
    0xE1, 0x18
  };

  // std::cout << std::hex << BatchListFile::calculateChecksum( rawFile) << std::endl;

  BatchListFile file{ rawFile};

  BOOST_CHECK( file.arincVersion() == SupportedArinc665Version::Supplement2);

  BOOST_CHECK( file.mediaSetPn() == "PN123");
  BOOST_CHECK( file.mediaSequenceNumber() == 1);
  BOOST_CHECK( file.numberOfMediaSetMembers() == 1);

  const auto& batches{ file.batches()};
  BOOST_CHECK( batches.size() == 2);

  auto batch{ batches.begin()};
  BOOST_CHECK( batch->partNumber() == "PN001");
  BOOST_CHECK( batch->filename() == "FN_001");
  BOOST_CHECK( batch->memberSequenceNumber() == 1);

  ++batch;
  BOOST_CHECK( batch->partNumber() == "PN002");
  BOOST_CHECK( batch->filename() == "FN_002");
  BOOST_CHECK( batch->memberSequenceNumber() == 1);


  BOOST_CHECK( (file.userDefinedData() ==
    BatchListFile::UserDefinedData{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06}));

  const auto raw2{ static_cast< RawFile>( file)};

  // std::cout << Dump( &(*(raw2.begin())), raw2.size());

  BOOST_CHECK( rawFile == raw2);
}

BOOST_AUTO_TEST_SUITE_END()

}
