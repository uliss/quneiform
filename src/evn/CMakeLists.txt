add_library(cuneiform-evn ${LIBTYPE}
compdata.cpp
compkit.cpp
evn.cpp
linepool.cpp
locompmn.cpp
rastror.cpp
v0compan.cpp
v0comper.cpp
v0compev.cpp
v0compgl.cpp
v0comprq.cpp
)

if(${CMAKE_COMPILER_IS_GNUCXX} AND NOT MINGW)
  set_property(TARGET cuneiform-evn PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
endif() 
set_property(TARGET cuneiform-evn PROPERTY COMPILE_DEFINITIONS __EVN__)

target_link_libraries(cuneiform-evn 
    cuneiform-dif 
    cuneiform-compat) 

library_hook(cuneiform-evn)
