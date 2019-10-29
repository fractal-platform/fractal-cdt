
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

cd build
rd /S /Q tools
rd /S /Q libraries

mkdir tools
cd tools
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ../../tools/ && nmake && nmake install