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

namespace Arinc665::Media {

/**
 * @brief Represents a directory within a medium.
 **/
class Directory : public ContainerEntity
{
  public:
    /**
     * @brief Initialises the instance with the given data.
     *
     * @param[in] parent
     *   Parent of this directory.
     * @param[in] name
     *   Name of the directory.
     **/
    Directory( ContainerEntityPtr parent, const std::string &name);

    //! @copydoc ContainerEntity::mediaSet
    ConstMediaSetPtr mediaSet() const final;

    //! @copydoc ContainerEntity::mediaSet
    MediaSetPtr mediaSet() final;

    //! @copydoc ContainerEntity::type
    Type type() const final;

    /**
     * @brief Returns the name of the directory.
     *
     * @return The name of the directory.
     **/
    const std::string& name() const;


    // void rename( const string& name);

    //! @copydoc ContainerEntity::partNumber
    std::string partNumber() const final;

    //! @copydoc ContainerEntity::path() const
    virtual fpath path() const final;

  private:
    //! Directory name
    const std::string nameV;
};

}

#endif
