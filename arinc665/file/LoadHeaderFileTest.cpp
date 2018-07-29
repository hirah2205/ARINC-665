/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#include <arinc665/file/LoadHeaderFile.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Arinc665::File {

BOOST_AUTO_TEST_SUITE( LoadHeaderFileTest)


BOOST_AUTO_TEST_CASE( constructor1)
{
  RawFile rawFile{
    // header file length
    0x00, 0x00, 0x00, 0x35,
    // Format version
    0x80, 0x03,
    // spare
    0x00, 0x00,
    // Pointer to Load Part Number
    0x00, 0x00, 0x00, 0x0E,
    // Pointer to Target HW ID list
    0x00, 0x00, 0x00, 0x12,
    // Pointer to Data File list
    0x00, 0x00, 0x00, 0x16,
    // Pointer to Support File list
    0x00, 0x00, 0x00, 0x23,
    // Pointer to User Defined Data
    0x00, 0x00, 0x00, 0x30,
    // Expansion Point No.1

    /* 14 */
    // Load PN Length
    0x00, 0x05,
    // Load PN
    'P', 'N', '1', '2', '3', 0x00,

    /* 18 */
    // Number of Target HW IDs
    0x00, 0x01,

    // Target HW ID Length
    0x00, 0x04,
    // Target HW ID
    'T', 'H', 'W', '0',

    /* 22 */
    // Number of Data Files
    0x00, 0x01,

    // Data File Pointer
    0x00, 0x00,
    // Data File Name Length
    0x00, 0x05,
    // Data File Name
    'F', 'I', 'L', 'E', '1', 0x00,

    // Data File PN Length
    0x00, 0x06,
    // Data File PN
    'P', 'N', '0', '0', '0', '1',

    // Data File Length
    0x00, 0x00, 0x00, 0x10,

    // Data File CRC
    0xAB, 0xCD,

    // Expansion Point No. 2

    /* 35 */

    // Number of Support Files
    0x00, 0x01,

    // Support File Pointer
    0x00, 0x00,

    // Support File Name Length
    0x00, 0x05,
    // Support File Name
    'F', 'I', 'L', 'E', '2', 0x00,

    // Support File PN Length
    0x00, 0x06,
    // Support File PN
    'P', 'N', '0', '0', '0', '2',
    // Support File Length
    0x00, 0x00, 0x00, 0x10,
    // Support File CRC
    0xAB, 0xCD,
    // Expansion Point No. 3

    // Expansion Point No. 4

    /* 48 */
    // User Defined Data
    0x12, 0x34, 0x56, 0x78,

    // Header File CRC
    0xA1, 0x6C,

    // Load CRC
    0xFE, 0xDC, 0xBA, 0x98
    /* 53 */
  };

  // std::cout << std::hex << LoadHeaderFile::calculateChecksum( rawFile, 6U)<<"\n";

  LoadHeaderFile file{ rawFile};

  BOOST_CHECK( file.arincVersion() == Arinc665Version::ARINC_665_2);

  BOOST_CHECK( file.partNumber() == "PN123");

  const auto& targetHardwareIds{ file.targetHardwareIds()};
  BOOST_CHECK( targetHardwareIds.size() == 1);
  BOOST_CHECK( *targetHardwareIds.begin() == "THW0");

  const auto &dataFiles{ file.dataFiles()};
  BOOST_CHECK( dataFiles.size() == 1);
  BOOST_CHECK( dataFiles.begin()->filename() == "FILE1");
  BOOST_CHECK( dataFiles.begin()->partNumber() == "PN0001");
  BOOST_CHECK( dataFiles.begin()->length() == 16);
  BOOST_CHECK( dataFiles.begin()->crc() == 0xABCD);

  const auto &supportFiles{ file.supportFiles()};
  BOOST_CHECK( supportFiles.size() == 1);
  BOOST_CHECK( supportFiles.begin()->filename() == "FILE2");
  BOOST_CHECK( supportFiles.begin()->partNumber() == "PN0002");
  BOOST_CHECK( supportFiles.begin()->length() == 16);
  BOOST_CHECK( supportFiles.begin()->crc() == 0xABCD);

  BOOST_CHECK(
    (file.userDefinedData() == LoadHeaderFile::UserDefinedData{ 0x12, 0x34, 0x56, 0x78}));

  BOOST_CHECK( file.loadCrc() == 0xFEDCBA98);
}

BOOST_AUTO_TEST_SUITE_END()

}
