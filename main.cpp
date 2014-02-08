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

#include <iostream>
#include "FileUtilities.h"

const int rcInvalidParameter = 1;

void showGPLNotice()
{
  std::cout << "copy-file-stats\n"
            << "  Copyright (C) 2014 Thoronador\n"
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
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -v               - same as --version\n"
            << "  --silent         - produces less output about ongoing changes\n"
            << "  --no-ownership   - do not change file ownership\n"
            << "  --no-chown       - alias for --no-ownership\n"
            << "  --no-permissions - do not change file permissions\n"
            << "  --no-chmod       - alias for --no-permissions\n"
            << "  --dry-run, -n    - only show what would be done, but don't do it\n"
            << "  --simulate, -s   - alias for --dry-run\n"
            << "  --force, -f      - force change of file permissions and/or ownership.\n"
            << "                     Either -f or -n have to be given to run this programme.\n"
            << "  SOURCE_DIR       - set source directory (i.e. reference directory) to\n"
            << "                     SOURCE_DIR\n"
            << "  DESTINATION_DIR  - set destination directory to DESTINATION_DIR\n";
}

void showVersion()
{
  std::cout << "copy-file-status, version 0.3, 2014-02-08\n";
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
        else if (sourceDir.empty())
        {
          sourceDir = param;
          std::cout << "Source directory was set to \""<<sourceDir<<"\".\n";
        }//source directory
        else if (destDir.empty())
        {
          destDir = param;
          std::cout << "Destination directory was set to \""<<destDir<<"\".\n";
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
    std::cout << "This programme requires a source and a destination directory as parameters to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  if (!hasForceOrDryRun)
  {
    std::cout << "Error: Neither --dry-run nor --force are given, refusing to run.\n";
    return rcInvalidParameter;
  }

  //save user from possible mistakes
  if (destDir=="/")
  {
    std::cout << "You do NOT want to change the permissions or ownership of the root directory!\n";
    return 1;
  }

  if (copy_stats_recursive(sourceDir, destDir, adjustPermissions, adjustOwnership, verbose, dryRun))
  {
    std::cout << "Success!\n";
    return 0;
  }
  std::cout << "Failure!\n";
  return 1;
}
