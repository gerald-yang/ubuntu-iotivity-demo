DEMOROOT=$(pwd)

if [ -z "$IOTIVITY_ROOT" ]; then
	IOTIVITY_VER="iotivity-1.0.1"
fi

IOTIVITY=$DEMOROOT/$IOTIVITY_VER

print_usage() {
	echo "Usage:"
	echo "build.sh [TARGET] [ARCH]"
	echo " "
	echo "Without TARGET:"
       	echo "build whole iotivity project and ubuntu-demo for x86_64"
	echo " "
	echo "TARGET:"
	echo "demo          build ubuntu-demo folder only (need to build the whole iotivity project first)"
	echo "demo-snappy   copy binaries and libraries to snappy folder and build snap package"	
	echo "              (need to build demo first)"
	echo "clean         clean iotivity built files"
	echo " "
	echo "ARCH:"
	echo "x86_64"
	echo "armeabi-v7a-hard"
	echo " "
}

if [ -z "$1" ]; then
	TARGET="all"
elif [ "$1" == "demo" ] || [ "$1" == "demo-snappy" ]; then
	TARGET=$1
elif [ "$1" == "-h" ]; then
	print_usage
	exit 0
else
	echo "Unknown target $2"
	print_usage
	exit 1
fi

if [ -z "$2" ] || [ "$2" == "x86_64" ] ; then
	ARCH="x86_64"
	MAGIC_BIN="x86_64-linux-gnu"
elif [ "$2" == "armeabi-v7a-hard" ]; then
	ARCH=$2
	MAGIC_BIN="arm-linux-gnueabihf"
else
	echo "Unknown architecture $2"
	print_usage
	exit 1
fi

if [ -z "$3" ]; then
	CORE=4
else
	CORE=$3
fi

if [ -z "$4" ]; then
	SNAPCRAFT=$4
else
	SNAPCRAFT="snapcraft"
fi

if ! [ -L $IOTIVITY/ubuntu-demo ]; then
	echo "Make symlink to ubuntu-demo"
	cd $IOTIVITY && ln -s ../ubuntu-demo ubuntu-demo
fi

if [ "$TARGET" == "all" ]; then
	cd $IOTIVITY && scons -j$CORE TARGET_ARCH=$ARCH TARGET_OS=linux
elif [ "$1" == "demo" ]; then
	cd $IOTIVITY && scons -j$CORE TARGET_ARCH=$ARCH TARGET_OS=linux ubuntu-demo
elif [ "$1" == "demo-snappy" ]; then
	cp -f $IOTIVITY/out/linux/$ARCH/release/*.so snappy/nucdemo/lib/
	cp -f $IOTIVITY/out/linux/$ARCH/release/ubuntu-demo/nucdemo snappy/nucdemo/
	cd snappy/nucdemo && $SNAPCRAFT clean && $SNAPCRAFT
	cp -f $IOTIVITY/out/linux/$ARCH/release/*.so snappy/demogateway/lib/$MAGIC_BIN/
	cp -f $IOTIVITY/out/linux/$ARCH/release/ubuntu-demo/demogateway snappy/demogateway/magic-bin/$MAGIC_BIN/
	cd snappy/demogateway && $SNAPCRAFT clean && $SNAPCRAFT
elif [ "$1" == "clean" ]; then
	cd $IOTIVITY && scons -c TARGET_ARCH=$ARCH TARGET_OS=linux
fi
