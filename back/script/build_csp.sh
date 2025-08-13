CURRENT_DIR=$(pwd)

cd lib/gscsp
python3 ./tools/buildtools/gsbuildtools_bootstrap.py
CFLAGS="-fPIC" CXXFLAGS="-fPIC" python3 ./waf distclean configure build
cd $CURRENT_DIR

cp lib/gscsp/build/libcsp-client.so bin/