DEMOROOT=$(pwd)

if [ -z "$IOTIVITY_ROOT" ]; then
	IOTIVITY_VER="iotivity-1.0.1"
fi

IOTIVITY=$DEMOROOT/$IOTIVITY_VER

print_usage() {
	echo "Usage:"
	echo "build.sh [TARGET] [ARCH] [CORE]"
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
	echo "CORE: build with CPU cores enabled (default: 4 cores)"
	echo " "
}

if [ -z "$1" ]; then
	TARGET="all"
elif [ "$1" == "all" ] || [ "$1" == "demo" ] || [ "$1" == "demo-snappy" ]; then
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
	SNAPCRAFT="snapcraft"
else
	SNAPCRAFT=$4
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
	if [ "$ARCH" == "x86_64" ]; then
		cp -f $IOTIVITY/out/linux/$ARCH/release/*.so $DEMOROOT/snappy/nucdemo/lib/
		cp -f $IOTIVITY/out/linux/$ARCH/release/ubuntu-demo/nucdemo $DEMOROOT/snappy/nucdemo/
		cd $DEMOROOT/snappy/nucdemo && $SNAPCRAFT clean && $SNAPCRAFT
	fi
	cp -f $IOTIVITY/out/linux/$ARCH/release/*.so $DEMOROOT/snappy/demogateway/lib/$MAGIC_BIN/
	cp -f $IOTIVITY/out/linux/$ARCH/release/ubuntu-demo/demogateway $DEMOROOT/snappy/demogateway/magic-bin/$MAGIC_BIN/
	cd $DEMOROOT/snappy/demogateway && $SNAPCRAFT clean && $SNAPCRAFT
	if [ "$ARCH" == "armeabi-v7a-hard" ]; then
		cp -f $DEMOROOT/grovepi/grovepi-server.py $DEMOROOT/snappy/grovepi/
		cp -rf $DEMOROOT/grovepi/pygrovepi $DEMOROOT/snappy/grovepi/
		cd $DEMOROOT/snappy/grovepi && $SNAPCRAFT clean && $SNAPCRAFT
		cp -f $IOTIVITY/out/linux/$ARCH/release/*.so $DEMOROOT/snappy/demoserver/lib/
		cp -f $IOTIVITY/out/linux/$ARCH/release/ubuntu-demo/demoserver $DEMOROOT/snappy/demoserver/
		cd $DEMOROOT/snappy/demoserver && $SNAPCRAFT clean && $SNAPCRAFT
	fi

elif [ "$1" == "clean" ]; then
	cd $IOTIVITY && scons -c TARGET_ARCH=$ARCH TARGET_OS=linux
fi
