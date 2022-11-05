/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Commands::ImportMediaSetXmlCommand.
 **/

#ifndef ARINC665_COMMANDS_IMPORTMEDIASETXMLCOMMAND_HPP
#define ARINC665_COMMANDS_IMPORTMEDIASETXMLCOMMAND_HPP

#include <arinc665_commands/Arinc665Commands.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/MediaSetManagerConfiguration.hpp>
#include <arinc665/media/Media.hpp>
#include <arinc665/files/Files.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>

namespace Arinc665Commands {

/**
 * @brief Import Media Set XML into Media Set Manager %Command
 *
 * Creates a Media Set based on the given XML description and imports them to
 * the media set manager.
 **/
class ARINC665_COMMANDS_EXPORT ImportMediaSetXmlCommand
{
  public:
    /**
     * @brief Constructs the Import XML Command.
     **/
    ImportMediaSetXmlCommand();

    /**
     * @brief Executes the Operation.
     *
     * @param[in] parameters
     *   Parameters supplied by User.
     **/
    void execute( const Commands::Parameters &parameters );

    //! Prints help screen.
    void help();

  private:
    /**
     * @brief Creates the directory for the given medium.
     *
     * @param[in] medium
     *   Medium to Create.
     **/
    void createMediumHandler( const Arinc665::Media::ConstMediumPtr &medium );

    /**
     * @brief Creates the directory for the given directory.
     *
     * @param[in] directory
     *   Directory to Create.
     **/
    void createDirectoryHandler(
      const Arinc665::Media::ConstDirectoryPtr &directory );

    /**
     * @brief Check File Existence Handler.
     *
     * @param[in] file
     *   File to Check
     *
     * @return If file exists
     **/
    bool checkFileExistenceHandler( const Arinc665::Media::ConstFilePtr &file );

    /**
     * @brief Create File Handler.
     *
     * @param[in] file
     *   File to Create
     **/
    void createFileHandler( const Arinc665::Media::ConstFilePtr &file );

    /**
     * @brief Write File Handler
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   File Path
     * @param[in] file
     *   File Content
     **/
    void writeFileHandler(
      uint8_t mediumNumber,
      const std::filesystem::path &path,
      const Arinc665::Files::ConstRawFileSpan &file );

    /**
     * @brief Read File Handler
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   File Path
     *
     * @return File Content
     **/
    Arinc665::Files::RawFile
    readFileHandler( uint8_t mediumNumber, const std::filesystem::path &path );

    //! Program Options Description
    boost::program_options::options_description optionsDescription;
    //! Media Set Manager Directory
    std::filesystem::path mediaSetManagerDirectory;
    //! Media Set XML File
    std::filesystem::path mediaSetXmlFile;
    //! Media Set Source Directory
    std::filesystem::path mediaSetSourceDirectory;
    //! Create Batch File Policy
    Arinc665::Utils::FileCreationPolicy createBatchFiles{
      Arinc665::Utils::FileCreationPolicy::Invalid };
    //! Create Load Header File Policy
    Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles{
      Arinc665::Utils::FileCreationPolicy::Invalid };
    //! ARINC 665 Version
    Arinc665::SupportedArinc665Version version{
      Arinc665::SupportedArinc665Version::Invalid };
    //! Media Set Output Paths
    Arinc665::Utils::MediaSetManagerConfiguration::MediaSetPaths mediaSetPaths;
    //! Load XML Results
    Arinc665::Utils::Arinc665Xml::LoadXmlResult loadXmlResult;
};

}

#endif
