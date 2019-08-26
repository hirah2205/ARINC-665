/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::BaseFile.
 **/

#ifndef ARINC665_MEDIA_BASEFILE_HPP
#define ARINC665_MEDIA_BASEFILE_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/Base.hpp>

#include <filesystem>

namespace Arinc665::Media {

/**
 * @brief The base class of all files, presented within a media set.
 **/
class BaseFile: public Base
{
  public:
    //! File type
    enum class FileType
    {
      RegularFile,
      LoadFile,
      BatchFile
    };

    //! Destructor
    ~BaseFile() override = default;

    //! @copydoc Base::mediaSet
    [[nodiscard]] ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::mediaSet
    MediaSetPtr mediaSet() final;

    //! @copydoc Base::type
    [[nodiscard]] Type type() const final;

   //void setName( void);

    //! @copydoc Base::partNumber
    [[nodiscard]] std::string_view partNumber() const final;

    /**
     * @brief Updates the part number
     *
     * @param[in] partNumber
     *   New Part number
     **/
    void partNumber( std::string_view partNumber);

    /**
     * @brief Returns the name of the file.
     *
     * @return The name of the file.
     **/
    std::string_view name() const;

    /**
     * @brief Returns the file type.
     *
     * @return The file type
     **/
    [[nodiscard]] virtual FileType fileType() const = 0;

    /**
     * @brief Returns the container element.
     *
     * @return The parent container element
     **/
    ContainerEntityPtr parent();

    /**
     * @brief Returns the container element.
     *
     * @return The parent container element
     **/
    [[nodiscard]] ConstContainerEntityPtr parent() const;

    /**
     * @brief Returns the medium where this file is located.
     *
     * @return The medium where this file is located.
     **/
    [[nodiscard]] ConstMediumPtr medium() const;

    //! @copydoc medium() const
    MediumPtr medium();

    /**
     * @brief Returns the file path up to the medium root.
     *
     * This also contains the filename.
     * The path is an absolute path originate in the medium root directory.
     *
     * @return The path up to the medium root.
     * @retval {}
     *   If parent is not available. (Should never happen)
     **/
    [[nodiscard]] std::filesystem::path path() const;

  protected:
    /**
     * @brief Initialises the instance with the given data.
     *
     * @param[in] parent
     *   Parent container.
     * @param[in] name
     *   Name of the element.
     *
     * @throw Arinc665Exception
     *   If parent is invalid
     **/
    BaseFile( const ContainerEntityPtr& parent, std::string_view name);

    //! @copydoc BaseFile::BaseFile(ContainerEntityPtr,std::string_view)
    BaseFile( const ContainerEntityPtr& parent, std::string &&name);

    /**
     * @brief Sets the parent element.
     *
     * @param[in] parent
     *   The parent element.
     *
     * @throw Arinc665Exception
     *   If parent is invalid
     **/
    void parent( const ContainerEntityPtr& parent);

  private:
    //! Parent
    WeakContainerEntityPtr parentValue;
    //! File name
    const std::string nameValue;
    //! Part number
    std::string partNumberValue;
};

}

#endif
