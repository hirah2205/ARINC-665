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
#include <arinc665/file/BatchTargetInfo.hpp>

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

    explicit BatchFile( Arinc665Version version);

    BatchFile(
      Arinc665Version version,
      const string &partNumber,
      const string &comment,
      const BatchTargetsInfo &targets);

    BatchFile(
      Arinc665Version version,
      string &&partNumber,
      string &&comment,
      BatchTargetsInfo &&targets);

    BatchFile( const RawFile &rawFile);

    //! @copydoc Arinc665File::operator=
    virtual BatchFile& operator=( const RawFile &rawFile) override;

    /**
     * @brief Returns the part number of the batch file.
     *
     * @return The part number of the batch file.
     **/
    string getPartNumber() const;

    /**
     * @brief Updates the part number of the batch file.
     *
     * @param[in] partNumber
     *   The new part number.
     **/
    void setPartNumber( const string &partNumber);

    /**
     * @brief Returns the comment text of the batch file.
     *
     * @return The comment text of the batch file.
     **/
    string getComment() const;

    /**
     * @brief Updates the comment text of the batch file.
     *
     * @param[in] comment
     *   The new comment text.
     **/
    void setComment( const string &comment);

    /**
     * @brief Returns the target hardwares information.
     *
     * @return The target hardwares information.
     **/
    const BatchTargetsInfo& getTargetHardwares() const;

    //! @copydoc getTargetHardwares() const
    BatchTargetsInfo& getTargetHardwares();

    void addTargetHardware( const BatchTargetInfo &targetHardwareInfo);

    void addTargetHardware( BatchTargetInfo &&targetHardwareInfo);

  private:
    //! @copydoc Arinc665File::encode
    virtual RawFile encode() const override final;

    void decodeBody( const RawFile &rawFile);

    RawFile encodeBatchTargetsInfo() const;

    void decodeBatchTargetsInfo(
      const RawFile &rawFile,
      std::size_t offset);

    string partNumber;
    string comment;
    BatchTargetsInfo targetHardwares;
};

}
}

#endif
