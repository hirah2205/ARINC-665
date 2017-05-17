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
 * @brief Declaration of class Arinc665::Utils::MediaSetManagerImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETMANAGERIMPL_HPP
#define ARINC665_UTILS_MEDIASETMANAGERIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/MediaSetManager.hpp>

namespace Arinc665 {
namespace Utils {

class MediaSetManagerImpl : public MediaSetManager
{
  public:
    /**
     *
     * @param config
     **/
    MediaSetManagerImpl(
      const boost::property_tree::ptree &config,
      const path &mediaSetBase);

    //! @copydoc MediaSetManager::getMediaSet
    virtual Media::MediaSetPtr getMediaSet( const string &partNumber) override final;

    //! @copydoc MediaSetManager::getMediaSets() const
    virtual const MediaSetList& getMediaSets() const  override final;

    //! @copydoc MediaSetManager::getMediaSets()
    virtual MediaSetList& getMediaSets() override final;

    //! @copydoc MediaSetManager::getLoads() const
    virtual Media::ConstLoads getLoads() const override final;

    //! @copydoc MediaSetManager::getFilePath() const
    virtual path getFilePath( Media::ConstBaseFilePtr file) const override final;

  private:
    using MediaPaths =
      std::map< Media::ConstMediumPtr, path>;

    MediaSetList mediaSets;
    MediaPaths mediaPaths;
};

}
}

#endif
