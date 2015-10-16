/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#ifndef ARINC665_IMPORTER_ARINC665IMPORTER_HPP
#define ARINC665_IMPORTER_ARINC665IMPORTER_HPP

#include <arinc665/Arinc665.hpp>

#include <arinc665/file/Arinc665File.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <boost/filesystem.hpp>

#include <functional>

namespace Arinc665
{
	namespace Importer
	{
		using Arinc665::File::RawFile;
		using Arinc665::File::Arinc665File;
		using Arinc665::Media::MediaSetPtr;

		using boost::filesystem::path;

		class Arinc665Importer
		{
			public:
				typedef path (GetMedium)( unsigned int mediumNumber);

				using GetMediumHandler = std::function< GetMedium>;

				MediaSetPtr getMediaSet( void);

				void import( GetMediumHandler getMediumHandler);

			protected:

				void addMedium( const unsigned int mediaIndex, const path &mediumPath);

				RawFile loadFile( const path &filePath);

			private:
				//! The media set.
				MediaSetPtr mediaSet;
		};
	}
}

#endif
