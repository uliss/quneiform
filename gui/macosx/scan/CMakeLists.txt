set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

set(RESOURCES ScanWindow)

set(SRC_LIST
    macscan.mm
    scanwindowcontroller.m
)

add_library(quneiform_mac_scan STATIC ${SRC_LIST})
target_link_libraries(quneiform_mac_scan "-framework Cocoa -framework Quartz")


# Make sure we can find the 'ibtool' program. If we can NOT find it we
# skip generation of this project
find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
if(${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
    message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with 
                    the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
endif()

# Compile the .xib files using the 'ibtool' program with the destination being the app package
foreach(xib ${RESOURCES})
  add_custom_command (TARGET quneiform_mac_scan POST_BUILD
                      COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text 
                              --compile ${CMAKE_BINARY_DIR}/${xib}.nib
                             ${CMAKE_CURRENT_SOURCE_DIR}/${xib}.xib
                      COMMENT "Compiling ${xib}.xib")

endforeach()


