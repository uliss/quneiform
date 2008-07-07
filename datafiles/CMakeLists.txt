# Copy all dat files to build directory's root.

file(GLOB datfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.dat)

foreach(i ${datfiles})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${i} ${PROJECT_BINARY_DIR} COPYONLY)
endforeach()
