include_directories(BEFORE
../hhh/tigerh/h/
../hhh/tigerh/
)

add_library(cuneiform-rselstr ${LIBTYPE}
${SNP_SRC}
_adaptive.cpp
_dlldummy.cpp
_loadroots.cpp
chstr_cutstr.cpp
chstr_lhstr.cpp
compdata.cpp
cutcomp.cpp
cutstr.cpp
graphics.cpp
ltblocks.cpp
ltbreak.cpp
lterrors.cpp
lthyst.cpp
ltinclin.cpp
ltmain.cpp
ltroots.cpp
puttocont.cpp
puttocstr.cpp
qsort.cpp
rotate.cpp
rselstrbase.cpp
rselstrmain.cpp
scalar.cpp
seblocks.cpp
seform.cpp
sehbreak.cpp
sehyst.cpp
semain.cpp
sestring.cpp
sevbreak.cpp
testforvertical.cpp
)

if(${CMAKE_COMPILER_IS_GNUCXX} AND NOT MINGW)
  set_property(TARGET cuneiform-rselstr PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
endif()

set_property(TARGET cuneiform-rselstr PROPERTY COMPILE_DEFINITIONS __RSELSTR__)

target_link_libraries(cuneiform-rselstr
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-loc
    cuneiform-rimage
    cuneiform-exc  
    cuneiform-cline
    cuneiform-cstr
    cuneiform-rcutp
)

library_hook(cuneiform-rselstr)
