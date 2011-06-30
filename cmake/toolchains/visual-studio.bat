@echo off

set file=%~dp0%

cmake -G "Visual Studio 10" -DCMAKE_BUILD_TYPE=Release "%file%..\.."

