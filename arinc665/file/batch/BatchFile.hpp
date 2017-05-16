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
 * @brief Declaration of class Arinc665::File::BatchFile.
 **/

#ifndef ARINC665_FILE_BATCH_HPP
#define ARINC665_FILE_BATCH_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>
#include <arinc665/file/batch/BatchTargetInfo.hpp>

#include <string>

namespace Arinc665 {
namespace File {

/**
 * @brief A batch file represents a list of loads of target hardwares.
 *
 *
 **/
class BatchFile: public Arinc665File
{
  public:
    using string = std::string;

    explicit BatchFile(
      const string &partNumber = {},
      const string &comment = {},
      const BatchTargetsInfo &targets = {});

    explicit BatchFile(
      string &&partNumber = {},
      string &&comment = {},
      BatchTargetsInfo &&targets = {});

    BatchFile( const RawFile &file);

    virtual Arinc665Version getArincVersion() const override;

    string getPartNumber() const;

    void setPartNumber( const string &partNumber);

    string getComment() const;

    void setComment( const string &comment);

    const BatchTargetsInfo& getTargetHardwares() const;

    BatchTargetsInfo& getTargetHardwares();

  private:
    string partNumber;
    string comment;
    BatchTargetsInfo targetHardwares;
};

}
}

#endif
