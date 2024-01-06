#!/bin/sh
APP_NAME=$1

CURR_DIR=$(dirname $0)
valgrind --log-file="analysis" --leak-check=yes --leak-check=full --show-leak-kinds=all ./bin/${APP_NAME}
