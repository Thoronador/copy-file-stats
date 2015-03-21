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

# check parameter
if [[ -z $1 ]]
then
  echo "Error: Expecting one parameter - executable for stat_file_test!"
  return 1
fi

# create base directory where the test shall take place
BASE_DIR=`mktemp --directory --tmpdir testSaveRestoreXXXXXXXXXX`

# get default user name and group
USER_NAME=`id -un`
USER_ID=`id -u`
GROUP_NAME=`id -gn`
GROUP_ID=`id -g`

ALL_GROUPS=`id -Gn`

echo "Info: User $USER_NAME belongs to the following groups: $ALL_GROUPS."

# create some files
touch $BASE_DIR/alpha || echo "touch failed (alpha)"
chmod 0700 $BASE_DIR/alpha || echo "chmod failed (alpha)"

touch $BASE_DIR/beta || echo "touch failed (beta)"
chmod 0600 $BASE_DIR/beta || echo "chmod failed (beta)"

touch $BASE_DIR/gamma || echo "touch failed (gamma)"
chmod 0600 $BASE_DIR/gamma || echo "chmod failed (gamma)"

touch $BASE_DIR/delta || echo "touch failed (delta)"
chmod 0600 $BASE_DIR/delta || echo "chmod failed (delta)"

# -- create subdirectory
mkdir $BASE_DIR/sub

if [[ $? -ne 0 ]]
then
  echo "Failed to create subdirectory!"
  exit 1
fi

chmod 0777 $BASE_DIR/sub || echo "chmod failed (sub)"

# -- create some files in subdirectory
touch $BASE_DIR/sub/epsilon || echo "touch failed (epsilon)"
chmod 0600 $BASE_DIR/sub/epsilon || echo "chmod failed (epsilon)"

touch $BASE_DIR/sub/riemann || echo "touch failed (riemann)"
chmod 0600 $BASE_DIR/sub/riemann || echo "chmod failed (riemann)"

touch $BASE_DIR/sub/zeta || echo "touch failed (zeta)"
chmod 0600 $BASE_DIR/sub/zeta || echo "chmod failed (zeta)"

# -- create directory within subdirectory
mkdir $BASE_DIR/sub/marine

if [[ $? -ne 0 ]]
then
  echo "Failed to create subdirectory!"
  exit 1
fi

chmod 0770 $BASE_DIR/sub/marine || echo "chmod failed (marine)"

# create some files in .../sub/marine
touch $BASE_DIR/sub/marine/anachronistic || echo "touch failed (anachronistic)"
chmod 0700 $BASE_DIR/sub/marine/anachronistic || echo "chmod failed (anachronistic)"

touch $BASE_DIR/sub/marine/brontosaurus || echo "touch failed (brontosaurus)"
chmod 0700 $BASE_DIR/sub/marine/brontosaurus || echo "chmod failed (brontosaurus)"

touch $BASE_DIR/sub/marine/catharsis || echo "touch failed (catharsis)"
chmod 0700 $BASE_DIR/sub/marine/catharsis || echo "chmod failed (catharsis)"

# -- create another subdirectory
mkdir $BASE_DIR/trivial

if [[ $? -ne 0 ]]
then
  echo "Failed to create subdirectory!"
  exit 1
fi

chmod 0700 $BASE_DIR/trivial || echo "chmod failed (trivial)"

if [[ $? -ne 0 ]]
then
  echo "Failed to create test files!"
  exit 1
fi

echo "Files created successfully in $BASE_DIR!"

# name for stat file
OUTPUT_STAT_FILE=`mktemp --dry-run --tmpdir=/tmp statfileXXXXXXXX`
# ... and file from template against which it will be compared
REFERENCE_STAT_FILE=`mktemp --dry-run --tmpdir=/tmp statfile_compareXXXXXXXX`

# replace placeholders in template statfile.tpl with actual values
SCRIPT_DIR=`dirname $0`
sed "s/USER_NAME/$USER_NAME/g" $SCRIPT_DIR/statfile.tpl | sed "s/USER_ID/$USER_ID/g" | \
sed "s/GROUP_NAME/$GROUP_NAME/g" | sed "s/GROUP_ID/$GROUP_ID/g" > $REFERENCE_STAT_FILE

# run test
$1 $BASE_DIR/ $REFERENCE_STAT_FILE $OUTPUT_STAT_FILE
# ...and save its exit code
TEST_EXIT_CODE=$?

if [[ $TEST_EXIT_CODE -eq 0 ]]
then
  
  # compare both files with diff
  diff $OUTPUT_STAT_FILE $REFERENCE_STAT_FILE
  # files are identical, if return code of diff is zero
  DIFF_EXIT_CODE=$?
  if [[ $DIFF_EXIT_CODE -ne 0 ]]
  then
    echo "Error: Files are NOT identical."
    echo "Generated file:"
    cat $OUTPUT_STAT_FILE
    echo "Template-based file:"
    cat $REFERENCE_STAT_FILE
    echo "---- end of files ---"
    echo ""
    echo "Directory listings:"
    echo "base:"
    ls -la $BASE_DIR
    echo "sub:"
    ls -la $BASE_DIR/sub
    echo "marine:"
    ls -la $BASE_DIR/sub/marine
  fi
else
  echo "Executable returned non-zero exit code ($TEST_EXIT_CODE)."
  DIFF_EXIT_CODE=2
fi

# clean up
# -- test directory
rm -rf $BASE_DIR
# -- stat file
if [[ -f $OUTPUT_STAT_FILE ]]
then
  rm -f $OUTPUT_STAT_FILE
fi
# -- stat file generated from template
if [[ -f $REFERENCE_STAT_FILE ]]
then
  rm -f $REFERENCE_STAT_FILE
fi

if [[ $TEST_EXIT_CODE -eq 0 && $DIFF_EXIT_CODE -eq 0 ]]
then
  exit 0
else
  exit 1
fi
