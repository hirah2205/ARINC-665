/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class
 *   Arinc665Commands::MediaSetManager::ImportMediaSetXmlCommand.
 **/

#ifndef ARINC665_COMMANDS_MEDIA_SET_MANAGER_IMPORTMEDIASETXMLCOMMAND_HPP
#define ARINC665_COMMANDS_MEDIA_SET_MANAGER_IMPORTMEDIASETXMLCOMMAND_HPP

#include <arinc665_commands/media_set_manager/MediaSetManager.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/MediaSetManagerConfiguration.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>

namespace Arinc665Commands::MediaSetManager {

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
      Arinc665::Utils::FileCreationPolicy::None };
    //! Create Load Header File Policy
    Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    //! ARINC 665 Version
    Arinc665::SupportedArinc665Version version{
      Arinc665::SupportedArinc665Version::Supplement2 };
};

}

#endif
