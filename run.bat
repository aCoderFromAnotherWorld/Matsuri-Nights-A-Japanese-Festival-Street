@echo off
cd /d "%~dp0Matsuri Nights — A Japanese Festival Street"
echo Starting Matsuri Nights...
if exist "x64\Debug\Matsuri Nights - A Japanese Festival Street.exe" (
    start "" "x64\Debug\Matsuri Nights - A Japanese Festival Street.exe"
) else (
    start "" "x64\Debug\Matsuri Nights — A Japanese Festival Street.exe"
)
