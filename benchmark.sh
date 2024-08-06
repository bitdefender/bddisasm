#!/bin/bash
# Must use bash instead of sh to support TIEMFORMAT
set -e

if [ "$#" -ne 4 ] ; then
    echo "Compare the speed of two different disasmtool versions"
    echo "Usage $0 <first disasmtool> <second disasmtool> <input file> <iterations>"
    exit 0
fi

FIRST=$1
SECOND=$2
INPUT=$3
COUNT=$4

if [ ! -x "$FIRST" ] ; then
    echo "First program $FIRST does not exist or is not executable"
    exit 1
fi

if [ ! -x "$SECOND" ] ; then
    echo "Second program $SECOND does not exist or is not executable"
    exit 1
fi

if [ ! -f "$INPUT" ] ; then
    echo "Input file $INPUT does not exist"
    exit 1
fi

case $COUNT in
    ''|*[!0-9]*) echo "Iteration count $COUNT is not a number" ; exit 1 ;;
    *) ;;
esac

truncate -s 0 before.txt
truncate -s 0 after.txt

# Make sure all necessary files are in cache
$FIRST  -f $INPUT -nv > /dev/null
$SECOND -f $INPUT -nv > /dev/null

TIMEFORMAT='%3U'

for n in `seq 1 $COUNT` ; do
	echo "$n"
    { time $FIRST  -f $INPUT -nv ; } 2>&1 | tee -a before.txt
    { time $SECOND -f $INPUT -nv ; } 2>&1 | tee -a after.txt
done

echo 'Time in seconds, lower is better'
ministat before.txt after.txt
