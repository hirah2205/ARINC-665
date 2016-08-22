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
 * @brief Declaration of class Arinc665::Media::BaseFile.
 **/

#ifndef ARINC665_MEDIA_BASEFILE_HPP
#define ARINC665_MEDIA_BASEFILE_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/PartNumberdEntity.hpp>

namespace Arinc665 {
namespace Media {

/**
 * @brief The base class of all files, presented within a media set.
 *
 * CRC is part of this, because this information is available in the
 * list of files.
 **/
class BaseFile: public PartNumberdEntity
{
  public:
    enum class FileType
    {
      RegularFile,
      LoadFile,
      BatchFile
    };

    virtual ~BaseFile( void) = default;

    /**
     * @brief Returns the name of the file.
     *
     * @return The name of the file.
     **/
    const string& getName( void) const;

    //void setName( void);

    /**
     * @brief Returns the CRC of the file.
     *
     * @return The CRC of the file.
     **/
    uint16_t getCrc( void) const;

    /**
     * @brief Sets the CRC of the file.
     *
     * @param[in] crc
     *   The CRC of the file.
     **/
    void setCrc( const uint16_t crc);

    virtual FileType getFileType( void) const = 0;

    ContainerEntityPtr getParent( void);

    ConstContainerEntityPtr getParent( void) const;

  protected:
    BaseFile( ContainerEntityPtr parent, const string &name);

    void setParent( ContainerEntityPtr parent);

  private:
    //! The parent
    WeakContainerEntityPtr parent;
    //! The file name
    const string name;
    //! The CRC
    uint16_t crc;
};

}
}

#endif
