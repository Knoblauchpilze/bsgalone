#!/bin/sh

if [ $# -lt 1 ]; then
  echo "Usage: $0 app_name"
  exit 1
fi

APP_NAME=$1

./bin/${APP_NAME}
