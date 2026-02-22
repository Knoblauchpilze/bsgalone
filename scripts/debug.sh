#!/bin/bash

if [ $# -lt 2 ]; then
  echo "Usage: $0 app_name port"
  exit 1
fi

APP_NAME=$1
PORT=$2

export PORT=$PORT

gdb --args ./bin/${APP_NAME}
