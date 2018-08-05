/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Utils::MediaSetValidatorImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETVALIDATORIMPL_HPP
#define ARINC665_UTILS_MEDIASETVALIDATORIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Utils.hpp>

namespace Arinc665::Utils {

//! ARINC 665 Media Set Validator.
class MediaSetValidatorImpl
{
  public:
    /**
     * @brief Created the ARINC 665 Media Set Validator.
     *
     * @param[in] readFileHandler
     * @param[in] informationHandler
     **/
    MediaSetValidatorImpl(
      Arinc665Utils::ReadFileHandler readFileHandler,
      Arinc665Utils::ValidatorInformationHandler informationHandler);

    /**
     * @brief Executes the ARINC 665 Media Set Validator.
     *
     * @return Validation Result.
     **/
    bool operator()();

  private:
    //! Read File Handler
    Arinc665Utils::ReadFileHandler readFileHandler;
    //! Information Handler
    Arinc665Utils::ValidatorInformationHandler informationHandler;
};

}

#endif
