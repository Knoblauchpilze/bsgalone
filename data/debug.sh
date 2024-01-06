#!/bin/sh
APP_NAME=$1

CURR_DIR=$(dirname $0)

gdb --args ./bin/${APP_NAME}
