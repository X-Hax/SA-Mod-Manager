if not exist build.release\nul goto :notexist
rd /s /q build.release
:notexist
md build.release
cd build.release
cmake .. -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles"
nmake