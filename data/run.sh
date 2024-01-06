#!/bin/sh
APP_NAME=$1

export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

CURR_DIR=$(dirname $0)
./bin/${APP_NAME}
