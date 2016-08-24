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
 *
 **/
class Directory : public ContainerEntity
{
  public:
    Directory( ContainerEntityPtr parent, const string &name);

    virtual ConstMediaSetPtr getMediaSet( void) const override;

    virtual MediaSetPtr getMediaSet( void) override;

    virtual Type getType( void) const override final;

    const string& getName( void) const;

    // void rename( const string& name);
  private:
    //! Directory name
    const string name;
};

}
}

#endif
