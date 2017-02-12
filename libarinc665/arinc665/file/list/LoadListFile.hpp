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
 * @brief Declaration of class Arinc665::File::LoadListFile.
 **/

#ifndef ARINC665_LOADLISTFILE_HPP
#define ARINC665_LOADLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/LoadInfo.hpp>

#include <vector>
#include <map>

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
    using LoadInfoList = std::list< LoadInfo>;
    using LoadInfoMap = std::map< std::pair< uint8_t, string>, LoadInfo>;
    using UserDefinedData = std::vector< uint8_t>;

    LoadListFile( void);

    LoadListFile( const RawFile &file);

    //! @copydoc Arinc665File::getArincVersion
    virtual Arinc665Version getArincVersion() const override;

    //! @copydoc ListFile::getMediaSetPn
    virtual string getMediaSetPn() const override;

    //! @copydoc ListFile::setMediaSetPn
    virtual void setMediaSetPn( const string &mediaSetPn) override;

    //! @copydoc ListFile::getMediaSequenceNumber
    virtual uint8_t getMediaSequenceNumber() const override;

    //! @copydoc ListFile::setMediaSequenceNumber
    virtual void setMediaSequenceNumber( uint8_t mediaSequenceNumber) override;

    //! @copydoc ListFile::getNumberOfMediaSetMembers
    virtual uint8_t getNumberOfMediaSetMembers() const override;

    //! @copydoc ListFile::setNumberOfMediaSetMembers
    virtual void setNumberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) override;

    /**
     * @brief Returns the number of loads.
     *
     * @return The number of loads.
     **/
    size_t getNumberOfLoads() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    const LoadInfoList& getLoadInfos() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    LoadInfoList& getLoadInfos();

    /**
     * @brief Returns the loads as map for easy access.
     *
     * @return The loads as map.
     */
    LoadInfoMap getLoadInfosAsMap() const;

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    const UserDefinedData& getUserDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void setUserDefinedData( const UserDefinedData &userDefinedData);

    bool belongsToSameMediaSet( const LoadListFile &other) const;

  private:
    //! Media set part number
    string mediaSetPn;
    //! media sequence number
    uint8_t mediaSequenceNumber;
    //! number of media set members
    uint8_t numberOfMediaSetMembers;
    //! The load list
    LoadInfoList loadInfos;
    //! user defined data
    UserDefinedData userDefinedData;
};

}
}

#endif
