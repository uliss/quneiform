include_directories(../src)

add_executable(cf_accsum accsum.c)
add_executable(cf_accuracy accuracy.c)

target_link_libraries(cf_accsum ocrtk)
target_link_libraries(cf_accuracy ocrtk)