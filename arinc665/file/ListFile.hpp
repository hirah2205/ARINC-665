/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::ListFile.
 **/


#ifndef ARINC665_LISTFILE_HPP
#define ARINC665_LISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>

namespace Arinc665 {
namespace File {

/**
 * @brief Base class for list files.
 *
 * Children are:
 * * FileListFile
 * * LoadListFile
 * * BatchListFile
 **/
class ListFile: public Arinc665File
{
  public:
    /**
     * @brief Returns the Media Set Part Number, where the files belongs to.
     *
     * @return The Media Set Part Number.
     **/
    virtual string getMediaSetPn() const = 0;

    /**
     * @brief Sets the Media Part Number, where the files belongs to.
     *
     * @param[in] mediaSetPn
     *   The Media Set part Number.
     **/
    virtual void setMediaSetPn( const string &mediaSetPn) = 0;

    /**
     *
     * @return
     **/
    virtual uint8_t getMediaSequenceNumber() const = 0;

    /**
     *
     * @param mediaSequenceNumber
     **/
    virtual void setMediaSequenceNumber( uint8_t mediaSequenceNumber) = 0;

    /**
     *
     * @return
     **/
    virtual uint8_t getNumberOfMediaSetMembers() const = 0;

    /**
     *
     * @param numberOfMediaSetMembers
     **/
    virtual void setNumberOfMediaSetMembers(
      uint8_t numberOfMediaSetMembers) = 0;

  protected:
    //! @copydoc Arinc665File::Arinc665File(FileType,Arinc665Version,std::size_t)
    ListFile(
      FileType fileType,
      Arinc665Version version,
      std::size_t checksumPosition = 2U);

    //! @copydoc Arinc665File::Arinc665File(FileType,const RawFile&,std::size_t)
    ListFile(
      FileType fileType,
      const RawFile &rawFile,
      std::size_t checksumPosition = 2U);
};

}
}

#endif
