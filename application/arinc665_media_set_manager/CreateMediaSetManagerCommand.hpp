/**
* @file
* @copyright
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* @author Thomas Vogt, thomas@thomas-vogt.de
*
* @brief Declaration of Class CreateMediaSetManagerCommand.
**/

#ifndef ARINC665_MEDIA_SET_MANAGER_CREATEMEDIASETMANAGERCOMMAND_HPP
#define ARINC665_MEDIA_SET_MANAGER_CREATEMEDIASETMANAGERCOMMAND_HPP

#include <commands/Commands.hpp>

class CreateMediaSetManagerCommand
{
  public:
    void execute( Commands::Parameters parameters );

    void help();
};


#endif
