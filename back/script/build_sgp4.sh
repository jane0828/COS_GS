CURRENT_DIR=$(pwd)

cd lib/sgp4
rm CMakeCache.txt
cmake CMakeLists.txt
make clean && make

cd $CURRENT_DIR

cp lib/sgp4/libsgp4/libsgp4.a bin/
cp lib/sgp4/libsgp4/libsgp4s.so bin/