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
#include <cstdlib> //for mkdtemp(), free()
#include <cstring> //for memset()
#include <unistd.h> //for rmdir(), unlink()
#include <fstream>
#include "../../program/SaveRestore.hpp"

int main()
{
  const unsigned int buffer_size = 50;
  char* buffer = new char[50];
  memset(buffer, '\0', buffer_size);
  const std::string tempName = "/tmp/testdir.XXXXXX";
  strncpy(buffer, tempName.c_str(), tempName.size());

  char * tmpDirNameBuffer = mkdtemp(buffer);
  if (tmpDirNameBuffer == NULL)
  {
    std::cout << "Error: Could not create temporary directory!\n";
    delete[] buffer;
    buffer = NULL;
    return 1;
  }

  const std::string tempDir = std::string(buffer);
  delete [] buffer;
  buffer = NULL;
  char * fn = tempnam(tempDir.c_str(), "mode");
  if (NULL == fn)
  {
    std::cout << "Error: Could not get temporary file name!\n";
    return 1;
  }
  std::string tempFileName = std::string(fn);
  free(fn);

  //create file with some random content
  std::ofstream testStream(tempFileName, std::ios::out | std::ios::binary | std::ios::trunc);
  if (not testStream.good())
  {
    std::cout << "Error: File creation failed!\n";
    return 1;
  }
  const std::string fileContent = std::string("This file is here for testing")
        +" purposes only and can be safely deleted, once the test is over.\n";
  testStream.write(fileContent.c_str(), fileContent.size());
  if (!testStream.good())
  testStream.close();


  //now on to the real testing
  SaveRestore sr_instance;
  std::string statLine;
  mode_t data_mode = 0;
  uid_t UID;
  gid_t GID;
  std::string name;

  //change mode to all possible combinations
  mode_t m;
  for (m = 0; m <= 07777; ++m)
  {
    if (0 != chmod(tempFileName.c_str(), m))
    {
      std::cout << "Error: Could not set file mode to " << std::oct << m << ".\n";
      unlink(tempFileName.c_str());
      rmdir(tempDir.c_str());
      return 1;
    }
    if (!sr_instance.getStatString(tempFileName, statLine))
    {
      std::cout << "Error: saveStats failed!\n";
      unlink(tempFileName.c_str());
      rmdir(tempDir.c_str());
      return 1;
    }
    if (!sr_instance.statLineToData(statLine, data_mode, UID, GID, name))
    {
      std::cout << "Error: SaveRestore::statLineToData() failed!\n";
      unlink(tempFileName.c_str());
      rmdir(tempDir.c_str());
      return 1;
    }
    if (data_mode != m)
    {
      std::cout << "Test failed for file mode " << std::oct << m << ".\n"
                << "Returned mode was " << std::oct << data_mode << " instead.\n";
      unlink(tempFileName.c_str());
      rmdir(tempDir.c_str());
      return 1;
    }
  } //for

  std::cout << "All mode tests passed. :)\n";

  //remove temporary file that was created earlier
  if (0 != unlink(tempFileName.c_str()))
  {
    std::cout << "Error: Could not remove temporary file " << tempFileName << ".\n";
    return 1;
  }

  //remove temporary directory that was created earlier
  if (0 != rmdir(tempDir.c_str()))
  {
    std::cout << "Error: Could not remove temporary directory " << tempDir << "!\n";
    return 1;
  }
  return 0;
}
