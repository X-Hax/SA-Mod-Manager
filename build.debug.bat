if not exist build.debug\nul goto :notexist
rd /s /q build.debug
:notexist
md build.debug
cd build.debug
cmake .. -G "NMake Makefiles"
nmake