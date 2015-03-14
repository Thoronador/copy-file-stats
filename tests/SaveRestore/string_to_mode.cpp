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
#include <string>
#include <tuple>
#include <vector>
#include "../../program/SaveRestore.hpp"

int main()
{
  /* Covered functions in test:
     This program tests the return values of SaveRestore::stringToMode(). */

  std::vector<std::tuple<std::string, bool, mode_t> > tests;

  tests.push_back(std::make_tuple("", false, 0));
  tests.push_back(std::make_tuple("rwx", false, 0));
  tests.push_back(std::make_tuple("RWXRWXRWX", false, 0));
  tests.push_back(std::make_tuple("abcdefghi", false, 0));
  tests.push_back(std::make_tuple("         ", false, 0));
  tests.push_back(std::make_tuple("---------", true,  0000));
  tests.push_back(std::make_tuple("--------x", true,  0001));
  tests.push_back(std::make_tuple("-------w-", true,  0002));
  tests.push_back(std::make_tuple("-------wx", true,  0003));
  tests.push_back(std::make_tuple("------r--", true,  0004));
  tests.push_back(std::make_tuple("------r-x", true,  0005));
  tests.push_back(std::make_tuple("------rw-", true,  0006));
  tests.push_back(std::make_tuple("------rwx", true,  0007));
  tests.push_back(std::make_tuple("-----x---", true,  0010));
  tests.push_back(std::make_tuple("----w----", true,  0020));
  tests.push_back(std::make_tuple("----wx---", true,  0030));
  tests.push_back(std::make_tuple("---r-----", true,  0040));
  tests.push_back(std::make_tuple("---r-x---", true,  0050));
  tests.push_back(std::make_tuple("---rw----", true,  0060));
  tests.push_back(std::make_tuple("---rwx---", true,  0070));
  tests.push_back(std::make_tuple("--x------", true,  0100));
  tests.push_back(std::make_tuple("-w-------", true,  0200));
  tests.push_back(std::make_tuple("-wx------", true,  0300));
  tests.push_back(std::make_tuple("r--------", true,  0400));
  tests.push_back(std::make_tuple("r-x------", true,  0500));
  tests.push_back(std::make_tuple("rw-------", true,  0600));
  tests.push_back(std::make_tuple("rw-r--r--", true,  0644));
  tests.push_back(std::make_tuple("rwx------", true,  0700));
  tests.push_back(std::make_tuple("rwxr-xr--", true,  0754));
  tests.push_back(std::make_tuple("rwxr-xr-x", true,  0755));
  tests.push_back(std::make_tuple("rwxrwxrwx", true,  0777));
  tests.push_back(std::make_tuple("--S--S--T", true, 07000));
  tests.push_back(std::make_tuple("--s--s--t", true, 07111));
  tests.push_back(std::make_tuple("rwsrwsrwt", true, 07777));


  tests.push_back(std::make_tuple("--------T", true, S_ISVTX));
  tests.push_back(std::make_tuple("--------t", true, S_ISVTX | 0001));
  tests.push_back(std::make_tuple("-----S---", true, S_ISGID));
  tests.push_back(std::make_tuple("-----s---", true, S_ISGID | 0010));
  tests.push_back(std::make_tuple("--S------", true, S_ISUID));
  tests.push_back(std::make_tuple("--s------", true, S_ISUID | 0100));

  //check all test cases for stringToMode()
  for (const std::tuple<std::string, bool, mode_t>& tup : tests)
  {
    mode_t mode = 0;
    const bool result = SaveRestore::stringToMode(std::get<0>(tup), mode);
    if (result != std::get<1>(tup))
    {
      std::cout << "Test failed!\n";
      const std::ios::fmtflags flags = std::cout.flags();
      std::cout << "Tuple was (\""<< std::get<0>(tup) << "\", " << std::get<1>(tup) << ", " << std::oct << std::get<2>(tup) << ").\n";
      std::cout.flags(flags);
      std::cout << "Result was " << (result ? "true" : "false") << ", but " << (std::get<1>(tup) ? "true" : "false") << " was expected.\n";
      return 1;
    } //if

    //Check mode - but only for tests that shall return true.
    if (std::get<1>(tup) && (mode != std::get<2>(tup)))
    {
      std::cout << "Test failed!\n";
      const std::ios::fmtflags flags = std::cout.flags();
      std::cout << "Tuple was (\""<< std::get<0>(tup) << "\", " << std::get<1>(tup) << ", " << std::oct << std::get<2>(tup) << ").\n";
      std::cout << "Returned mode was " << std::oct << mode << ", but it should be " << std::get<2>(tup) << " instead.\n";
      std::cout.flags(flags);
      return 1;
    } //if
  } //for

  std::cout << "All \"string to file mode\" tests passed.\n";
  return 0;
}
