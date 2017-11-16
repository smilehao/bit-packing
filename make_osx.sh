mkdir -p build_osx && cd build_osx
cmake -GXcode ../
cd ..
cmake --build build_osx --config Release
mkdir -p Plugins/BitPacking.bundle/Contents/MacOS/
cp build_osx/Release/BitPacking.bundle/Contents/MacOS/BitPacking Plugins/BitPacking.bundle/Contents/MacOS/BitPacking
rm -rf build_osx
