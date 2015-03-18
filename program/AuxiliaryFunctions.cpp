/*
 -------------------------------------------------------------------------------
    This file is part of an utility to copy file permissions + ownership.
    Copyright (C) 2015  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "AuxiliaryFunctions.hpp"
#include <limits>
#include <sstream>

std::string uintToString(const unsigned int value)
{
  std::ostringstream s_str;
  s_str << value;
  return s_str.str();
}

bool stringToUint(const std::string& str, unsigned int& value)
{
  if (str.empty()) return false;
  value = 0;
  const unsigned int cTenthLimit = std::numeric_limits<unsigned int>::max() / 10;
  const unsigned int cRealLimit = std::numeric_limits<unsigned int>::max();
  std::string::size_type i = 0;
  for ( ; i<str.size(); ++i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value>cTenthLimit) return false;
      value = value * 10;
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value>cRealLimit-(str.at(i)-'0')) return false;
      value = value + (str.at(i)-'0');
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  return true;
}

std::string slashify(const std::string& path)
{
  if (path.empty()) return path;
  //Does it have a trailing (back)slash?
  #if defined(_WIN32)
  const char pathDelimiter = '\\';
  #elif defined(__linux__) || defined(linux)
  const char pathDelimiter = '/';
  #else
    #error "Unknown operating system!"
  #endif
  if (path[path.length()-1] != pathDelimiter)
  {
    return path + pathDelimiter;
  }
  return path;
}
