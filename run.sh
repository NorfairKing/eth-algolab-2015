#!/bin/bash
INPUT_FILE='input.txt'
OUTPUT_FILE='output.txt'

MAKEFILE='../build/makefile'
EXECUTABLE='main.bin'

TMP_OUTPUT='/tmp/output.txt'

echo "Making..."
make --makefile ${MAKEFILE}
echo

echo "Running..."
time ./${EXECUTABLE} < ${INPUT_FILE} > ${TMP_OUTPUT}
echo

echo "Diff:"
diff ${OUTPUT_FILE} ${TMP_OUTPUT}
