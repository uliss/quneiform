Cuneiform for Linux 0.4.0

Cuneiform is an multi-language OCR system originally developed
and open sourced by Cognitive Technologies. Cuneiform was
originally a Windows program, which was ported to Linux
by Jussi Pakkanen. 

The following people have sent patches or have otherwise helped the
project. If someone is missing, please let me know, so I can add them.

Keith Beaumont
Vincent Wagelaar
zanin
Alexander Schlegel
Alex Samorukov
yaleks
Serj Poltavskiy


Caveats

This port is has several limitations. Among the biggest ones are:

- it has been tested only on Linux and OS X (FreeBSD should also work)
- it only works on x86 and amd64 processors
- there is no table recognition, because of
  https://bugs.launchpad.net/cuneiform-linux/+bug/260327

Patches to fix any of these issues are gladly accepted.


Compiling

Extract the source and go to the root folder (the one this file is in).
Then type the following commands:

mkdir builddir
cd builddir
cmake -DCMAKE_BUILD_TYPE=debug ..
make
make install 

By default Cuneiform installs to /usr/local. You can specify a different prefix
by giving a command line switch "-DCMAKE_INSTALL_PREFIX=/what/ever/you/want"
to CMake.

If you have ImageMagick++ on your system, Cuneiform autodetects and builds
against it. Then Cuneiform can process any image that ImageMagick knows how
to open. Otherwise it can only read uncompressed BMP images.

If you want to run Cuneiform without installing it on your system, you
have to point the CF_DATADIR environment variable to a directory
containing the .dat files. These can be found in the "datafiles"
directory of the source package.


Running

After install you simply run.

cuneiform [-l language -o result_file --html --dotmatrix --fax] <image_file>

Output is written to pumaout.txt. Cuneiform assumes that your image contains
only a single column of text.

By default Cuneiform recognizes English text. To change the language use the
command line switch -l followed by your language string. To get a list of
supported languages type "cuneiform -l".

By default Cuneiform outputs plain text. You can specify the "--html" switch
to make it output in HTML format.

If you do not define an output file with the -o switch, Cuneiform
writes the result to a file "cuneiform-out.[format]". The file extension
is either "txt" or "html" depending on your output format.


Contact information

Project home page: https://launchpad.net/cuneiform-linux
Mailing list: https://launchpad.net/~cuneiform
