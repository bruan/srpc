git clone -b 2.7.0 https://github.com/google/protobuf.git ./protobuf_src

cd protobuf_src/cmake
mkdir build & cd build
mkdir debug & cd debug
cmake -G "NMake Makefiles"  -DCMAKE_BUILD_TYPE=Debug  -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../../install_debug ../..
nmake
nmake install

cd ../../../../
cd protobuf_src/cmake
cd build
mkdir release & cd release
cmake -G "NMake Makefiles"  -DCMAKE_BUILD_TYPE=Release  -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../../install_release ../..
nmake
nmake install

cd ../../../../
mkdir protobuf & cd protobuf
mkdir include
mkdir lib & cd lib
mkdir win32 & cd win32
mkdir debug
mkdir release

cd ../../../

xcopy protobuf_src\install_debug\include protobuf\include /e /s /i /y

xcopy protobuf_src\install_debug\lib\libprotobufd.lib protobuf\lib\win32\debug /e /s /i /y
xcopy protobuf_src\install_release\lib\libprotobuf.lib protobuf\lib\win32\release /e /s /i /y