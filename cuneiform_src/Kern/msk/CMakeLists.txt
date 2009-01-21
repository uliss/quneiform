include_directories(src)

add_definitions(-D__MSK__)

add_library(msk ${LIBTYPE} src/msk.c)

target_link_libraries(msk mmx windummy)

install_dll(msk)
