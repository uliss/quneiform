Cuneiform for Linux 0.1

Cuneiform is an multi-language OCR system originally developed
and open sourced by Cognitive Technologies. Cuneiform was
originally a Windows program, which was ported to Linux
by Jussi Pakkanen with help from Keith Beaumont, Vincent
Wagelaar and others.


Caveats

This port is has several limitations. Among the biggest ones are:

- it only works on Linux
- it only works on 32 bit x86
- it only works if you run it in the build directory
- it only reads BMP files
- layout analysis is disabled

Patches to fix any of these issues are gladly accepted.

Compiling

Extract the source and go to the root folder (the one this file is in).
Then type the following commands:

mkdir builddir
cd builddir
cmake -DCMAKE_BUILD_TYPE=debug ..
make


Running

You MUST be in the root of the build directory. Then just do

./pumatest [-l language] <filename.bmp>

Output is written to pumaout.txt. Cuneiform assumes that your image contains
only a single column of text.

By default Cuneiform recognizes english text. To change the language use the
command line switch -l followed by your language string. To get a list of
supported languages type "./pumatest -l".


Contact information

Project home page: https://launchpad.net/cuneiform-linux
Email: jpakkane at gmail
