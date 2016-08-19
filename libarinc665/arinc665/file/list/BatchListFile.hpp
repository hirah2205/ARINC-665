/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class BatchListFile
 **/

#ifndef ARINC665_BATCHLISTFILE_HPP
#define ARINC665_BATCHLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/BatchInfo.hpp>

namespace Arinc665 {
namespace File {

/**
 * @brief The load list represents each BATCHES.LUM file on each media of
 *   the media set.
 **/
class BatchListFile: public ListFile
{
  public:
    typedef std::list< BatchInfo> BatchList;

    BatchListFile( void);

    BatchListFile( const RawFile &file);

    virtual Arinc665Version getArincVersion( void) const override;

    unsigned int getNumberOfBatches( void) const;

    const BatchList& getBatches( void) const;

    BatchList& getBatches( void);

  private:
    BatchList batchList;
};
}
}

#endif
