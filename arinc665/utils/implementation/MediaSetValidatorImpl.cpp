/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Utils::MediaSetValidatorImpl.
 **/

#include "MediaSetValidatorImpl.hpp"

namespace Arinc665::Utils {

MediaSetValidatorImpl::MediaSetValidatorImpl(
  Arinc665Utils::ReadFileHandler readFileHandler,
  Arinc665Utils::ValidatorInformationHandler informationHandler):
  readFileHandler( readFileHandler),
  informationHandler( informationHandler)
{
}

bool MediaSetValidatorImpl::operator()()
{
  informationHandler( "Not implemented");

  return false;
}

}
