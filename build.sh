print_usage() {
	echo "Usage:"
	echo "build.sh [TARGET]"
	echo " "
	echo "Without TARGET:"
       	echo "build whole iotivity project and ubuntu-demo"
	echo " "
	echo "TARGET:"
	echo "demo          build ubuntu-demo folder only (need to build the whole iotivity project first)"
	echo "demo-snappy   copy binaries and libraries to snappy folder and build snap package"	
	echo "              (need to build demo first)"
	echo " "
}


if [ -z $1 ]; then
	cd iotivity-1.0.1 && ln -f -s ../ubuntu-demo ubuntu-demo && scons -j4
elif [ "$1" == "demo" ]; then
	cd iotivity-1.0.1 && ln -f -s ../ubuntu-demo ubuntu-demo && scons -j4 ubuntu-demo
elif [ "$1" == "demo-snappy" ]; then
	cp -f iotivity-1.0.1/out/linux/x86_64/release/*.so snappy/iotivity-nucdemo/lib/
	cp -f iotivity-1.0.1/out/linux/x86_64/release/ubuntu-demo/nucdemo snappy/iotivity-nucdemo/
	cd snappy/iotivity-nucdemo && snapcraft clean && snapcraft
elif [ "$1" == "-h" ]; then
	print_usage
else
	echo "Unknow command $1"
	print_usage
fi
