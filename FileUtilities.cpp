/*
 -------------------------------------------------------------------------------
    This file is part of an utility to copy file permissions + ownership.
    Copyright (C) 2014  Thoronador

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

#include "FileUtilities.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "AuxiliaryFunctions.hpp"

#if defined(__linux__) || defined(linux)
  //Linux directory entries
  #include <dirent.h>
#else
  #error "Unknown operating system! Only compiles on Linux-like systems."
#endif

FileEntry::FileEntry()
: fileName(""), isDirectory(false)
{ }

std::vector<FileEntry> getDirectoryFileList(const std::string& Directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(__linux__) || defined(linux)
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.fileName = std::string(entry->d_name);
    one.isDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
    }
    entry = readdir(direc);
  }//while
  closedir(direc);
  #else
    #error "Unknown operating system!"
  #endif
  return result;
}//function

std::string getHumanReadableOwnership(const struct stat& statbuf)
{
  std::string result = "";
  struct passwd *pwd = getpwuid(statbuf.st_uid);
  if (NULL!=pwd)
    result = std::string(pwd->pw_name);
  else
    result = uintToString(statbuf.st_uid);
  result = result + ":";

  struct group * grp = getgrgid(statbuf.st_gid);
  if (NULL!=grp)
    result = result + std::string(grp->gr_name);
  else
    result = result + uintToString(statbuf.st_gid);
  return result;
}

bool copy_file_stats(const std::string& src_path, const std::string& dest_path, const bool permissions, const bool ownership, const bool verbose, const bool dryRun)
{
  if (!(permissions or ownership))
  {
    std::cout << "Hint: No stats for change!\n";
    return true;
  }
  struct stat src_statbuf;
  int ret = lstat(src_path.c_str(), &src_statbuf);
  if (0!=ret)
  {
    int errorCode = errno;
    std::cout << "Error while querying status of \"" << src_path << "\": Code " << errorCode << " (" << strerror(errorCode) << ").\n";
    return false;
  }
  struct stat dest_statbuf;
  ret = lstat(dest_path.c_str(), &dest_statbuf);
  if (0!=ret)
  {
    int errorCode = errno;
    if (errorCode==ENOENT)
    {
      //destination file does not exist, skip silently
      return true;
    }
    std::cout << "Error while querying status of \"" << dest_path << "\": Code "
              << errorCode << " (" << strerror(errorCode) << ").\n";
    return false;
  }
  //check for equivalence
  if ((dest_statbuf.st_dev == src_statbuf.st_dev) and (dest_statbuf.st_ino==src_statbuf.st_ino))
  {
    std::cout << "Error: " << src_path << " and " << dest_path << " are the same file!\n";
    return false;
  }

  // mode change allowed?
  if (permissions)
  {
    //check for required permission change
    if (dest_statbuf.st_mode != src_statbuf.st_mode)
    {
      if (verbose or dryRun)
      {
        std::cout << (dryRun ? "Would change mode of " : "Changing mode of ")
                  << dest_path << " from " << std::oct << dest_statbuf.st_mode
                  << " to " << std::oct << src_statbuf.st_mode << std::dec <<"...\n";
      }
      if (!dryRun)
      {
        ret = chmod(dest_path.c_str(), src_statbuf.st_mode);
        if (0!=ret)
        {
          int errorCode = errno;
          std::cout << "Error while changing mode of \"" << dest_path
                    << "\": Code " << errorCode << " (" << strerror(errorCode) << ").\n";
          return false;
        }
      }//if not dry run
    }
  }// if permissions

  // ownership change allowed?
  if (ownership)
  {
    //change needed?
    if ((dest_statbuf.st_uid!=src_statbuf.st_uid) or (dest_statbuf.st_gid!=src_statbuf.st_gid))
    {
      if (verbose or dryRun)
      {
          std::cout << (dryRun ? "Would change ownership of \"" : "Changing ownership of \"")
                    << dest_path << "\" from " << getHumanReadableOwnership(dest_statbuf)
                    << " to " << getHumanReadableOwnership(src_statbuf) << "...\n";
      }
      if (!dryRun)
      {
        ret = chown(dest_path.c_str(), src_statbuf.st_uid, src_statbuf.st_gid);
        if (0!=ret)
        {
          int errorCode = errno;
          std::cout << "Error while changing ownership of \"" << dest_path
                    << "\": Code " << errorCode << " (" << strerror(errorCode)
                    << ").\n";
          return false;
        }
      }//if not dry run
    } //if change required
  }//if ownership
  return true;
}

bool copy_stats_recursive(const std::string& src_dir, const std::string& dest_dir, const bool permissions, const bool ownership, const bool verbose, const bool dryRun)
{
  const std::vector<FileEntry> files = getDirectoryFileList(src_dir);
  unsigned int i;
  for (i=0; i<files.size(); ++i)
  {
    if ((files[i].fileName!="..") and (files[i].fileName!="."))
    {
      //handle file/dir itself
      if (!copy_file_stats(src_dir+pathDelimiter+files[i].fileName, dest_dir+pathDelimiter+files[i].fileName, permissions, ownership, verbose, dryRun))
      {
        return false;
      }
      //handle directory content, if it's a directory
      if (files[i].isDirectory)
      {
        if (!copy_stats_recursive(src_dir+pathDelimiter+files[i].fileName, dest_dir+pathDelimiter+files[i].fileName, permissions, ownership, verbose, dryRun))
        {
          return false;
        }
      }
    }//if not dot or dot+dot
  }//for
  return true;
}
