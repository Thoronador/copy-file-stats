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

#ifndef AUXILIARYFUNCTIONS_HPP
#define AUXILIARYFUNCTIONS_HPP

#include <string>

/** \brief converts an unsigned integer value to its string representation
 *
 * \param value  the unsigned integer
 * \return Returns a string that contains the given value.
 */
std::string uintToString(const unsigned int value);


/** \brief tries to convert the string representation of an unsigned integer to an unsigned int
 *
 * \param str    the string that contains the number
 * \param value  the unsigned int that will be used to store the result
 * \return Returns true on success, false on failure.
 * \remarks If false is returned, the value of parameter value is undefined.
 */
bool stringToUint(const std::string& str, unsigned int& value);


/** \brief adds a slash or backslash (or whatever is the path delimiter on the current
 * system) to the given path, if the path is not empty and has no path delimiter
 * as the last character yet.
 * \param path   the path that should (possibly) have an (back)slash
 * \return Returns the path with added slash, if required. Otherwise returns unmodified path.
 */
std::string slashify(const std::string& path);

#endif // AUXILIARYFUNCTIONS_HPP
