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

#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <string>
#include <vector>

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

/* adds a slash or backslash (or whatever is the path delimiter on the current
   system) to the given path, if the path is not empty and has no path delimiter
   as the last character yet.

   parameters:
       path - the path that should (possibly) have an (back)slash
*/
std::string slashify(const std::string& path);

bool copy_file_stats(const std::string& src_path, const std::string& dest_path, const bool permissions, const bool ownership, const bool verbose);

bool copy_stats_recursive(const std::string& src_dir, const std::string& dest_dir, const bool permissions, const bool ownership, const bool verbose);

#endif // FILEUTILITIES_H
