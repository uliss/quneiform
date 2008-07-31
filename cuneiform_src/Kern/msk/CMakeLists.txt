include_directories(src)

add_library(msk ${LIBTYPE} src/msk.c)

target_link_libraries(msk mmx windummy)

install(TARGETS msk LIBRARY DESTINATION lib)
