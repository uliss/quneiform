add_library(r35 ${LIBTYPE}
src/r35.c)

target_link_libraries(r35 mmx std32 windummy)