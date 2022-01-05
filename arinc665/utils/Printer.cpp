/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Defintinion of Module Arinc665::Utils Printer.
 **/

#include "Printer.hpp"

#include <arinc665/media/MediaSet.hpp>

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
  const Media::ConstMediumPtr &medium,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent );

void printMediaSet(
  const Media::ConstMediaSetPtr &mediaSet,
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
    << "Media Set Part Number: '" << mediaSet->partNumber() << "'\n";

  // print files
  outS
    << initialIndent
    << "Media:" << "\n";

  // iterate over files
  for ( auto const &[number, medium] : mediaSet->media() )
  {
    printMedium( medium, outS, nextIndent, indent );
    outS << "\n";
  }

  // print Loads
  outS
    << initialIndent
    << "Loads:" << "\n";

  // iterate over loads
  for ( auto const &load : mediaSet->loads() )
  {
    printLoad( load, outS, nextIndent, indent );
    outS << "\n";
  }

  // iterate over Batches
  outS
    << initialIndent
    << "Batches:" << "\n";

  // iterate over loads
  for ( auto const &batch : mediaSet->batches() )
  {
    printBatch( batch, outS, nextIndent, indent );
    outS << "\n";
  }
}

void printFile(
  const Media::ConstFilePtr &file,
  std::ostream &outS,
  std::string_view initialIndent )
{
  outS
    << initialIndent
    << "File Path: "
    << file->path().generic_string() << "\n";

  outS
    << initialIndent
    << "File Type: ";

  switch ( file->fileType() )
  {
    case Arinc665::Media::BaseFile::FileType::RegularFile:
      outS << "Regular File";
      break;

    case Arinc665::Media::BaseFile::FileType::BatchFile:
      outS << "Batch File";
      break;

    case Arinc665::Media::BaseFile::FileType::LoadFile:
      outS << "Load Header File";
      break;

    default:
      outS << "Illegal Value";
      break;
  }
  outS << "\n";
}

void printLoad(
  const Media::ConstLoadPtr &load,
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
    << load->name() << "\n"

    << initialIndent
    << "Load Path: "
    << load->path().generic_string() << "\n"

    << initialIndent
    << "Load Part Nummer: '"
    << load->partNumber() << "'\n";

  if ( const auto type{ load->loadType() }; type )
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
  for ( auto const &[ thwId, positions ] : load->targetHardwareIdPositions() )
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
  for ( const auto &[file, partNumber] : load->dataFiles() )
  {
    outS
      << nextIndent
      << "File Name: "
      << file.lock()->name() << "\n"

      << nextIndent
      << "File Path: "
      << file.lock()->path().generic_string() << "\n"

      << nextIndent
      << "File Part Number: '"
      << partNumber << "'\n";

    outS << "\n";
  }

  outS
    << initialIndent
    << "Support Files:\n";

  // iterate over Support Files
  for ( const auto &[file, partNumber] : load->supportFiles() )
  {
    outS
      << nextIndent
      << "File Name: "
      << file.lock()->name() << "\n"

      << nextIndent
      << "File Path: "
      << file.lock()->path().generic_string() << "\n"

      << nextIndent
      << "Part Number: '"
      << partNumber << "'\n";

    outS << "\n";
  }
}

void printBatch(
  const Media::ConstBatchPtr &batch,
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
    << "Batch Name: " << batch->name() << "\n"

    << initialIndent
    << "Batch Path: "
    << batch->path().generic_string() << "\n"

    << initialIndent
    << "Batch Part Number: '" << batch->partNumber() << "'\n"

    << initialIndent
    << "Comment: '" << batch->comment() << "'\n";

  outS
    << initialIndent
    << "Target Loads:\n";

  // iterate over Target list
  for ( auto const & [target,loads] : batch->targets() )
  {
    outS
      << nextIndent
      << "Loads: " << target << "\n";

    for ( const auto &load : loads )
    {
      outS
        << nextNextIndent
        << "Name: " << load.lock()->name() << "\n"

        << nextNextIndent
        << "Load Path: "
        << load.lock()->path().generic_string() << "\n";

      outS << "\n";
    }

    outS << "\n";
  }
}

static void printMedium(
  const Media::ConstMediumPtr &medium,
  std::ostream &outS,
  std::string_view initialIndent,
  std::string_view indent )
{
  std::string nextIndent{ initialIndent };
  nextIndent += indent;

  outS
    << initialIndent
    << "Medium Number: "
    << (int)medium->mediumNumber() << "\n";

  outS
    << initialIndent
    << "Files:\n";

  // iterate over files
  for ( auto const &file : medium->files( true ) )
  {
    printFile( file, outS, nextIndent );
    outS << "\n";
  }
}

}
