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

#include <arinc665/media/Base.hpp>

namespace Arinc665 {
namespace Media {

/**
 * @brief The base class of all files, presented within a media set.
 **/
class BaseFile: public Base
{
  public:
    enum class FileType
    {
      RegularFile,
      LoadFile,
      BatchFile
    };

    virtual ~BaseFile( void) = default;

    //! @copydoc Base::getMediaSet
    virtual ConstMediaSetPtr getMediaSet( void) const override final;

    //! @copydoc Base::getMediaSet
    virtual MediaSetPtr getMediaSet( void) override final;

    //! @copydoc Base::getType
    virtual Type getType( void) const override final;

    /**
     * @copydoc Base::getType
     *
     * @return The name of the file.
     **/
    virtual const string& getName( void) const override;

   //void setName( void);

    //! @copydoc Base::getPartNumber
    virtual const string& getPartNumber( void) const override final;

    //! @copydoc Base::setPartNumber
    void setPartNumber( const string &partNumber);

    /**
     * @brief Returns the file type.
     *
     * @return The file type
     **/
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
    //! The part number
    string partNumber;
};

}
}

#endif
