#!/bin/bash

# This file is part of the test suite for copy-file-stats.
# Copyright (C) 2015  Thoronador
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# create_file: creates a new empty file
#     param. #1: path of the file (full or relative)
#     param. #2: permission in octal representation (e.g. 0755)
function create_file()
{
  touch $1 || echo "touch failed (`basename $1`)"
  chmod $2 $1 || echo "chmod failed (`basename $1`)"
}


# create_diretory: creates a new empty directory
#     param. #1: path of the directory (full or relative)
#     param. #2: permission in octal representation (e.g. 0755)
function create_directory()
{
  mkdir $1

  if [[ $? -ne 0 ]]
  then
    echo "Failed to create subdirectory!"
    exit 1
  fi

  chmod $2 $1 || echo "chmod failed (`basename $1`)"
}

