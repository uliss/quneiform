add_definitions(-DEXPORTER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

if(USE_IMAGE_MAGICK)
    set(magick_test testmagickimageexporter.cpp)
endif()

if(CF_USE_ODF)
    set(odf_test testodfexporter.cpp)
    set(odf_libs ${ZIP_LIBRARIES})
endif()

add_executable(test_exporters
    test_exporters.cpp
    testbmpimageexporter.cpp
    testgenericexporter.cpp
    testhtmlexporter.cpp
    ${odf_test}
    ${magick_test}
)

target_link_libraries(test_exporters 
    cuneiform-common 
    cuneiform-ced 
    cuneiform-rfrmt 
    cuneiform-export
    ${ZIP_LIBRARIES}
    ${CPPUNIT_LIBRARIES})

add_test(Exporters ${EXECUTABLE_OUTPUT_PATH}/test_exporters)