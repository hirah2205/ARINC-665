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
 * @brief Declaration of class Arinc665::Utils::Arinc665Xml.
 **/

#ifndef ARINC665_XML_ARINC665XML_HPP
#define ARINC665_XML_ARINC665XML_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/media/Media.hpp>

#include <boost/filesystem.hpp>

namespace Arinc665 {
namespace Utils {

/**
 * @brief Handles the representation of Media Sets as XML file.
 **/
class Arinc665Xml
{
  public:
    using MediaSetPtr = Media::MediaSetPtr;
    using ConstMediaSetPtr = Media::ConstMediaSetPtr;

    using path = boost::filesystem::path;

    /**
     * @brief Creates an ARINC 665 XML handler instance.
     *
     * @return The ARINC 665 XML handler instance.
     **/
    static Arinc665XmlPtr createInstance();

    /**
     * @brief Loads the Media Set information from the given XML file.
     *
     * @param[in] xmlFile
     *   The ARINC 665 XML file.
     *
     * @return The loaded Media Set information.
     **/
    virtual MediaSetPtr loadFromXml( const path &xmlFile) = 0;

    /**
     * @brief Saves the given Media Set information to the given XML file.
     *
     * @param[in] mediaSet
     *   The Media Set information.
     * @param[in] xmlFile
     *   The ARINC 665 XML file.
     */
    virtual void saveToXml( ConstMediaSetPtr mediaSet, const path &xmlFile) = 0;

    //! Default descructor
    virtual ~Arinc665Xml() = default;

  protected:
    //! Default constructor
    Arinc665Xml() = default;
};

}
}

#endif
