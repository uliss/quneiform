add_library(msk32 ${LIBTYPE} msk.cpp)
set_property(TARGET msk32 PROPERTY COMPILE_DEFINITIONS __MSK__)

target_link_libraries(msk32 mmx32 cuneiform-compat)

library_hook(msk32)
