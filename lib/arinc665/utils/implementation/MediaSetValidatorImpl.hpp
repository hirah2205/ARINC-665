/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetValidatorImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETVALIDATORIMPL_HPP
#define ARINC665_UTILS_MEDIASETVALIDATORIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/MediaSetValidator.hpp>

namespace Arinc665::Utils {

//! ARINC 665 Media Set Validator.
class MediaSetValidatorImpl final : public MediaSetValidator
{
  public:
    /**
     * @brief Created the ARINC 665 Media Set Validator.
     **/
    MediaSetValidatorImpl();

    //! @copydoc MediaSetValidator::readFileHandler()
    [[nodiscard]] MediaSetValidator& readFileHandler(
      ReadFileHandler readFileHandler ) override;

    //! @copydoc MediaSetValidator::informationHandler()
    [[nodiscard]] MediaSetValidator& informationHandler(
      ValidatorInformationHandler informationHandler ) override;

    /**
     * @brief Executes the ARINC 665 Media Set Validator.
     *
     * @return Validation Result.
     **/
    [[nodiscard]] bool operator()() override;

  private:
    //! Read File Handler
    MediaSetValidator::ReadFileHandler readFileHandlerV;
    //! Information Handler
    MediaSetValidator::ValidatorInformationHandler informationHandlerV;
};

}

#endif
