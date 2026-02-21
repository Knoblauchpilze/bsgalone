#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: $0 folder"
  exit 1
fi

SOURCE_FOLDER="$1"

SOURCE_EXTENSION="cc"
HEADER_EXTENSION="hh"

ALL_FILES=$(find "${SOURCE_FOLDER}" -iname "*.${HEADER_EXTENSION}" -o -iname "*.${SOURCE_EXTENSION}")

ERRORS_FOUND="no"

for FILE in ${ALL_FILES}; do
  clang-format "${FILE}" > "${FILE}.clang-format"
  WRONG_LINES=$(diff "${FILE}" "${FILE}.clang-format" | wc -l)

  if [ ${WRONG_LINES} -gt 0 ]; then
    ERRORS_FOUND="yes"
    echo "Detected formatting issues in ${FILE}"
    diff "${FILE}" "${FILE}.clang-format"
  fi

  rm "${FILE}.clang-format"
done

if [[ "${ERRORS_FOUND}" == "no" ]]; then
  echo "No violations found"
else
  echo "ERROR: Coding style issues found"
  echo "Please run:"
  echo "find ${SOURCE_FOLDER} -iname '*.${HEADER_EXTENSION}' -o -iname '*.${SOURCE_EXTENSION}' | xargs clang-format -i"
  exit 1
fi
