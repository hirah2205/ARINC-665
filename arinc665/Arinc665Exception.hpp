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
 * @brief Declaration/ definition of class Arinc665::Arinc665Exception.
 **/

#ifndef ARINC665_ARINC665EXCEPTION_HPP
#define ARINC665_ARINC665EXCEPTION_HPP

#include <arinc665/Arinc665.hpp>

#include <helper/Exception.hpp>

#include <exception>
#include <string>

namespace Arinc665 {

//! Base class for ARINC 665 exceptions
class Arinc665Exception: public virtual Exception
{
  public:
    /**
     * @brief Returns the exception description.
     * @return The exception description.
     **/
    virtual const char* what() const noexcept override
    {
      return "ARINC 665 Exception";
    }
};

//! Invalid ARINC 665 file exception.
class InvalidArinc665File: public virtual Arinc665Exception
{
  public:
    /**
     * @brief Returns the exception description.
     * @return The exception description.
     **/
    virtual const char* what() const noexcept override
    {
      return "Invalid ARINC 665 Exception";
    }
};

}

#endif
