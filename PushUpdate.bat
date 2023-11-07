@echo off
setlocal enabledelayedexpansion

:input
set /p version=Enter a version number (x.y.z format, example: 1.0.5): 

REM Check if the input matches the expected format (x.y.z)
for /f "tokens=1-3 delims=." %%a in ("!version!") do (
    set "major=%%a"
    set "minor=%%b"
    set "patch=%%c"
)

if "%major%%minor%%patch%"=="" (
    echo Invalid version number format. Please use the x.y.z format.
    goto input
)

REM If the input is valid, push update
echo Valid version number: !version!
git tag -a !version! HEAD -m "Release !version!"
if errorlevel 1 (
    echo Failed to create Git tag.
    goto end
)

git push --follow-tags
if errorlevel 1 (
    echo Failed to push Git tags.
    goto end
)

:end
endlocal
pause
