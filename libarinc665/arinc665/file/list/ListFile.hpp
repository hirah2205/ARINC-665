/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class ListFile.
 **/


#ifndef ARINC665_LISTFILE_HPP
#define ARINC665_LISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>

#include <string>

namespace Arinc665 {
namespace File {

/**
 * @brief Base class for list files.
 *
 * Childs are:
 * # FileListFile
 * # LoadListFile
 * # BatchListFile
 **/
class ListFile: public Arinc665File
{
  public:
    using string = std::string;

    /**
     * @brief Returns the Media Set Part Number, where the files belongs to.
     *
     * @return The Media Set Part Number.
     **/
    string getMediaSetPn( void) const;

    /**
     * @brief Sets the Media Part Number, where the files belongs to.
     *
     * @param[in] mediaSetPn
     *   The Media Set part Number.
     **/
    void setMediaSetPn( const string &mediaSetPn);

    uint8_t getMediaSequenceNumber( void) const;

    void setMediaSequenceNumber( const uint8_t mediaSequenceNumber);

    uint8_t getNumberOfMediaSetMembers( void) const;

    void setNumberOfMediaSetMembers( const uint8_t numberOfMediaSetMembers);

  protected:
    ListFile( void);

    ListFile(
      const RawFile &file,
      const Arinc665FileFormatVersion expectedFormatVersion,
      const unsigned int skipLastBytes = 2);

  private:
    string mediaSetPn;
    uint8_t mediaSequenceNumber;
    uint8_t numberOfMediaSetMembers;
};
}
}

#endif
