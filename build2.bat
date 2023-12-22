@echo off
start /B cmake -B ./test-build -G "MinGW Makefiles" && mingw32-make.exe -C ./test-build/ && copy .\test-build\app.exe .