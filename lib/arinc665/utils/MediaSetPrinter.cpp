/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Module Arinc665::Utils Media Set Printer.
 **/

#include "MediaSetPrinter.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc645/CheckValueTypeDescription.hpp>
#include <arinc645/CheckValue.hpp>

namespace Arinc665::Utils {

/**
 * @brief Print Medium
 *
 * @param[in] medium
 *   ARINC 665 Medium
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
static void printMedium(
  const Media::Medium &medium,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent );

static void printFiles(
  const Media::ContainerEntity &containerEntity,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent );

static std::string printCheckValueType(
  std::optional< Arinc645::CheckValueType > type );

void MediaSetPrinter_print(
  const Media::MediaSet &mediaSet,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  std::string nextNextIndent{ nextIndent };
  nextNextIndent += indent;

  outS
    << initialIndent
    << "Media Set Part Number: '" << mediaSet.partNumber() << "'\n"

    << initialIndent
    << "Number of Media: " << (int)mediaSet.numberOfMedia() << "\n"

    << initialIndent
    << "Media Set Check Value Type: "
    << printCheckValueType( mediaSet.mediaSetCheckValueType() )
    << " - "
    << printCheckValueType( mediaSet.effectiveMediaSetCheckValueType() ) << "\n"

    << initialIndent
    << "List of Files Check Value Type: "
    << printCheckValueType( mediaSet.listOfFilesCheckValueType() )
    << " - "
    << printCheckValueType( mediaSet.effectiveListOfFilesCheckValueType() ) << "\n"

    << initialIndent
    << "List of Loads Check Value Type: "
    << printCheckValueType( mediaSet.listOfLoadsCheckValueType() )
    << " - "
    << printCheckValueType( mediaSet.effectiveListOfLoadsCheckValueType() ) << "\n"

    << initialIndent
    << "List of Batches Check Value Type: "
    << printCheckValueType( mediaSet.listOfBatchesCheckValueType() )
    << " - "
    << printCheckValueType( mediaSet.effectiveListOfBatchesCheckValueType() ) << "\n"

    << initialIndent
    << "Files Check Value Type: "
    << printCheckValueType( mediaSet.filesCheckValueType() )
    << " - "
    << printCheckValueType( mediaSet.effectiveFilesCheckValueType() ) << "\n";

  // print files
  outS
    << initialIndent
    << "Media:" << "\n";

  // iterate over files
  for ( auto const &[ number, medium ] : mediaSet.media() )
  {
    printMedium( *medium, outS, nextIndent, indent );
    outS << "\n";
  }

  // print Loads
  outS
    << initialIndent
    << "Loads:" << "\n";

  // iterate over loads
  for ( auto const &load : mediaSet.loads() )
  {
    MediaSetPrinter_print( *load, outS, nextIndent, indent );
    outS << "\n";
  }

  if ( mediaSet.numberOfBatches() != 0U )
  {
    // iterate over Batches
    outS << initialIndent << "Batches:" << "\n";

    // iterate over loads
    for ( auto const &batch : mediaSet.batches() )
    {
      MediaSetPrinter_print( *batch, outS, nextIndent, indent );
      outS << "\n";
    }
  }
}

void MediaSetPrinter_print(
  const Media::File &file,
  std::ostream &outS,
  std::string_view initialIndent )
{
  outS
    << initialIndent
    << "File Path: "
    << file.path().generic_string() << "\n";

  outS
    << initialIndent
    << "File Type: ";

  switch ( file.fileType() )
  {
    case Arinc665::Media::FileType::RegularFile:
      outS << "Regular File";
      break;

    case Arinc665::Media::FileType::BatchFile:
      outS << "Batch File";
      break;

    case Arinc665::Media::FileType::LoadFile:
      outS << "Load Header File";
      break;

    default:
      outS << "Illegal Value";
      break;
  }
  outS << "\n";

  outS
    << initialIndent
    << "File Check Value Type: "
    << printCheckValueType( file.checkValueType() )
    << " - "
    << printCheckValueType( file.effectiveCheckValueType() ) << "\n";

}

void MediaSetPrinter_print(
  const Media::Load &load,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  std::string nextNextIndent{ nextIndent };
  nextNextIndent += indent;

  outS
    << initialIndent
    << "Load Name: "
    << load.name() << "\n"

    << initialIndent
    << "Load Path: "
    << "[" << (int)load.medium()->mediumNumber() << "]:"
    << load.path().generic_string() << "\n"

    << initialIndent
    << "Load Part Nummer: '"
    << load.partNumber() << "'\n"

    << initialIndent
    << "Load Check Value Type: '"
    << printCheckValueType( load.loadCheckValueType() )
    << " - "
    << printCheckValueType( load.effectiveLoadCheckValueType() ) << "\n"

    << initialIndent
    << "Data Files Check Value Type: '"
    << printCheckValueType( load.dataFilesCheckValueType() )
    << " - "
    << printCheckValueType( load.effectiveDataFilesCheckValueType() ) << "\n"

    << initialIndent
    << "Support Files Check Value Type: '"
    << printCheckValueType( load.supportFilesCheckValueType() )
    << " - "
    << printCheckValueType( load.effectiveSupportFilesCheckValueType() ) << "\n";

  if ( const auto &type{ load.loadType() }; type )
  {
    outS
      << initialIndent
      << "Load Type: '" << type->first
      << "' 0x" << std::hex << type->second << std::dec << "\n";
  }

  outS
    << initialIndent
    << "Compatible THW IDs:\n";

  // iterate over THW ID list
  for ( auto const &[ thwId, positions ] : load.targetHardwareIdPositions() )
  {
    outS
      << nextIndent
      << "THW ID: " << thwId << "\n";

    for ( auto const &position : positions )
    {
      outS
        << nextNextIndent
        << "Position: " << position << "\n";
    }
    outS << "\n";
  }

  outS
    << initialIndent
    << "Data Files:\n";

  // iterate over Data Files
  for ( const auto &[ file, partNumber, checkValueType ] : load.dataFiles() )
  {
    outS
      << nextIndent
      << "File Name: "
      << file->name() << "\n"

      << nextIndent
      << "File Path: "
      << "[" << (int)file->medium()->mediumNumber() << "]:"
      << file->path().generic_string() << "\n"

      << nextIndent
      << "File Part Number: '"
      << partNumber << "'\n"

      << nextIndent
      << "Check Value Type: '"
      << printCheckValueType( checkValueType )
      << " - "
      << printCheckValueType( checkValueType.value_or(
        load.effectiveDataFilesCheckValueType() ) ) << "'\n";

    outS << "\n";
  }

  outS
    << initialIndent
    << "Support Files:\n";

  // iterate over Support Files
  for ( const auto &[ file, partNumber, checkValueType ] : load.supportFiles() )
  {
    outS
      << nextIndent
      << "File Name: "
      << file->name() << "\n"

      << nextIndent
      << "File Path: "
      << "[" << (int)file->medium()->mediumNumber() << "]:"
      << file->path().generic_string() << "\n"

      << nextIndent
      << "Part Number: '"
      << partNumber << "'\n"

      << nextIndent
      << "Check Value Type: '"
      << printCheckValueType( checkValueType )
      << " - "
      << printCheckValueType( checkValueType.value_or(
        load.effectiveSupportFilesCheckValueType() ) ) << "'\n";

    outS << "\n";
  }
}

void MediaSetPrinter_print(
  const Media::Batch &batch,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  std::string nextNextIndent{ nextIndent };
  nextNextIndent += indent;

  outS
    << initialIndent
    << "Batch Name: " << batch.name() << "\n"

    << initialIndent
    << "Batch Path: "
    << "[" << (int)batch.medium()->mediumNumber() << "]:"
    << batch.path().generic_string() << "\n"

    << initialIndent
    << "Batch Part Number: '" << batch.partNumber() << "'\n"

    << initialIndent
    << "Comment: '" << batch.comment() << "'\n";

  outS
    << initialIndent
    << "Target Loads:\n";

  // iterate over Target list
  for ( auto const & [target,loads] : batch.targets() )
  {
    outS
      << nextIndent
      << "Loads: " << target << "\n";

    for ( const auto &load : loads )
    {
      outS
        << nextNextIndent
        << "Name: " << load->name() << "\n"

        << nextNextIndent
        << "Load Path: "
        << "[" << (int)load->medium()->mediumNumber() << "]:"
        << load->path().generic_string() << "\n";

      outS << "\n";
    }

    outS << "\n";
  }
}

void MediaSetPrinter_print(
  const CheckValues &checkValues,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  for ( const auto &[ file, checkValuesSet ] : checkValues )
  {
    outS
      << initialIndent
      << file->path()
      << "\n";

    for ( const auto &checkValue : checkValuesSet )
    {
      outS
        << initialIndent << indent
        << checkValue
        << "\n";
    }
  }
}

static void printMedium(
  const Media::Medium &medium,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  outS
    << initialIndent
    << "Medium Number: "
    << (int)medium.mediumNumber() << "\n";

  outS
    << initialIndent
    << "Files:\n";

  printFiles( medium, outS, initialIndent, indent );
}

static void printFiles(
  const Media::ContainerEntity &containerEntity,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  // iterate over files
  for ( auto const &file : containerEntity.files() )
  {
    MediaSetPrinter_print( *file, outS, nextIndent );
    outS << "\n";
  }

  // iterate over subdirectories
  for ( const auto &container : containerEntity.subdirectories() )
  {
    printFiles( *container, outS, initialIndent, indent );
  }
}

static std::string printCheckValueType(
  std::optional< Arinc645::CheckValueType > type )
{
  return ( type ?
      std::string( Arinc645::CheckValueTypeDescription::instance().name( * type ) ) :
      "***Undefined***" );
}

}
