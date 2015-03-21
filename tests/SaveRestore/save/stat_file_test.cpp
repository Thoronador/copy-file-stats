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

int main(int argc, char** argv)
{
  // check command line arguments
  if (argc < 3)
  {
    std::cout << "Hint: This program expects two parameters:\n"
              << "  1st) path to base directory for this test\n"
              << "  2nd) path of the \"stat file\" that will be created during the test.\n"
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
  // -- location of stat file
  if (argv[2] == NULL)
  {
    std::cout << "Error: argv[2] is a null pointer!\n";
    return 1;
  }
  const std::string statFile = std::string(argv[2]);


  if (!SaveRestore::save(baseDir, statFile, true))
  {
    std::cout << "SaveRestore::save() failed!\n";
    return 1;
  }

  //Everthing went fine... so far.
  return 0;
}
