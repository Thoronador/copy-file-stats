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
#include <iostream>
#include <vector>
#include <cerrno>  //for errno
#include <cstring> //for strerror()
#include <pwd.h>
#include <grp.h>
#include <unistd.h> //for lchown()
#include "AuxiliaryFunctions.hpp"
#include "FileUtilities.hpp"

SaveRestore::SaveRestore(const bool useCache)
: mUseCache(useCache),
  mUserCache(std::map<std::string, uid_t>()),
  mGroupCache(std::map<std::string, gid_t>())
{
}

bool SaveRestore::getStatString(const std::string& src_path, const std::string& removeSuffix, std::string& statLine)
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
  statLine += " " + uintToString(src_statbuf.st_uid);

  //space before group name
  statLine += " ";

  const struct group * grp = getgrgid(src_statbuf.st_gid);
  if (NULL != grp)
    statLine += std::string(grp->gr_name);
  else
    statLine += "?";
  statLine += " " + uintToString(src_statbuf.st_gid);

  //file name + space
  if (removeSuffix.empty())
  {
    statLine += " " + src_path;
  }
  else
  {
    if (src_path.substr(0, removeSuffix.size()) == removeSuffix)
    {
      std::string temp(src_path);
      temp.erase(0, removeSuffix.size());
      statLine += " " + temp;
    }
    else
    {
      statLine += " " + src_path;
    }
  } //else (removeSuffix not empty)
  return true;
}


bool SaveRestore::stringToMode(const std::string& mode_string, mode_t& mode)
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

bool SaveRestore::stringToUID(const std::string& user_name, const std::string& uid_string, uid_t& UID)
{
  if ((user_name != "?") && (!user_name.empty()))
  {
    //Can we use the cache and is the name already in the cache?
    if (mUseCache)
    {
      const std::map<std::string, uid_t>::const_iterator found = mUserCache.find(user_name);
      if (found != mUserCache.end())
        return found->second;
    }
    const struct passwd* ptr = getpwnam(user_name.c_str());
    if (ptr != NULL)
    {
      UID = ptr->pw_uid;
      if (mUseCache)
        mUserCache[user_name] = ptr->pw_uid;
      return true;
    }
  } //if user_name not "?"
  //fall back on uid string
  unsigned int possibleUID = 0;
  if (stringToUint(uid_string, possibleUID))
  {
    UID = possibleUID;
    return true;
  } //if
  return false;
}

bool SaveRestore::stringToGID(const std::string& group_name, const std::string& gid_string, gid_t& GID)
{
  if ((group_name != "?") && (!group_name.empty()))
  {
    //Can we use the cache and is the name in the cache already?
    if (mUseCache)
    {
      const std::map<std::string, gid_t>::const_iterator found = mGroupCache.find(group_name);
      if (found != mGroupCache.end())
        return found->second;
    }

    const struct group* ptr = getgrnam(group_name.c_str());
    if (ptr != NULL)
    {
      GID = ptr->gr_gid;
      if (mUseCache)
        mGroupCache[group_name] = ptr->gr_gid;
      return true;
    }
  } //if group_name not "?"
  //fall back on gid string
  unsigned int possibleGID = 0;
  if (stringToUint(gid_string, possibleGID))
  {
    GID = possibleGID;
    return true;
  } //if
  return false;
}

bool SaveRestore::statLineToData(const std::string& statLine, mode_t& mode, uid_t& UID, gid_t& GID, std::string& filename)
{
  if (statLine.empty())
    return false;

  std::string modeString;
  std::string name;
  std::string idString;
  filename.clear();

  std::string::const_iterator sIter = statLine.begin();
  while (sIter != statLine.end())
  {
    if (*sIter != ' ')
    {
      modeString.push_back(*sIter);
    }
    else
    {
      //space - break out of loop
      break;
    }
    ++sIter;
  } //while
  if (!stringToMode(modeString, mode))
    return false;

  if (sIter == statLine.end())
    return false;
  ++sIter;
  while (sIter != statLine.end())
  {
    if (*sIter != ' ')
    {
      name.push_back(*sIter);
    }
    else
    {
      //space - break out of loop
      break;
    }
    ++sIter;
  } //while

  //skip space
  if (sIter == statLine.end())
    return false;
  ++sIter;

  while (sIter != statLine.end())
  {
    if (*sIter != ' ')
    {
      idString.push_back(*sIter);
    }
    else
    {
      //space - break out of loop
      break;
    }
    ++sIter;
  } //while

  if (!stringToUID(name, idString, UID))
    return false;

  //skip space
  if (sIter == statLine.end())
    return false;
  ++sIter;

  name.clear();
  idString.clear();

  while (sIter != statLine.end())
  {
    if (*sIter != ' ')
    {
      name.push_back(*sIter);
    }
    else
    {
      //space - break out of loop
      break;
    }
    ++sIter;
  } //while

  //skip space
  if (sIter == statLine.end())
    return false;
  ++sIter;

  while (sIter != statLine.end())
  {
    if (*sIter != ' ')
    {
      idString.push_back(*sIter);
    }
    else
    {
      //space - break out of loop
      break;
    }
    ++sIter;
  } //while

  if (!stringToGID(name, idString, GID))
    return false;

  //skip space
  if (sIter == statLine.end())
    return false;
  ++sIter;

  filename = std::string(sIter, statLine.end());
  return (!filename.empty());
}

bool SaveRestore::save(const std::string& src_directory, const std::string& statFileName, const bool verbose)
{
  //We don't want to overwrite an existing file.
  if (fileExists(statFileName))
  {
    if (verbose)
      std::cout << "Error: file " << statFileName << " already exists and we do not want to overwrite it.\n";
    return false;
  }

  //open file for writing
  std::ofstream statStream(statFileName.c_str(), std::ios::out | std::ios::binary);
  if (!statStream.good())
  {
    if (verbose)
      std::cout << "Error: Could not create/open file " << statFileName << ".\n";
    return false;
  }

  const bool success = saveRecursive(src_directory, slashify(src_directory), statStream, verbose);
  //close file
  statStream.close();
  return success;
}

bool SaveRestore::saveRecursive(const std::string& src_directory, const std::string& removeSuffix, std::ofstream& statStream, const bool verbose)
{
  const std::vector<FileEntry> files = getDirectoryFileList(src_directory);
  //empty directory or directory does not exist
  if (files.empty())
  {
    if (verbose)
      std::cout << "Error: Directory \"" << src_directory << "\" does not exist or is empty.\n";
    return false;
  }

  #ifdef DEBUG
  {
    std::cout << "DEBUG: content of files (" << files.size() << " entries):\n";
    unsigned int i;
    for (i = 0; i < files.size(); ++i)
    {
      std::cout << (files[i].isDirectory ? "  d " : "  f ") << files[i].fileName << "\n";
    } //for
    std::cout << "--- end of files ---\n";
  } //scope
  #endif // DEBUG

  unsigned int i;
  for (i = 0; i < files.size(); ++i)
  {
    if ((files[i].fileName != "..") and (files[i].fileName != "."))
    {
      //handle file/dir itself
      std::string line;
      if (!getStatString(slashify(src_directory)+files[i].fileName, removeSuffix, line))
      {
        if (verbose)
          std::cout << "Error: Could not generate info line for file " << (slashify(src_directory)+files[i].fileName) << ".\n";
        return false;
      }
      //write to file
      statStream.write(line.c_str(), line.size());
      statStream.write("\n", 1);
      if (!statStream.good())
      {
        if (verbose)
          std::cout << "Error: Failed to write to info file.\n";
        return false;
      }

      //handle directory content, if it's a directory
      if (files[i].isDirectory)
      {
        if (!saveRecursive(slashify(src_directory)+files[i].fileName, removeSuffix, statStream, verbose))
        {
          return false;
        }
      }
    }//if not dot or dot+dot
  }//for
  return true;
}

bool SaveRestore::restore(const std::string& dest_directory, const std::string& statFileName, const bool permissions, const bool ownership, const bool verbose, const bool dryRun)
{
  if (!(permissions or ownership))
  {
    std::cout << "Hint: No stats to change!\n";
    return true;
  }
  if (!fileExists(statFileName))
  {
    std::cout << "Error: file " << statFileName << " does not exist.\n";
    return false;
  }

  //open file for reading
  std::ifstream statStream(statFileName.c_str(), std::ios::in | std::ios::binary);
  if (!statStream.good())
  {
    std::cout << "Error: Could not open file " << statFileName << ".\n";
    return false;
  }


  mode_t mode;
  uid_t UID;
  gid_t GID;
  std::string file;

  struct stat dest_statbuf;
  int ret = 0;

  const unsigned int cMaxLine = 256;
  char buffer[cMaxLine];
  std::string line = "";
  while (statStream.getline(buffer, cMaxLine-1))
  {
    buffer[cMaxLine-1] = '\0';
    line = std::string(buffer);
    if (!statLineToData(line, mode, UID, GID, file))
    {
      statStream.close();
      std::cout << "Error: Could not extract data from line \"" << line << "\"!\n";
      return false;
    } //if statLineToData() failed


    const std::string destinationFile(slashify(dest_directory) + file);

    ret = lstat(destinationFile.c_str(), &dest_statbuf);
    if (0 != ret)
    {
      const int errorCode = errno;
      if (errorCode != ENOENT)
      {
        std::cout << "Error while querying status of \"" << destinationFile << "\": Code "
                  << errorCode << " (" << strerror(errorCode) << ").\n";
        statStream.close();
        return false;
      }
      else
      {
        //destination file does not exist, skip silently - except when verbose
        if (verbose or dryRun)
          std::cout << "Info: file \""<<destinationFile<<"\" does not exist, skipping.\n";
      } //else
    }
    else
    {
      //mode change requested?
      if (permissions)
      {
        if (dest_statbuf.st_mode != mode)
        {
          if (verbose or dryRun)
            std::cout << (dryRun ? "Would change mode of " : "Changing mode of ")
                      << destinationFile << " from " << std::oct << dest_statbuf.st_mode
                    << " to " << std::oct << mode << std::dec <<"...\n";
          if (!dryRun)
          {
            ret = chmod(destinationFile.c_str(), mode);
            if (0 != ret)
            {
              int errorCode = errno;
              std::cout << "Error while changing mode of \"" << destinationFile
                        << "\": Code " << errorCode << " (" << strerror(errorCode) << ").\n";
              statStream.close();
              return false;
            } //if ret != 0
          } //if not dryRun
        } //if permissions do not match
      } //if permissions shall be adjusted

      if (ownership)
      {
        //check, if user and group match
        if ((dest_statbuf.st_uid != UID) || (dest_statbuf.st_gid != GID))
        {
          if (verbose or dryRun)
          {
            std::cout << (dryRun ? "Would change ownership of \"" : "Changing ownership of \"")
                      << destinationFile << "\" from " << getHumanReadableOwnership(dest_statbuf)
                      << " to " << getHumanReadableOwnership(UID, GID) << "...\n";
          }
          if (!dryRun)
          {
            ret = lchown((dest_directory + file).c_str(), UID, GID);
            if (0 != ret)
            {
              const int errorCode = errno;
              statStream.close();
              std::cout << "Error while changing ownership of \"" << destinationFile
                        << "\": Code " << errorCode << " (" << strerror(errorCode)
                        << ").\n";
              return false;
            } //if 0 != ret
          } //if not dry run
        } //if owners do not match
      } //if ownership shall be adjusted
    } //else (lstat succeeded)
  } //while

  statStream.close();
  return true;
}
