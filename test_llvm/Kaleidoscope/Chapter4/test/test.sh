#!/bin/bash

# Sandbox
# -- A collection of experimental code
# Copyright (C) 2011 onlyuser <mailto:onlyuser@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

show_help()
{
    echo "Usage: `basename $0` <EXEC> <INPUT_FILE> <GOLD_FILE> <OUTPUT_FILE_STEM>"
}

if [ $# -ne 4 ]; then
    echo "fail! -- expect 4 arguments! ==> $@"
    show_help
    exit 1
fi

TEMP_FILE_0=`mktemp`
trap "rm $TEMP_FILE_0" EXIT
TEMP_FILE_1=`mktemp`
trap "rm $TEMP_FILE_1" EXIT

EXEC=$1
INPUT_FILE=$2
GOLD_FILE=$3
OUTPUT_FILE_STEM=$4
PASS_FILE=${OUTPUT_FILE_STEM}.pass
FAIL_FILE=${OUTPUT_FILE_STEM}.fail

if [ ! -f $INPUT_FILE ]; then
    echo "fail! -- INPUT_FILE not found! ==> $INPUT_FILE"
    exit 1
fi

if [ ! -f $GOLD_FILE ]; then
    echo "fail! -- GOLD_FILE not found! ==> $GOLD_FILE"
    exit 1
fi

PROMPT="ready> "
DIV="; ModuleID = 'my cool jit'"
cat $INPUT_FILE | $EXEC |& sed "s/${PROMPT}//g" | tr "\n" ? | sed "s/?${DIV}.*//g" | tr ? "\n" > $TEMP_FILE_0
diff $TEMP_FILE_0 $GOLD_FILE | tee $TEMP_FILE_1
if [ ${PIPESTATUS[0]} -ne 0 ]; then # $? captures the last pipe
    echo "fail!"
    cp $TEMP_FILE_1 $FAIL_FILE # TEMP_FILE_1 already trapped on exit
    exit 1
fi

echo "success!" | tee $PASS_FILE
