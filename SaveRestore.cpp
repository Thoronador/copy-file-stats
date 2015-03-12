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

#include "SaveRestore.hpp"
#include <pwd.h>
#include <grp.h>
#include "AuxiliaryFunctions.hpp"

bool saveStats(const std::string& src_path, std::string& statLine)
{
  struct stat src_statbuf;
  int ret = lstat(src_path.c_str(), &src_statbuf);
  if (0 != ret)
  {
    //error while querying status of src_path
    return false;
  }

  statLine.clear();
  //read access for user
  if ((src_statbuf.st_mode & S_IRUSR) == S_IRUSR)
    statLine = "r";
  else
    statLine = "-";
  //write access for user
  if ((src_statbuf.st_mode & S_IWUSR) == S_IWUSR)
    statLine.append("w");
  else
    statLine.append("-");
  //executable bit for user
  if ((src_statbuf.st_mode & S_IXUSR) == S_IXUSR)
  {
    //UID bit set?
    if ((src_statbuf.st_mode & S_ISUID) == S_ISUID)
      statLine.append("s");
    else
      statLine.append("x");
  }
  else
  {
    //UID bit set?
    if ((src_statbuf.st_mode & S_ISUID) == S_ISUID)
      statLine.append("S");
    else
      statLine.append("-");
  }

  //read access for group
  if ((src_statbuf.st_mode & S_IRGRP) == S_IRGRP)
    statLine.append("r");
  else
    statLine.append("-");
  //write access for group
  if ((src_statbuf.st_mode & S_IWGRP) == S_IWGRP)
    statLine.append("w");
  else
    statLine.append("-");
  //executable bit for group
  if ((src_statbuf.st_mode & S_IXGRP) == S_IXGRP)
  {
    //GID bit set?
    if ((src_statbuf.st_mode & S_ISGID) == S_ISGID)
      statLine.append("s");
    else
      statLine.append("x");
  }
  else
  {
    //GID bit set?
    if ((src_statbuf.st_mode & S_ISGID) == S_ISGID)
      statLine.append("S");
    else
      statLine.append("-");
  }

  //read access for others
  if ((src_statbuf.st_mode & S_IROTH) == S_IROTH)
    statLine.append("r");
  else
    statLine.append("-");
  //write access for others
  if ((src_statbuf.st_mode & S_IWOTH) == S_IWOTH)
    statLine.append("w");
  else
    statLine.append("-");
  //executable bit for others
  if ((src_statbuf.st_mode & S_IXOTH) == S_IXOTH)
  {
    //sticky bit set?
    if ((src_statbuf.st_mode & S_ISVTX) == S_ISVTX)
      statLine.append("t");
    else
      statLine.append("x");
  }
  else
  {
    //GID bit set?
    if ((src_statbuf.st_mode & S_ISVTX) == S_ISVTX)
      statLine.append("T");
    else
      statLine.append("-");
  }

  //space before user name
  statLine += " ";

  const struct passwd *pwd = getpwuid(src_statbuf.st_uid);
  if (NULL != pwd)
    statLine += std::string(pwd->pw_name);
  else
    statLine += "?";
  statLine += "(" + uintToString(src_statbuf.st_uid) + ")";

  //space before group name
  statLine += " ";

  const struct group * grp = getgrgid(src_statbuf.st_gid);
  if (NULL != grp)
    statLine += std::string(grp->gr_name);
  else
    statLine += "?";
  statLine += "(" + uintToString(src_statbuf.st_gid) + ")";

  //file name + space
  statLine += " " + src_path;
  return true;
}


bool stringToMode(const std::string& mode_string, mode_t& mode)
{
  //string should be exactly nine characters long
  if (mode_string.size() != 9)
    return false;
  //read access for user
  switch(mode_string[0])
  {
    case 'r':
         mode = S_IRUSR;
         break;
    case '-':
         mode = 0;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //write access for user
  switch(mode_string[1])
  {
    case 'w':
         mode |= S_IWUSR;
         break;
    case '-':
         // mode |= 0;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //executable status for user
  switch(mode_string[2])
  {
    case 'x':
         mode |= S_IXUSR;
         break;
    case '-':
         // mode |= 0;
         break;
    case 's':
         mode |= (S_IXUSR | S_ISUID);
         break;
    case 'S':
         mode |= S_ISUID;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //read access for group
  switch(mode_string[3])
  {
    case 'r':
         mode |= S_IRGRP;
         break;
    case '-':
         //mode |= 0;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //write access for group
  switch(mode_string[4])
  {
    case 'w':
         mode |= S_IWGRP;
         break;
    case '-':
         // mode |= 0;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //executable status for group
  switch(mode_string[5])
  {
    case 'x':
         mode |= S_IXGRP;
         break;
    case '-':
         // mode |= 0;
         break;
    case 's':
         mode |= (S_IXGRP | S_ISGID);
         break;
    case 'S':
         mode |= S_ISGID;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //read access for others
  switch(mode_string[6])
  {
    case 'r':
         mode |= S_IROTH;
         break;
    case '-':
         //mode |= 0;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //write access for others
  switch(mode_string[7])
  {
    case 'w':
         mode |= S_IWOTH;
         break;
    case '-':
         // mode |= 0;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //executable bit for others
  switch(mode_string[8])
  {
    case 'x':
         mode |= S_IXOTH;
         break;
    case '-':
         // mode |= 0;
         break;
    case 't':
         mode |= (S_IXOTH | S_ISVTX);
         break;
    case 'T':
         mode |= S_ISVTX;
         break;
    default:
         //invalid character
         return false;
  } //swi

  //finished successfully
  return true;
}
