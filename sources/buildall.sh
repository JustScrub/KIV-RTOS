echo Building Standard Library
./build.sh --stdlib
echo
echo Building Userspace Tasks
cd userspace
./build.sh
cd ..
echo
echo Building Kernel
./build.sh