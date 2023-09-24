#!/bin/sh

CURR_DIR=$(dirname $0)
valgrind --log-file="analysis" --leak-check=yes --leak-check=full --show-leak-kinds=all ./bin/bsgalone
