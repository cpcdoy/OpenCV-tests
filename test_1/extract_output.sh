#!/bin/sh

docker build -t test .
id=$(docker run -d test)

if [ "$1" != "" ]; then
  o="Saved as "
  o+=$1
  echo $o
  docker cp $id:OpenCV-tests/test_1/output.jpg $1.jpg
else
  echo "Saved as output.jpg"
  docker cp $id:OpenCV-tests/test_1/output.jpg output.jpg
fi
