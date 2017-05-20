/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665CompilerApplication.
 **/

#include "Arinc665CompilerApplication.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/application.hpp>

#include <cstdlib>
#include <memory>

Arinc665CompilerApplication::Arinc665CompilerApplication(
  boost::application::context &context) :
  context( context),
  optionsDescription( "ARINC 665 Media Set Compiler Options")
{
  optionsDescription.add_options()
    (
      "help",
      "print this help screen"
    )
    (
      "xml-file",
      boost::program_options::value( &mediaSetXmlFile)->required(),
      "ARINC 665 media set description file"
    )
    (
      "source-directory",
      boost::program_options::value( &mediaSetSourceDirectory)->required(),
      "ARINC 665 source directory"
    )
    (
      "destination-directory",
      boost::program_options::value( &mediaSetDestinationDirectory)->required(),
      "Output directory for ARINC 665 media set"
    );
}

int Arinc665CompilerApplication::operator()()
{
  try
  {
    std::cout << "ARINC 665 Media Set Compiler" << std::endl;

    if ( !handleCommandLine())
    {
      return EXIT_FAILURE;
    }

    // load XML file
    auto xml( Arinc665::Utils::Arinc665Xml::createInstance());

    auto result( xml->loadFromXml( mediaSetXmlFile));

    auto exporter( Arinc665::Utils::Arinc665Utils::createArinc665Exporter(
      std::get< 0>( result),
      [this](const uint8_t mediumNumber)
      {
        return mediaSetDestinationDirectory / ("MEDIA_" + std::to_string( mediumNumber));
      },
      [result,this](Arinc665::Media::ConstFilePtr file, const boost::filesystem::path &destination)
      {
        auto fileIt( std::get< 1>( result).find( file));

        if (fileIt == std::get< 1>( result).end())
        {
          BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
            AdditionalInfo( "file mapping not found"));
        }

        boost::filesystem::copy(
          mediaSetSourceDirectory / fileIt->second,
          destination);
      }));

    exporter();
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::string const * info = boost::get_error_info< AdditionalInfo>( e);

    std::cerr <<
      "compiler failed: " <<
//    	typid( e).name() << " - " <<
      ((nullptr==info) ? "Unknown" : *info) <<
      std::endl;

    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "Error in compiler: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in compiler: UNKNOWN EXCEPTION" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

bool Arinc665CompilerApplication::handleCommandLine()
{
  try
  {
    std::shared_ptr< boost::application::args> args =
      context.find< boost::application::args>();

    boost::program_options::variables_map options;
    boost::program_options::store(
      boost::program_options::parse_command_line(
        args->argc(),
        args->argv(),
        optionsDescription),
      options);
    boost::program_options::notify( options);

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return false;
    }
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl << optionsDescription << std::endl;
    return false;
  }

  return true;
}
