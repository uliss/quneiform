add_library(evn32 ${LIBTYPE}
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

set_property(TARGET evn32 PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET evn32 PROPERTY COMPILE_DEFINITIONS __EVN__)

target_link_libraries(evn32 dif32 cuneiform-cfcompat)

library_hook(evn32)
