/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for copy-file-stats.
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

#include <iostream>
#include "../../../program/SaveRestore.hpp"

/* Covered functions in test:
   This program tests the function SaveRestore::restore() and its ability to
   restore file stats from a given "stat file".
   Afterwards, the current stats are written to another stat file, which is
   then compared against a reference file to verify that the produced file
   matches the expected result and thus the SaveRestore::restore() function did
   its job as expected.
   The comparison is performed by a Bash script: stat-file-test.sh. This script
   is also used to set up the files and directories and their permissions
   (with the help of chown) for this test.
*/

int main(int argc, char** argv)
{
  // check command line arguments
  if (argc < 4)
  {
    std::cout << "Hint: This program expects two parameters:\n"
              << "  1st) path to base directory for this test\n"
              << "  2nd) path of the \"stat file\" that will be used as source during the test.\n"
              << "  3rd) path of the \"stat file\" that will be used as output during the test.\n"
              << "Aborting.\n";
    return 1;
  }
  if (argv == NULL)
  {
    std::cout << "Error: argv is a null pointer!\n";
    return 1;
  }
  // -- path to base directory
  if (argv[1] == NULL)
  {
    std::cout << "Error: argv[1] is a null pointer!\n";
    return 1;
  }
  std::string baseDir = std::string(argv[1]);
  // -- location of "source" stat file
  if (argv[2] == NULL)
  {
    std::cout << "Error: argv[2] is a null pointer!\n";
    return 1;
  }
  const std::string statFileSource = std::string(argv[2]);
  // -- location of "output" stat file
  if (argv[3] == NULL)
  {
    std::cout << "Error: argv[3] is a null pointer!\n";
    return 1;
  }
  const std::string statFileOutput = std::string(argv[3]);

  //Source and output file names shall not be the same.
  if (statFileOutput == statFileSource)
  {
    std::cout << "Error: Source and output file names are equal!\n";
    return 1;
  }


  SaveRestore testInstance;
  if (!testInstance.restore(baseDir, statFileSource, true))
  {
    std::cout << "SaveRestore::restore() failed!\n";
    return 1;
  }
  std::cout << "Info: SaveRestore::restore() succeeded.\n";

  //Everthing went fine so far. Let's save the current state to another file.
  if (!SaveRestore::save(baseDir, statFileOutput, true))
  {
    std::cout << "SaveRestore::save() failed!\n";
    return 1;
  }

  //Everthing went fine... so far.
  return 0;
}
