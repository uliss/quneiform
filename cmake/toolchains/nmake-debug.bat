@echo off

set file=%~dp0%

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug "%file%..\.."

