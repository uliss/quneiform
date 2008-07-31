# Copy all dat files to build directory's root.

file(GLOB datfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.dat)

foreach(i ${datfiles})
    install(FILES ${i} DESTINATION ${RELATIVE_DATADIR}) 
endforeach()
