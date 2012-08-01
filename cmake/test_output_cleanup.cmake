function(delete_test_files files)
    foreach(file ${files})
        message(STATUS " Deleting \"${file}\"")
        execute_process(COMMAND ${CMAKE_COMMAND} -E remove "${file}")
    endforeach()
endfunction()

message("Deleting text files")
file(GLOB test_output ${CMAKE_BINARY_DIR}/*.text)
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.textdebug)
message("Deleting textdebug files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.smarttext)
message("Deleting smarttext files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.summary)
message("Deleting summary files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.diff)
message("Deleting diff files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.odf)
message("Deleting ODF files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.djvutxt)
message("Deleting Djvu text files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.djvuxml)
message("Deleting Djvu xml files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.native)
message("Deleting native files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.native-xml)
message("Deleting native-xml files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.html)
message("Deleting html files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.hocr)
message("Deleting hocr files")
delete_test_files("${test_output}")

file(GLOB test_output ${CMAKE_BINARY_DIR}/*.xml)
message("Deleting xml files")
delete_test_files("${test_output}")
