#!/bin/bash

set -e 
trap "echo \"Exiting from ${0} due to error signal.\"; exit 1;" ERR

echo "Begin checking for problems in valgrind log."
if [ $(cat logs/valgrind.log | wc -l) -eq 0 ]; then echo "Valgrind log is empty.  Should at least contain summary"; false; fi
if [ $(cat logs/valgrind.log | grep "All heap blocks were freed -- no leaks are possible" |  wc -l) -ne 1 ]; then cat logs/valgrind.log; false; fi
if [ $(cat logs/valgrind.log | grep "depends on uninitialised value" |  wc -l) -ne 0 ]; then cat logs/valgrind.log; false; fi
if [ $(cat logs/valgrind.log | grep "Invalid write" |  wc -l) -ne 0 ]; then cat logs/valgrind.log; false; fi
if [ $(cat logs/valgrind.log | grep "Invalid read" |  wc -l) -ne 0 ]; then cat logs/valgrind.log; false; fi
if [ $(cat logs/valgrind.log | grep "Bad permissions" |  wc -l) -ne 0 ]; then cat logs/valgrind.log; false; fi
echo "No valgrind log errors observed."
exit 0;
