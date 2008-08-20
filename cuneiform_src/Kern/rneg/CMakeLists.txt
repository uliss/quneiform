include_directories(BEFORE sources/src/h)

add_library(rneg ${LIBTYPE}
${SNP_SRC}
sources/src/negatest/negatestcell.cpp
sources/src/negmain/negdefs.cpp
sources/src/recog/negrecog.cpp
sources/src/negmain/recnegmain.cpp
sources/src/negmain/rnegbase.cpp
sources/src/searchneg/searchneg.cpp
)

target_link_libraries(rneg
cimage
cpage
rdib
rimage
rselstr
exc
rstr
)

install(TARGETS rneg LIBRARY DESTINATION ${LIBDIR})
