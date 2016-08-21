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
 * @brief TODO Enter Brief description
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Importer.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <iostream>

using Arinc665::Media::MediaSetPtr;
using Arinc665::Media::Load;
using Arinc665::Media::LoadPtr;
using Arinc665::Media::FilePtr;

using boost::filesystem::path;

static MediaSetPtr loadMediaSet( const boost::filesystem::path &mediaSetDirectory);

static void printMediaSet( MediaSetPtr &mediaSet);

void list_luh( const boost::filesystem::path &luhFile)
{
	try
	{
		std::cout << "File size is: " << std::dec << boost::filesystem::file_size( luhFile) << std::endl;

		std::vector< uint8_t> data( boost::filesystem::file_size( luhFile));

		std::ifstream file(
			luhFile.string().c_str(),
			std::ifstream::binary | std::ifstream::in);

		if (!file.is_open())
		{
			std::cout << "Error opening file: " << luhFile.string() << std::endl;
			return;
		}

		file.read( (char*)&data.at(0), data.size());

		LoadHeaderFile load( data);

		std::cout << "part number: "<< load.getPartNumber() << std::endl;

		for (
			std::list< std::string>::const_iterator it = load.getTargetHardwareIdList().begin();
			it != load.getTargetHardwareIdList().end();
			++it)
		{
			std::cout << "target hw id: " << *it << std::endl;
		}

		for (
			std::list< LoadFileInfo>::const_iterator it = load.getDataFileList().begin();
			it != load.getDataFileList().end();
			++it)
		{
			std::cout << "data file name: " << it->getName() << std::endl;
			std::cout << "data file PN: "   << it->getPartNumber() << std::endl;
			std::cout << "data file size: " << std::dec << it->getLength() << std::endl;
			std::cout << "data file crc: "  << std::hex << it->getCrc() << std::endl << std::endl;
		}

		for (
			std::list< LoadFileInfo>::const_iterator it = load.getSupportFileList().begin();
			it != load.getSupportFileList().end();
			++it)
		{
			std::cout << "support file name: " << it->getName() << std::endl;
			std::cout << "support file PN: "   << it->getPartNumber() << std::endl;
			std::cout << "support file size: " << std::dec << it->getLength() << std::endl;
			std::cout << "support file crc: "  << std::hex << it->getCrc() << std::endl << std::endl;
		}

		std::cout << "load crc " << std::hex << load.getLoadCrc() << std::endl;

		std::cout << "header file crc " << std::hex << load.getCrc() << std::endl << std::endl;
	}
	catch (boost::exception &e)
	{
		std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
	}
	catch ( std::exception &e)
	{
		std::cout << "std exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exception occurred" << std::endl;
	}
}

void list_loads_lum( const boost::filesystem::path &loadsLum)
{
	try
	{
		std::cout << "File size is: " << std::dec << boost::filesystem::file_size( loadsLum) << std::endl;

		std::vector< uint8_t> data( boost::filesystem::file_size( loadsLum));

		std::ifstream file(
			loadsLum.string().c_str(),
			std::ifstream::binary | std::ifstream::in);

		if (!file.is_open())
		{
			std::cout << "Error opening file: " << loadsLum.string() << std::endl;
			return;
		}

		file.read( (char*)&data.at(0), data.size());

		LoadListFile loadList( data);

		std::cout << "media set pn: " << loadList.getMediaSetPn() << std::endl;

		std::cout << "media seq no: " << std::dec << (int)loadList.getMediaSequenceNumber() << std::endl;

		std::cout << "no of media set members: " << (int)loadList.getNumberOfMediaSetMembers() << std::endl;

		for (
			std::list< LoadInfo>::const_iterator it = loadList.getLoads().begin();
			it != loadList.getLoads().end();
			++it)
		{
			std::cout << "load load pn: "                << it->getPartNumber() << std::endl;
			std::cout << "load header file name: "       << it->getHeaderFilename() << std::endl;
			std::cout << "load member sequence number: " << std::dec << it->getMemberSequenceNumber() << std::endl << std::endl;

			for (
				std::list< std::string>::const_iterator thwIt = it->getTargetHardwareIdList().begin();
				thwIt != it->getTargetHardwareIdList().end();
				++thwIt)
			{
				std::cout << "target hardware id: " << *thwIt << std::endl << std::endl;
			}
		}

		std::cout << "load list crc: " << std::hex << (int)loadList.getCrc() << std::endl;
	}
	catch (boost::exception &e)
	{
		std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
	}
	catch ( std::exception &e)
	{
		std::cout << "std exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exception occurred" << std::endl;
	}
}

void list_files_lum( const boost::filesystem::path &filesLum)
{
	try
	{
		std::cout << "File size is: " << std::dec << boost::filesystem::file_size( filesLum) << std::endl;

		std::vector< uint8_t> data( boost::filesystem::file_size( filesLum));

		std::ifstream file(
			filesLum.string().c_str(),
			std::ifstream::binary | std::ifstream::in);

		if (!file.is_open())
		{
			std::cout << "Error opening file: " << filesLum.string() << std::endl;
			return;
		}

		file.read( (char*)&data.at(0), data.size());

		FileListFile fileList( data);

		std::cout << "media set pn: " << fileList.getMediaSetPn() << std::endl;

		std::cout << "media seq no: " << (int)fileList.getMediaSequenceNumber() << std::endl;

		std::cout << "no of media set members: " << std::dec << (int)fileList.getNumberOfMediaSetMembers() << std::endl;

		for (
			std::list< FileInfo>::const_iterator it = fileList.getFiles().begin();
			it != fileList.getFiles().end();
			++it)
		{
			std::cout << "file file name: " << it->getFilename() << std::endl;
			std::cout << "file path name: " << it->getPathName() << std::endl;
			std::cout << "file member sequence number: " << std::dec << it->getMemberSequenceNumber() << std::endl;
			std::cout << "file crc: " << std::hex << it->getCrc() << std::endl << std::endl;
		}

		std::cout << "file list crc: " << std::hex << (int)fileList.getCrc() << std::endl;

	}
	catch (boost::exception &e)
	{
		std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
	}
	catch ( std::exception &e)
	{
		std::cout << "std exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exception occurred" << std::endl;
	}
}

void list_file( const boost::filesystem::path &filename)
{
}

static void list_files( const boost::filesystem::path &loadDir)
{
	std::cout << "directory: " << loadDir << std::endl;

	for (boost::filesystem::directory_iterator itr( loadDir);
		itr!=boost::filesystem::directory_iterator();
		++itr)
	{
		std::cout << itr->path().filename() << std::endl;

		if (is_regular_file(itr->status()))
		{
			std::cout << " - ";

			switch (FileFactory::getFileType( itr->path().filename()))
			{
				case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_FILE:
					std::cout << "ARINC 665 BATCH file";
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_UPLOAD_HEADER:
					std::cout << "ARINC 665 LOAD UPLOAD HEADER file";
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_LIST:
					std::cout << "ARINC 665 LOAD LIST file";
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_LIST:
					std::cout << "ARINC 665 BATCH LIST file";
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_FILE_LIST:
					std::cout << "ARINC 665 FILE LIST file";
					break;

				default:
					std::cout << "No special ARINC 665 file";
					break;
			}

			std::cout << std::endl;

			switch (FileFactory::getFileType( itr->path().filename()))
			{
				case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_FILE:
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_UPLOAD_HEADER:
					list_file( itr->path());
					list_luh( itr->path());
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_LIST:
					list_loads_lum( itr->path());
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_LIST:
					break;

				case Arinc665::FileType::ARINC_665_FILE_TYPE_FILE_LIST:
					list_files_lum( itr->path());
					break;

				default:
					list_file( itr->path());
					break;
			}
		}
	}
}

int main( int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "enter load directory" << std::endl;

		return EXIT_FAILURE;
	}

	try
	{
		boost::filesystem::path loadDir( argv[1]);

		std::cout << "List files" << std::endl;
		list_files( loadDir);

		std::cout  << std::endl << "Load Media Set " << std::endl;
		MediaSetPtr mediaSet( loadMediaSet( loadDir));

		std::cout  << std::endl << "Print Media Set " << std::endl;
		printMediaSet( mediaSet);
	}
	catch (boost::exception &e)
	{
		std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
		return EXIT_FAILURE;
	}
	catch ( std::exception &e)
	{
		std::cout << "std exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cout << "unknown exception occurred" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


static MediaSetPtr loadMediaSet( const path &mediaSetDirectory)
{
  Arinc665::Utils::Arinc665ImporterPtr importer( Arinc665::Utils::Arinc665Importer::create());

  importer->import(
    [&mediaSetDirectory]( uint8_t)
    {
      return mediaSetDirectory;
    });

	MediaSetPtr mediaSet( importer->getMediaSet());

	std::cout << "media set pn: " << mediaSet->getPartNumber() << std::endl;

	std::cout << "no of media set members: " << std::dec << (int)mediaSet->getNumberOfMedia() << std::endl;

	return mediaSet;
}

static void printMediaSet( MediaSetPtr &mediaSet)
{
	std::cout << "Media Set " << mediaSet->getPartNumber() << std::endl;

	// iterate over files
	for ( auto const &file : mediaSet->getFiles())
	{
		std::cout <<
			"File " << file->getName() << "\n";
	}

	// iterate over loads
	for ( auto const &load : mediaSet->getLoads())
	{
		std::cout <<
			"Load " << load->getName() << " " << load->getPartNumber() << std::endl;


		std::cout << "Compatible THW IDs" << std::endl;
		// iterate over THW ID list
		for ( auto const & thwId : load->getTargetHardwareIdList())
		{
			std::cout << " * " << thwId << std::endl;
		}

		std::cout << "Data Files" << std::endl;
		// iterate over Data Files
		for ( const auto & dataFile : load->getDataFiles())
		{
			std::cout << " * " <<
				dataFile.lock()->getName() << " " <<
				dataFile.lock()->getPartNumber() << std::endl;
		}

		std::cout << "Support Files" << std::endl;
		// iterate over Data Files
		for ( const auto & supportFile : load->getSupportFiles())
		{
			std::cout << " * " <<
				supportFile.lock()->getName() << " " <<
				supportFile.lock()->getPartNumber() << std::endl;
		}
	}
}
