/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::FileCreationPolicyDescription.
 **/

#include <arinc665/utils/FileCreationPolicyDescription.hpp>

#include <boost/exception/exception.hpp>
#include <boost/program_options.hpp>

namespace Arinc665 {
namespace Utils {

FileCreationPolicyDescription::FileCreationPolicyDescription():
  Description{
    { "None",         FileCreationPolicy::None},
    { "NoneExisting", FileCreationPolicy::NoneExisting},
    { "All",          FileCreationPolicy::All},

    { "Invalid",      FileCreationPolicy::Invalid}
  }
{
}

std::ostream& operator<<(
  std::ostream &stream,
  const FileCreationPolicy fileCreationPolicy)
{
  return (stream << FileCreationPolicyDescription::instance().findName(
    fileCreationPolicy));
}

std::istream& operator>>(
  std::istream& stream,
  FileCreationPolicy &fileCreationPolicy)
{
  std::string str;

  // extract string from stream
  stream >> str;

  // Decode
  fileCreationPolicy = FileCreationPolicyDescription::instance().findEnum( str);

  if ( FileCreationPolicy::Invalid == fileCreationPolicy)
  {
    BOOST_THROW_EXCEPTION(
      boost::program_options::invalid_option_value( str));
  }

  return stream;
}

}
}
