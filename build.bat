@echo off
:: Change this path if your raylib is installed elsewhere
SET RAYLIB_PATH=C:\raylib\raylib\src

g++ src/main.cpp -o build/UnstoppableValet.exe ^
    -I src/include -I %RAYLIB_PATH% ^
    -L %RAYLIB_PATH% ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -Wl,--subsystem,windows

if %ERRORLEVEL% EQU 0 (
    echo Build Successful! Running...
    build\UnstoppableValet.exe
) else (
    echo Build Failed!
    pause
)