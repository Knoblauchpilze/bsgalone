#!/bin/sh
APP_NAME=$1

# Delete previous profiling.
rm -rf callgrind.out.*

CURR_DIR=$(dirname $0)
valgrind --tool=callgrind --log-file="analysis" ./bin/${APP_NAME}

gprof2dot -f callgrind callgrind.out.* | dot -Tpng -o output.png
gimp output.png
