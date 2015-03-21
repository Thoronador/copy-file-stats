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

#include <fstream>
#include <map>
#include <string>
#include <sys/stat.h>

class SaveRestore
{
  public:
    /** \brief constructor
     *
     * \param useCache  If set to true (default), then the class will cache
     *            the known users and groups for subsequent calls to
     *            stringToUID() and stringToGID().
     */
    SaveRestore(const bool useCache = true);


    /** \brief generates a string that contains all required file stats
     *
     * \param src_path   file name of the source file
     * \param removeSuffic  string that will be removed from the beginning of the file name
     * \param statLine   string that shall hold the information
     * \return Returns true, if function succeeded. Returns false otherwise.
     *
     */
    static bool getStatString(const std::string& src_path, const std::string& removeSuffix, std::string& statLine);


    /** \brief creates file mode (for chmod) from a string like "rwxr-xr--"
     *
     * \param mode_string a valid mode string, e.g. "rwxr-xr--"
     * \param mode   variable that will be used to store the resulting mode
     * \return Returns true, if the string could be translated to a mode_t value.
     *         Returns false otherwise.
     * \remarks The value of parameter mode is undefined, if the function returns
     *          false.
     */
    static bool stringToMode(const std::string& mode_string, mode_t& mode);


    /** \brief creates user ID (for chown) from a string
     *
     * \param user_name  the user name (e.g. "user1")
     * \param uid_string the user ID as string (e.g. "1000")
     * \param UID        variable that will be used to store the resulting UID
     * \return Returns true, if the string could be translated to a UID.
     *         Returns false otherwise.
     */
    bool stringToUID(const std::string& user_name, const std::string& uid_string, uid_t& UID);


    /** \brief creates group ID (for chown) from a string
     *
     * \param group_name  the group name (e.g. "www-data")
     * \param gid_string  the group ID as string (e.g. "1000")
     * \param GID         variable that will be used to store the resulting GID
     * \return Returns true, if the string could be translated to a GID.
     *         Returns false otherwise.
     */
    bool stringToGID(const std::string& group_name, const std::string& gid_string, gid_t& GID);


    /** \brief tries to extract all info from a line generated with saveStats()
     *
     * \param statLine  the line containing the file information (generated with saveStats())
     * \param mode      variable to hold the file mode (for chmod)
     * \param UID       variable to hold the user ID (for chown)
     * \param GID       variable to hold the group ID (for chown)
     * \param filename  variable to hold the file name
     * \return Returns true, if all info could be extracted. Returns false otherwise.
     * \remarks The contents of the reference parameters are undefined, if the
     *          function fails (i.e. returns false).
     */
    bool statLineToData(const std::string& statLine, mode_t& mode, uid_t& UID, gid_t& GID, std::string& filename);


    /** \brief tries to save the file information (permissions + owner/group) to a text file
     *
     * \param src_directory the directory whose info shall be saved
     * \param statFileName  name of the file that will be used to store the info
     * \param verbose       if set to true, shows more info about errors
     * \return Returns true, if all info was saved. Returns false otherwise.
     */
    static bool save(const std::string& src_directory, const std::string& statFileName, const bool verbose);


    /** \brief tries to restore the file information (permissions + owner/group) from a text file
     *
     * \param dest_directory  the directory whose info shall be restored
     * \param statFileName    name of the file that will be used to load the info
     * \param verbose         if set to true, shows more info about errors
     * \param dryRun          If set to true, no actual changes will be made, but the function just shows what would be changed.
     * \return Returns true, if all info was restored. Returns false otherwise.
     */
    bool restore(const std::string& dest_directory, const std::string& statFileName, const bool verbose, const bool dryRun);
  private:
    bool mUseCache; /**< whether caches are used or not */
    std::map<std::string, uid_t> mUserCache;  /**< caches user name -> user ID associations */
    std::map<std::string, gid_t> mGroupCache; /**< caches group name -> group ID associations */

    static bool saveRecursive(const std::string& src_directory, const std::string& removeSuffix, std::ofstream& statStream, const bool verbose);
}; //class

#endif // SAVERESTORE_HPP
