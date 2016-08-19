/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class LoadListFile
 **/

#ifndef ARINC665_LOADLISTFILE_HPP
#define ARINC665_LOADLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/LoadInfo.hpp>

namespace Arinc665 {
namespace File {

/**
 * @brief The load list represents each LOADS.LUM file on each media of the
 *   media set.
 **/
class LoadListFile: public ListFile
{
  public:
    //! Type definition of LoadList
    typedef std::list< LoadInfo> ListType;

    LoadListFile( void);

    LoadListFile( const RawFile &file);

    virtual Arinc665Version getArincVersion( void) const override;

    unsigned int getNumberOfLoads( void) const;

    const ListType& getLoads( void) const;

    ListType& getLoads( void);

    const std::vector< uint8_t>& getUserDefinedData( void) const;

    void setUserDefinedData( const std::vector< uint8_t> &userDefinedData);

  private:
    //! The load list
    ListType loadList;
    //! user defined data
    std::vector< uint8_t> userDefinedData;
};
}
}

#endif
