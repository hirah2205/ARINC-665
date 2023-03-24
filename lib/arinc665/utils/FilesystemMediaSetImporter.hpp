/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FilesystemMediaSetImporter.
 **/

#ifndef ARINC665_UTILS_FILESYSTEMMEDIASETIMPORTER_HPP
#define ARINC665_UTILS_FILESYSTEMMEDIASETIMPORTER_HPP

#include <arinc665/utils/Utils.hpp>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Media Set Importer using local Filesystem.
 *
 * This importer expects that each medium is contained as directory on the local
 * filesystem.
 * Normal filesystem access routines are used to access the media and files.
 *
 * @sa @ref MediaSetImporter
 **/
class ARINC665_EXPORT FilesystemMediaSetImporter
{
  public:
    /**
     * @brief Creates the ARINC 665 %Media Set Importer Instance.
     *
     * @return ARINC 665 %Media Set Importer Instance
     **/
    static FilesystemMediaSetImporterPtr create();

    //! Destructor
    virtual ~FilesystemMediaSetImporter() = default;

    /**
     * @brief Sets the Check File Integrity Flag.
     *
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     *
     * @return @p *this for chaining.
     **/
    virtual FilesystemMediaSetImporter& checkFileIntegrity(
      bool checkFileIntegrity ) noexcept = 0;

    /**
     * @brief Sets the Media Paths
     *
     * @param[in] mediaPaths
     *   Media Paths
     *
     * @return @p *this for chaining.
     **/
    virtual FilesystemMediaSetImporter& mediaPaths( MediaPaths mediaPaths ) = 0;

    /**
     * @brief Executes the ARINC 665 %Media Set Importer.
     *
     * All parameters must have been set previously.
     *
     * @return Imported %Media Set
     *
     * @throw Arinc665Exception
     *   When the media set cannot be imported.
     **/
    virtual MediaSetImportResult operator()() = 0;
};

}

#endif
