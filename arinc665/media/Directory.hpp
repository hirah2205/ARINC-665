/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::Directory.
 **/

#ifndef ARINC665_MEDIA_DIRECTORY_HPP
#define ARINC665_MEDIA_DIRECTORY_HPP

#include <arinc665/media/ContainerEntity.hpp>

#include <string_view>

namespace Arinc665::Media {

/**
 * @brief %Directory within a %Medium.
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
    Directory( ContainerEntityPtr parent, std::string_view name );

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
    std::string_view name() const;


    // void rename( const string& name);

    //! @copydoc ContainerEntity::partNumber
    std::string_view partNumber() const final;

    //! @copydoc ContainerEntity::path() const
    std::filesystem::path path() const final;

    //! @copydoc ContainerEntity::parent() const
    ConstContainerEntityPtr parent() const final;

    ///! @copydoc ContainerEntity::parent()
    ContainerEntityPtr parent() final;

    //! @copydoc ContainerEntity::medium() const
    ConstMediumPtr medium() const final;

    //! @copydoc ContainerEntity::medium()
    MediumPtr medium() final;

  protected:
    /**
     * @brief Updates the parent.
     *
     * @param[in] parent
     *   New parent.
     **/
    void parent( const ContainerEntityPtr& parent);

  private:
    //! Directory Name
    const std::string nameV;
    //! Parent
    ContainerEntityPtr::weak_type parentV;
};

}

#endif
