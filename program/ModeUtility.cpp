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

#include "ModeUtility.hpp"

const mode_t Mode::cPermissionBitmask =
      S_ISUID | S_ISGID | S_ISVTX |
      S_IRUSR | S_IWUSR | S_IXUSR |
      S_IRGRP | S_IWGRP | S_IXGRP |
      S_IROTH | S_IWOTH | S_IXOTH;

mode_t Mode::onlyPermissions(const mode_t m)
{
  return (m & cPermissionBitmask);
}
