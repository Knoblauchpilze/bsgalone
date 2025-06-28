#!/bin/sh

if [ $# -lt 1 ]; then
  echo "Usage: $0 app_name [filters]"
  echo "filters: if provided, will be passed as --gtest_filter to the test binary"
  exit 1
fi

APP_NAME=$1

FILTERS=""
if [ $# -ge 2 ]; then
  # https://stackoverflow.com/questions/12076072/how-to-run-specific-test-cases-in-googletest
  FILTERS="--gtest_filter=$2"
fi

./bin/${APP_NAME} ${FILTERS}
