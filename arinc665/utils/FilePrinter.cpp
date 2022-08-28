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

#include "FilePrinter.hpp"

#include <arinc665/files/FileListFile.hpp>
#include <arinc665/files/LoadListFile.hpp>
#include <arinc665/files/BatchListFile.hpp>
#include <arinc665/files/LoadHeaderFile.hpp>

#include <arinc645/CheckValueTypeDescription.hpp>

#include <fmt/format.h>

namespace Arinc665::Utils {

static std::string FilePrinter_checkValue(
  const Arinc645::CheckValue &checkValue );

void FilePrinter_print(
  const Arinc665::Files::FileListFile &fileListFile,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  outS
    << fmt::format(
      "{0}media set PN: {1}\n"
      "{0}media seq no: {2:02d}\n"
      "{0}no of media set members: {3:02d}\n",
      initialIndent,
      fileListFile.mediaSetPn(),
      fileListFile.mediaSequenceNumber(),
      fileListFile.numberOfMediaSetMembers() );

  for ( const auto &file : fileListFile.files() )
  {
    outS
      << fmt::format(
        "{0}Filename: {1}\n"
        "{0}File Path: {2}\n"
        "{0}File Member Sequence Number: {3:02d}\n"
        "{0}File CRC: 0x{4:04X}\n"
        "{0}File Check Value: {5}\n\n",
        nextIndent,
        file.filename,
        file.pathName,
        file.memberSequenceNumber,
        file.crc,
        FilePrinter_checkValue( file.checkValue ) );
  }
}

void FilePrinter_print(
  const Arinc665::Files::LoadListFile &loadListFile,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  outS
    << fmt::format(
      "{0}media set PN: {1}\n"
      "{0}media seq no: {2:02d}\n"
      "{0}no of media set members: {3:02d}\n\n",
      initialIndent,
      loadListFile.mediaSetPn(),
      loadListFile.mediaSequenceNumber(),
      loadListFile.numberOfMediaSetMembers() );

  for ( const auto & load : loadListFile.loads() )
  {
    outS
      << fmt::format(
        "{0}load PN: {1}\n"
        "{0}load header filename: {2}\n"
        "{0}load member sequence number: {3:02d}\n\n",
        nextIndent,
        load.partNumber,
        load.headerFilename,
        load.memberSequenceNumber );

    for ( const auto & thw : load.targetHardwareIds )
    {
      outS << nextIndent << "target hardware id: " << thw << "\n\n";
    }
  }
}

void FilePrinter_print(
  const Arinc665::Files::BatchListFile &batchListFile,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  outS
    << fmt::format(
         "{0}media set PN: {1}\n"
         "{0}media seq no: {2:02d}\n"
         "{0}no of media set members: {3:02d}\n\n",
         initialIndent,
         batchListFile.mediaSetPn(),
         batchListFile.mediaSequenceNumber(),
         batchListFile.numberOfMediaSetMembers() );

  for ( const auto & batch : batchListFile.batches() )
  {
    outS
      << fmt::format(
           "{0}Batch PN: {1}\n"
           "{0}Batch filename: {2}\n"
           "{0}Batch member sequence number: {3}\n\n",
           nextIndent,
           batch.partNumber,
           batch.filename,
           batch.memberSequenceNumber );
  }
}

void FilePrinter_print(
  const Arinc665::Files::LoadHeaderFile &loadHeaderFile,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  outS << initialIndent << "part number: " << loadHeaderFile.partNumber() << "\n";

  for ( auto const &targetHardwareId : loadHeaderFile.targetHardwareIds() )
  {
    outS << initialIndent << "Target HW ID: " << targetHardwareId << "\n";
  }

  for ( auto const &dataFile : loadHeaderFile.dataFiles() )
  {
    outS
      << fmt::format(
        "{0}Data File Filename: {1}\n"
        "{0}Data File PN: {2}\n"
        "{0}Data File Size: {3}\n"
        "{0}Data File CRC: 0x{4:02X}\n"
        "{0}Data File Check Value: {5}\n\n",
        nextIndent,
        dataFile.filename,
        dataFile.partNumber,
        dataFile.length,
        dataFile.crc,
        FilePrinter_checkValue( dataFile.checkValue ) );
  }

  for ( auto const &supportFile : loadHeaderFile.supportFiles() )
  {
    outS
      << fmt::format(
        "{0}Support File Filename: {1}\n"
        "{0}Support File PN: {2}\n"
        "{0}Support File Size: {3}\n"
        "{0}Support File CRC: 0x{4:02X}\n"
        "{0}Support File Check Value: {5}\n\n",
        nextIndent,
        supportFile.filename,
        supportFile.partNumber,
        supportFile.length,
        supportFile.crc,
        FilePrinter_checkValue( supportFile.checkValue ) );
  }
}

static std::string FilePrinter_checkValue(
  const Arinc645::CheckValue &checkValue )
{
  const auto& [ checkValueType, checkValueRaw ]{ checkValue };

  std::string checkValueRawString{};
  checkValueRawString.reserve( checkValueRaw.size() * 2U );

  for ( const auto &checkValueByte : checkValueRaw )
  {
    checkValueRawString += fmt::format( "{:02X}", checkValueByte );
  }

  return fmt::format(
    "{} {}",
    Arinc645::CheckValueTypeDescription::instance().name( checkValueType ),
    checkValueRawString );
}

}
