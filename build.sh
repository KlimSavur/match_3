#!/bin/bash

# Please specify your qmake path here
QMAKE_PATH="/home/myownpc/Qt/5.15.0/gcc_64/bin"

QMAKE_EXEC=$QMAKE_PATH/qmake
BUILD_DIR=$PWD/build

echo "Start building"

if [ ! -f $QMAKE_EXEC ]; then
  echo "QMAKE_PATH is not properly specified. Exit"
  exit 1
fi

echo "qmake path: ${QMAKE_PATH}"

if [ ! -d $BUILD_DIR ]; then
  echo "Create build directory: ${BUILD_DIR}"
  mkdir build
fi

cd build

echo "Building plugin ..."
$QMAKE_EXEC ../Engine >/dev/null 2>&1
make -j 4 >/dev/null 2>&1

echo "Building application ..."
$QMAKE_EXEC ../Game >/dev/null 2>&1
make -j 4 >/dev/null 2>&1

echo "Done"
