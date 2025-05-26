#!/bin/sh
APP_NAME=$1

gdb --args ./bin/${APP_NAME}
