include_directories(BEFORE
../hhh/tigerh/h/
../hhh/tigerh/
)

add_library(rselstr ${LIBTYPE}
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

set_property(TARGET rselstr PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rselstr PROPERTY COMPILE_DEFINITIONS __RSELSTR__)

target_link_libraries(rselstr
cuneiform-cimage
cpage
loc32
rimage
exc  
cline
cstr
rcutp
)

library_hook(rselstr)
