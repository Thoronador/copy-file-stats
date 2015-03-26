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

#include <iostream>
#include "FileUtilities.hpp"
#include "SaveRestore.hpp"

const int rcInvalidParameter = 1;

void showGPLNotice()
{
  std::cout << "copy-file-stats\n"
            << "  Copyright (C) 2014, 2015  Thoronador\n"
            << "\n"
            << "  This programme is free software: you can redistribute it and/or\n"
            << "  modify it under the terms of the GNU General Public License as published\n"
            << "  by the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  This programme is distributed in the hope that they will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with this programme.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showHelp()
{
  std::cout << "\ncopy-file-stats [options] SOURCE_DIR DESTINATION_DIR\n"
            << "copy-file-stats [options] --save SOURCE_DIR STAT_FILE\n"
            << "copy-file-stats [options] --restore STAT_FILE DESTINATION_DIR\n"
            << "\n"
            << "options:\n"
            << "  --help           - display this help message and quit\n"
            << "  -?               - same as --help\n"
            << "  --version        - display the version of the programme and quit\n"
            << "  -v               - same as --version\n"
            << "  --license, -l    - show license information and quit\n"
            << "  --silent         - produce less output about ongoing changes\n"
            << "  --no-ownership   - do not change file ownership\n"
            << "  --no-chown       - alias for --no-ownership\n"
            << "  --no-permissions - do not change file permissions\n"
            << "  --no-chmod       - alias for --no-permissions\n"
            << "  --dry-run, -n    - only show what would be done, but don't do it\n"
            << "  --simulate, -s   - alias for --dry-run\n"
            << "  --force, -f      - force change of file permissions and/or ownership.\n"
            << "                     Either -f or -n have to be given to run this programme.\n"
            << "                     The only exception is --save, the programme always tries\n"
            << "                     to perform a save, there is no dry run for that.\n"
            << "  --save           - indicates that stats shall just be copied to a file and\n"
            << "                     not be applied to a source directory. Mutually exclusive\n"
            << "                     with --restore.\n"
            << "  --restore        - indicates that stats shall be retrieved from a stat file\n"
            << "                     and not from a source directory.\n"
            << "                     Mutually exclusive with --save.\n"
            << "  SOURCE_DIR       - set source directory (i.e. reference directory) to\n"
            << "                     SOURCE_DIR\n"
            << "  DESTINATION_DIR  - set destination directory to DESTINATION_DIR\n"
            << "  STAT_FILE        - when used with --save name of the file that will be used\n"
            << "                     to save the stats of the source directory.\n"
            << "                     When used with --restore name of the file that will be\n"
            << "                     used to restore stats of the destination directory.\n";
}

void showVersion()
{
  std::cout << "copy-file-stats, version 0.4.3, 2015-03-26\n";
}

int main(int argc, char **argv)
{
  std::string sourceDir = "";
  std::string destDir = "";
  bool verbose = true;
  bool adjustPermissions = true;
  bool adjustOwnership = true;
  bool dryRun = false;
  bool hasForceOrDryRun = false;
  bool save = false;
  bool restore = false;

  if ((argc>1) and (argv!=NULL))
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if ((param=="--help") or (param=="-?") or (param=="/?"))
        {
          showHelp();
          return 0;
        }//if help wanted
        //version information requested?
        else if ((param=="--version") or (param=="-v"))
        {
          showVersion();
          return 0;
        }
        else if ((param=="--licence") or (param=="--license") or (param=="-l"))
        {
          showGPLNotice();
          return 0;
        }
        else if (param=="--verbose")
        {
          verbose = true;
        }
        else if (param=="--silent")
        {
          if (!dryRun)
            verbose = false;
        }
        else if ((param=="--no-ownership") or (param=="--no-chown"))
        {
          adjustOwnership = false;
        }
        else if ((param=="--no-permission") or (param=="--no-permissions")  or (param=="--no-chmod"))
        {
          adjustPermissions = false;
        }
        else if ((param=="--ownership-only") or (param=="--chown-only"))
        {
          adjustPermissions = false;
          adjustOwnership = true;
        }
        else if ((param=="--permissions-only") or (param=="--chmod-only"))
        {
          adjustPermissions = true;
          adjustOwnership = false;
        }
        else if ((param=="--dry-run") or (param=="-n") or (param=="--simulate") or (param=="-s"))
        {
          if (!hasForceOrDryRun)
          {
            dryRun = true;
            hasForceOrDryRun = true;
          }
          else
          {
            std::cout << "Error: Parameters --force and --dry-run are mutually exclusive and may only be given once per run.\n";
            return rcInvalidParameter;
          }
        }
        else if ((param=="--force") or (param=="-f"))
        {
          if (!hasForceOrDryRun)
          {
            dryRun = false;
            hasForceOrDryRun = true;
          }
          else
          {
            std::cout << "Error: Parameters --force and --dry-run are mutually exclusive and may only be given once per run.\n";
            return rcInvalidParameter;
          }
        }
        else if (param=="--save")
        {
          if (save)
          {
            std::cout << "Error: Parameter --save may only be given once per run.\n";
            return rcInvalidParameter;
          }
          if (restore)
          {
            std::cout << "Error: Parameters --save and --restore are mutually exclusive.\n";
            return rcInvalidParameter;
          }
          save = true;
        } //if --save
        else if (param=="--restore")
        {
          if (restore)
          {
            std::cout << "Error: Parameter --restore may only be given once per run.\n";
            return rcInvalidParameter;
          }
          if (save)
          {
            std::cout << "Error: Parameters --save and --restore are mutually exclusive.\n";
            return rcInvalidParameter;
          }
          restore = true;
        } //if --restore
        else if (sourceDir.empty())
        {
          sourceDir = param;
          if (!restore)
            std::cout << "Source directory was set to \""<<sourceDir<<"\".\n";
          else
            std::cout << "Source stat file was set to \""<<sourceDir<<"\".\n";
        }//source directory
        else if (destDir.empty())
        {
          destDir = param;
          if (!save)
            std::cout << "Destination directory was set to \""<<destDir<<"\".\n";
          else
            std::cout << "Destination stat file was set to \""<<destDir<<"\".\n";
        }//dest. directory
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index "<<i<<" is NULL.\n";
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  if (sourceDir.empty() or destDir.empty())
  {
    if (save)
    {
      std::cout << "This programme requires a source directory and a destination stat file as parameters to run properly.\n"
                << "Use --help to get a list of valid parameters.\n";
    }
    else if (restore)
    {
      std::cout << "This programme requires a source stat file and a destination directory as parameters to run properly.\n"
                << "Use --help to get a list of valid parameters.\n";
    }
    else
    {
      std::cout << "This programme requires a source and a destination directory as parameters to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    }
    return rcInvalidParameter;
  } //if source or dest are missing

  //Either --force or --dry-run are required, unless we save to a stat file.
  if (!hasForceOrDryRun and !save)
  {
    std::cout << "Error: Neither --dry-run nor --force are given, refusing to run.\n";
    return rcInvalidParameter;
  }

  if (save and dryRun)
  {
    std::cout << "Info: The --dry-run option has no effect when used together with --save.\n";
  }

  //save user from possible mistakes
  if (destDir=="/")
  {
    std::cout << "You do NOT want to change the permissions or ownership of the root directory!\n";
    return 1;
  }

  if (save)
  {
    //save to a stat file
    if (SaveRestore::save(sourceDir, destDir, verbose))
    {
      std::cout << "Success!\n";
      return 0;
    }
    std::cout << "Failure!\n";
  }
  else if (restore)
  {
    //restore from a stat file
    SaveRestore instance;
    if (instance.restore(destDir, sourceDir, adjustPermissions, adjustOwnership, verbose, dryRun))
    {
      std::cout << "Success!\n";
      return 0;
    }
    std::cout << "Failure!\n";
  }
  else
  {
    //"default" directory to directory copying of stats
    if (copy_stats_recursive(sourceDir, destDir, adjustPermissions, adjustOwnership, verbose, dryRun))
    {
      std::cout << "Success!\n";
      return 0;
    }
    std::cout << "Failure!\n";
  } //else
  return 1;
}
