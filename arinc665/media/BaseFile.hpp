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

#include <boost/filesystem/path.hpp>

namespace Arinc665 {
namespace Media {

/**
 * @brief The base class of all files, presented within a media set.
 **/
class BaseFile: public Base
{
  public:
    //! Path type
    using fpath = boost::filesystem::path;

    //! File type
    enum class FileType
    {
      RegularFile,
      LoadFile,
      BatchFile
    };

    //! Default destructor
    virtual ~BaseFile() = default;

    //! @copydoc Base::getMediaSet
    ConstMediaSetPtr mediaSet() const final;

    //! @copydoc Base::getMediaSet
    MediaSetPtr mediaSet() final;

    //! @copydoc Base::getType
    Type type() const final;

    /**
     * @copydoc Base::getType
     *
     * @return The name of the file.
     **/
    const string& name() const final;

   //void setName( void);

    //! @copydoc Base::getPartNumber
    string partNumber() const final;

    /**
     * @brief Updates the part number
     *
     * @param[in] partNumber
     *   New Part number
     **/
    void partNumber( const string &partNumber);

    /**
     * @brief Returns the file type.
     *
     * @return The file type
     **/
    virtual FileType fileType() const = 0;

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
    ConstContainerEntityPtr parent() const;

    /**
     * @brief Returns the medium where this file is located.
     *
     * @return The medium where this file is located.
     **/
    ConstMediumPtr medium() const;

    //! @copydoc getMedium() const
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
    fpath path() const;

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
    BaseFile( ContainerEntityPtr parent, const string &name);

    //! @copydoc BaseFile::BaseFile(ContainerEntityPtr,const string&)
    BaseFile( ContainerEntityPtr parent, string &&name);

    /**
     * @brief Sets the parent element.
     *
     * @param[in] parent
     *   The parent element.
     *
     * @throw Arinc665Exception
     *   If parent is invalid
     **/
    void parent( ContainerEntityPtr parent);

  private:
    //! The parent
    WeakContainerEntityPtr parentValue;
    //! The file name
    const string nameValue;
    //! The part number
    string partNumberValue;
};

}
}

#endif
