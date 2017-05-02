#!/bin/sh

ARGS=$1

if [ $# -lt 1 ]; then
    ARGS="help"
fi

BUILD_PATH=$(cd `dirname $0`; pwd)/../

case $ARGS in
	3rd)
		cd $BUILD_PATH/3rd/; sh linux_3rd.sh
		;;
    all)
		cd $BUILD_PATH; cmake .;  make
        ;;
    cleanall)
        cd $BUILD_PATH; make clean; rm -rf CMakeFiles/ CMakeCache.txt Makefile cmake_install.cmake bin/ src/CMakeFiles/ src/CMakeCache.txt src/Makefile src/cmake_install.cmake test/test_srpc/CMakeFiles/ test/test_srpc/CMakeCache.txt test/test_srpc/Makefile test/test_srpc/cmake_install.cmake
        ;;
    help|*)
        echo "Usage:"
        echo "$0 help:     view help info."
		echo "$0 3rd:      build all 3rd."
        echo "$0 all:      build all target."
        echo "$0 cleanall: remove all temp file."
        ;;
esac