if not exist vsproj\nul goto :notexist
rd /s /q vsproj
:notexist
md vsproj
cd vsproj
cmake .. -G "Visual Studio 12 2013"