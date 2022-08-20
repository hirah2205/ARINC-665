/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::File.
 **/

#ifndef ARINC665_MEDIA_FILE_HPP
#define ARINC665_MEDIA_FILE_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/Base.hpp>

#include <arinc645/Arinc645.hpp>

#include <filesystem>
#include <optional>

namespace Arinc665::Media {

/**
 * @brief %Media Set %File.
 *
 * This class is used as common base for:
 *  - regular files (File),
 *  - load header files (Load), and
 *  - batch files (Batch).
 **/
class ARINC665_EXPORT File : public Base
{
  public:
    //! File Type
    enum class FileType
    {
      RegularFile,
      LoadFile,
      BatchFile
    };

    //! Destructor
    ~File() override = default;

    //! @copydoc Base::mediaSet
    [[nodiscard]] ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::mediaSet
    MediaSetPtr mediaSet() final;

    //! @copydoc Base::type
    [[nodiscard]] Type type() const final;

   //void setName( void);

    /**
     * @brief Returns the Name of the File.
     *
     * @return Name of the file.
     **/
    [[nodiscard]] std::string_view name() const;

    /**
     * @brief Returns the File Type.
     *
     * @return File type
     **/
    [[nodiscard]] virtual FileType fileType() const = 0;

    /**
     * @brief Returns the Parent Container.
     *
     * @return The parent container element
     **/
    [[nodiscard]] ConstContainerEntityPtr parent() const;

    //! @copydoc parent() const
    [[nodiscard]] ContainerEntityPtr parent();

    /**
     * @brief Returns the Medium where this file is Located.
     *
     * @return Medium where this file is located.
     **/
    [[nodiscard]] ConstMediumPtr medium() const;

    //! @copydoc medium() const
    [[nodiscard]] MediumPtr medium();

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


    /**
     * @name File Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of Media Set Files List Check Value.
     * This Value override a common value set by
     * ref MediaSet::filesCheckValueType().
     *
     * @{
     **/

    /**
     * @brief Returns the effective File Check Value Type.
     *
     * If value is not set the effetive Media Set File Check Value Type is used.
     *
     * @return Effective File Check Value Type.
     *
     * @sa MediaSet::effectiveFilesCheckValueType.
     **/
    Arinc645::CheckValueType effectiveCheckValueType() const;

    /**
     * @brief Returns the File Check Value Type.
     *
     * @param[in] effective
     *   If set to true the effective value is returned.
     *
     * @return File Check Value Type
     **/
    std::optional< Arinc645::CheckValueType > checkValueType() const;

    /**
     * @brief Updates the File Check Value Type
     *
     * @param[in] type
     *   New File Check Value Type.
     **/
    void checkValueType( std::optional< Arinc645::CheckValueType > type );

    /** @} **/

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
    File( const ContainerEntityPtr& parent, std::string_view name );

    //! @copydoc File::File(const ContainerEntityPtr&,std::string_view)
    File( const ContainerEntityPtr& parent, std::string &&name );

    /**
     * @brief Sets the parent element.
     *
     * @param[in] parent
     *   Parent element.
     *
     * @throw Arinc665Exception
     *   If parent is invalid
     **/
    void parent( const ContainerEntityPtr& parent );

  private:
    //! Parent
    ContainerEntityPtr::weak_type parentV;
    //! File Name
    const std::string nameV;
    //! Check Value Type
    std::optional< Arinc645::CheckValueType > checkValueTypeV;
};

}

#endif
