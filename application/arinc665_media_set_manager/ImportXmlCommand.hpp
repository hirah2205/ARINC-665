/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class ImportXmlCommand.
 **/

#ifndef ARINC665_MEDIA_SET_MANAGER_IMPORTXMLCOMMAND_HPP
#define ARINC665_MEDIA_SET_MANAGER_IMPORTXMLCOMMAND_HPP

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/MediaSetManagerConfiguration.hpp>
#include <arinc665/media/Media.hpp>
#include <arinc665/files/Files.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>

//! Import XML Command
class ImportXmlCommand
{
  public:
    /**
     * @brief Constructs the Import XML Command.
     **/
    ImportXmlCommand();

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
     *   Medium.
     **/
    void createMediumHandler( Arinc665::Media::ConstMediumPtr medium );

    /**
     * @brief Creates the directory for the given directory.
     *
     * @param[in] directory
     *   Directory.
     **/
    void createDirectoryHandler( Arinc665::Media::ConstDirectoryPtr directory );

    bool checkFileExistanceHandler( Arinc665::Media::ConstFilePtr file );

    void createFileHandler( Arinc665::Media::ConstFilePtr file );

    void writeFileHandler(
      uint8_t mediumNumber,
      const std::filesystem::path &path,
      const Arinc665::Files::ConstRawFileSpan &file );

    Arinc665::Files::RawFile readFileHandler(
      uint8_t mediumNumber,
      const std::filesystem::path &path );

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

#endif
