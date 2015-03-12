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

#ifndef SAVERESTORE_HPP
#define SAVERESTORE_HPP

#include <string>
#include <sys/stat.h>

/** \brief generates a string that contains all required file stats
 *
 * \param src_path   file name of the source file
 * \param statLine   string that shall hold the information
 * \return Returns true, if function succeeded. Returns false otherwise.
 *
 */
bool saveStats(const std::string& src_path, std::string& statLine);


/** \brief creates file mode (for chmod) from a string like "rwxr-xr--"
 *
 * \param mode_string a valid mode string, e.g. "rwxr-xr--"
 * \param mode   variable that will be used to store the resulting mode
 * \return Returns true, if the string could be translated to a mode_t value.
 *         Returns false otherwise.
 * \remarks The value of parameter mode is undefined, if the function returns
 *          false.
 */
bool stringToMode(const std::string& mode_string, mode_t& mode);

#endif // SAVERESTORE_HPP
