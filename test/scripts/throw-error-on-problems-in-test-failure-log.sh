#!/bin/bash

set -e 
trap "echo \"Exiting from ${0} due to error signal.\"; exit 1;" ERR

echo "Begin checking for problems in test failure log."
#  Throw an error if there is anything in the test failure log
if [ $(cat logs/test-failures.log | wc -l) -ne 0 ]; then cat logs/test-failures.log; exit 1; fi
echo "No problems in test failure log observed."
exit 0;
