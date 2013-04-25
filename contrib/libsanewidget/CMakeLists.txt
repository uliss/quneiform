#project(sanewidget) # the name of your project
#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
find_package(Sane REQUIRED)
include(FindLibintl.cmake)

# tell cmake to process CMakeLists.txt in that subdirectory
add_subdirectory(src)
