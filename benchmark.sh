#!/bin/sh
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

if [ "$COUNT" -lt 3 ] ; then
    echo "ministat requires at least 3 samples"
    exit 1
fi

FIRSTRESULT="$FIRST.result"
SECONDRESULT="$SECOND.result"

truncate -s 0 $FIRSTRESULT
truncate -s 0 $SECONDRESULT

# Make sure all necessary files are in cache
$FIRST  -f $INPUT -nv > /dev/null
$SECOND -f $INPUT -nv > /dev/null

for n in `seq 1 $COUNT` ; do
    echo "$n"
    $FIRST  -f $INPUT -nv -iv | tee -a $FIRSTRESULT
    $SECOND -f $INPUT -nv -iv | tee -a $SECONDRESULT
done

ministat -C 6 $FIRSTRESULT $SECONDRESULT
echo 'Instructions/second, higher is better'
if [ "$COUNT" -lt 30 ] ; then
    echo "Sample count $COUNT was less than 30, results might be unreliable"
fi
