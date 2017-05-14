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
 * @brief Declaration of class Arinc665::Media::Directory.
 **/

#ifndef ARINC665_MEDIA_DIRECTORY_HPP
#define ARINC665_MEDIA_DIRECTORY_HPP

#include <arinc665/media/ContainerEntity.hpp>

namespace Arinc665 {
namespace Media {

/**
 * @brief Represents a directory within a medium.
 **/
class Directory : public ContainerEntity
{
  public:
    Directory( ContainerEntityPtr parent, const string &name);

    //! @copydoc ContainerEntity::getMediaSet
    virtual ConstMediaSetPtr getMediaSet() const override final;

    //! @copydoc ContainerEntity::getMediaSet
    virtual MediaSetPtr getMediaSet() override final;

    //! @copydoc ContainerEntity::getType
    virtual Type getType() const override final;

    //! @copydoc ContainerEntity::getName
    virtual const string& getName() const override final;

    // void rename( const string& name);

    //! @copydoc ContainerEntity::getPartNumber
    virtual const string& getPartNumber() const override final;

    //! @copydoc ContainerEntity::getPath() const
    virtual path getPath() const override final;

  private:
    //! Directory name
    const string name;
};

}
}

#endif
