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

#include <arinc665/MediumNumber.hpp>

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
 *
 * The parent stored within this class is held as weak reference.
 **/
class ARINC665_EXPORT File : public Base
{
  public:
    //! Destructor
    ~File() override = default;

    //! @copydoc Base::mediaSet
    [[nodiscard]] ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::mediaSet
    [[nodiscard]] MediaSetPtr mediaSet() final;

    //! @copydoc Base::type
    [[nodiscard]] Type type() const final;

    //! @copydoc Base::parent() const
    [[nodiscard]] ConstContainerEntityPtr parent() const final;

    //! @copydoc Base::parent() const
    [[nodiscard]] ContainerEntityPtr parent() final;

    /**
     * @copydoc Base::path() const
     *
     * This also contains the filename.
     * The path is an absolute path originate in the medium root directory.
     *
     * @return The path up to the medium root.
     * @retval {}
     *   If parent is not available. (Should never happen)
     **/
    [[nodiscard]] std::filesystem::path path() const final;

    /**
     * @brief Returns the Name of the File.
     *
     * @return Name of the file.
     **/
    [[nodiscard]] std::string_view name() const;

    /**
     * @brief Renames the file.
     *
     * Checks if a directory or file exist with this name.
     *
     * @param[in] name
     *   New filename.
     **/
    void rename( std::string name );

    /**
     * @name Medium Number
     *
     * Each file is placed on a medium.
     * @{
     **/

    /**
     * @brief Returns the effective medium number.
     *
     * If the medium number is not defined, the parent effective medium number
     * is returned.
     *
     * @return effecive medium number.
     */
    [[nodiscard]] MediumNumber effectiveMediumNumber() const;

    /**
     * @brief Returns the Medium Number, where the file is located on.
     *
     * @return Medium Number, where the file is located on.
     **/
    [[nodiscard]] OptionalMediumNumber mediumNumber() const;

    /**
     * @brief Updates the Medium Number of the file,
     *
     * @param[in] mediumNumber
     *   New Medium Number.
     *   If empty it is not defined on this level
     **/
    void mediumNumber( const OptionalMediumNumber &mediumNumber );

    /** @} **/

    /**
     * @brief Returns the File Type.
     *
     * @return File type
     **/
    [[nodiscard]] virtual FileType fileType() const = 0;

    /**
     * @name File Check Value Type
     *
     * This information is used to determine the Check Value Type used for
     * generation of Media Set Files List Check Value.
     * This Value override a common value set by
     * @ref MediaSet::filesCheckValueType().
     *
     * @{
     **/

    /**
     * @brief Returns the effective File Check Value Type.
     *
     * If value is not set the effective Media Set File Check Value Type is
     * used.
     *
     * @return Effective File Check Value Type.
     *
     * @sa MediaSet::effectiveFilesCheckValueType.
     **/
    [[nodiscard]] Arinc645::CheckValueType effectiveCheckValueType() const;

    /**
     * @brief Returns the File Check Value Type.
     *
     * @return File Check Value Type
     **/
    [[nodiscard]] std::optional< Arinc645::CheckValueType >
    checkValueType() const;

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
     * @param[in] mediumNumber
     *   Medium Number, where the file is located on.
     *
     * @throw Arinc665Exception
     *   If parent is invalid
     **/
    File(
      const ContainerEntityPtr &parent,
      std::string name,
      const OptionalMediumNumber &mediumNumber );

    /**
     * @brief Sets the parent element.
     *
     * @param[in] parent
     *   Parent element.
     *
     * @throw Arinc665Exception
     *   If parent is invalid
     **/
    void parent( const ContainerEntityPtr &parent );

  private:
    //! Parent Container
    ContainerEntityPtr::weak_type parentV;
    //! Filename
    std::string nameV;
    //! Medium Number
    OptionalMediumNumber mediumNumberV;
    //! Check Value Type
    std::optional< Arinc645::CheckValueType > checkValueTypeV;
};

}

#endif
