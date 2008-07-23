Cuneiform for Linux 0.1

Cuneiform is an multi-language OCR system originally developed
and open sourced by Cognitive Technologies. Cuneiform was
originally a Windows program, which was ported to Linux
by Jussi Pakkanen. 

The following people have sent patches or have otherwise helped the
project. If someone is missing, please let me know, so I can add them.

Keith Beaumont
Vincent Wagelaar
Alexander Schlegel


Caveats

This port is has several limitations. Among the biggest ones are:

- it has been tested only on Linux and OS X
- it only works on 32 bit x86 processors
- it only works if you run it in the build directory
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

./pumatest [-l language] <image_file>

Output is written to pumaout.txt. Cuneiform assumes that your image contains
only a single column of text.

By default Cuneiform recognizes english text. To change the language use the
command line switch -l followed by your language string. To get a list of
supported languages type "./pumatest -l".

If you have ImageMagick++ on your system, Cuneiform autodetects and builds
against it. Then Cuneiform can process any image that ImageMagick knows how
to open. Otherwise it can only read uncompressed BMP images.

Contact information

Project home page: https://launchpad.net/cuneiform-linux
Email: jpakkane at gmail
