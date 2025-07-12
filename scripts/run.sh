#!/bin/sh

if [ $# -lt 2 ]; then
  echo "Usage: $0 app_name port [username] [password}]"
  exit 1
fi

APP_NAME=$1
PORT=$2

USERNAME=""
PASSWORD=""
if [ $# -ge 4 ]; then
  USERNAME=$3
  PASSWORD=$4
fi

export PORT=$PORT

if [ "$USERNAME" != "" ] && [ "$PASSWORD" != "" ]; then
  export USERNAME=$USERNAME
  export PASSWORD=$PASSWORD
fi

./bin/${APP_NAME}
