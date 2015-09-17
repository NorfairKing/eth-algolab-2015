#!/bin/bash
INPUT_FILE='sample.in'
OUTPUT_FILE='sample.out'

MAKEFILE='../makefile'

EXECUTABLE='main.bin'

TMP_OUTPUT='/tmp/output.txt'

# Make sure the script exist if anything goes wrong
set -e 

echo "Making..."
make --makefile ${MAKEFILE}
echo

echo -n "Running..."
time ./${EXECUTABLE} < ${INPUT_FILE} > ${TMP_OUTPUT}
echo

echo "Diff:"
diff ${OUTPUT_FILE} ${TMP_OUTPUT} 

if [[ "$?" == "0" ]]; then
  echo "Nothing, Yay!"
fi
