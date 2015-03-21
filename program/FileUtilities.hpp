/*
 -------------------------------------------------------------------------------
    This file is part of an utility to copy file permissions + ownership.
    Copyright (C) 2014, 2015  Thoronador

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

#ifndef FILEUTILITIES_HPP
#define FILEUTILITIES_HPP

#include <string>
#include <vector>
#include <sys/stat.h>

#if defined(_WIN32)
  const char pathDelimiter = '\\';
#elif defined(__linux__) || defined(linux)
  const char pathDelimiter = '/';
#else
  #error "Unknown operating system!"
#endif

/* structure for file list entries */
struct FileEntry {
    std::string fileName;
    bool isDirectory;

    /* constructor */
    FileEntry();
};//struct

/* returns a list of all files in the given directory as a vector */
std::vector<FileEntry> getDirectoryFileList(const std::string& Directory);


/** \brief transforms user ID and group ID into names
 *
 * \param userID  the user ID
 * \param groupID the group ID
 * \return Returns a string like "username:groupname".
 */
std::string getHumanReadableOwnership(const uid_t userID, const gid_t groupID);

std::string getHumanReadableOwnership(const struct stat& statbuf);

/* copies file permissions and/or ownership from file src_path to dest_path without copying the file itself

   parameters:
       src_path  - the source file
       dest_path - the destination file (permissions/ownership will be changed to match those of src_path)
       permissions - if set to true, permissions of dest_path will be adjusted
       ownership   - if set to true, ownership of dest_path will be adjusted
       verbose     - whether to print information (true) or not (false)
       dryRun      - if set to true, no actual changes will be made, but the function just shows what would be changed.

   return value:
       Returns true in case of success, or false if an error occurred.
*/
bool copy_file_stats(const std::string& src_path, const std::string& dest_path, const bool permissions, const bool ownership, const bool verbose, const bool dryRun);

bool copy_stats_recursive(const std::string& src_dir, const std::string& dest_dir, const bool permissions, const bool ownership, const bool verbose, const bool dryRun);

/** \brief checks for existence of file @fileName
 *
 * \param fileName  the file whose existence shall be determined
 * \return Returns true, if the file exists. Returns false, if not.
 */
bool fileExists(const std::string& fileName);

#endif // FILEUTILITIES_HPP
