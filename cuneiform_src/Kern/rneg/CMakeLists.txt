include_directories(BEFORE sources/src/h)

add_library(rneg ${LIBTYPE}
#SOURCE=..\..\Hdebug\__Snp.c
sources/src/negatest/negatestcell.cpp
sources/src/negmain/negdefs.cpp
sources/src/recog/negrecog.cpp
sources/src/negmain/recnegmain.cpp
sources/src/negmain/rnegbase.cpp
sources/src/searchneg/searchneg.cpp
)
