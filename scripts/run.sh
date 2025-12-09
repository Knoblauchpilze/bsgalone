#!/bin/sh

if [ $# -lt 2 ]; then
  echo "Usage: $0 app_name port [username] [password] [gamerole]"
  exit 1
fi

APP_NAME=$1
PORT=$2

USERNAME=""
PASSWORD=""
GAMEROLE=""
if [ $# -ge 5 ]; then
  USERNAME=$3
  PASSWORD=$4
  GAMEROLE=$5
fi

export PORT=$PORT

if [ "$USERNAME" != "" ] && [ "$PASSWORD" != "" ] && [ "$GAMEROLE" != "" ]; then
  export USERNAME=$USERNAME
  export PASSWORD=$PASSWORD
  export GAMEROLE=$GAMEROLE
fi

./bin/${APP_NAME}
