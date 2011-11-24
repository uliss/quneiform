add_definitions(-DBOOST_DATE_TIME_NO_LIB)

set(CUNEIFORM_SOURCES
    localrecognitionserver.cpp
    processrecognitionserver.cpp
    pumaimpl.cpp
    recognitionfactory.cpp
    startprocess.cpp
)

if(UNIX)
    list(APPEND CUNEIFORM_SOURCES processtimeoutkiller.cpp)
endif()

set(CUNEIFORM_HEADERS
    abstractrecognitionserver.h
    puma.h
    shmem_data.h
)

set(CUNEIFORM_LIBS
    cuneiform-load
    cuneiform-common
    cuneiform-ced
    cuneiform-cimage
    cuneiform-cline
    cuneiform-cpage
    cuneiform-export
    cuneiform-markup
    cuneiform-rblock
    cuneiform-rcorrkegl
    cuneiform-rfrmt
    cuneiform-rimage
    cuneiform-rline
    cuneiform-rpic
    cuneiform-rselstr
    cuneiform-rsl
    cuneiform-rstuff
    cuneiform-exc
    cuneiform-rreccom
    cuneiform-rpstr
    cuneiform-shmem
)

if(CF_LIB_RT_HAS_SHMOPEN)
    list(APPEND CUNEIFORM_LIBS rt)
endif()

find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
    if(NOT WIN32)
        list(APPEND CUNEIFORM_LIBS pthread)
    endif()
endif()

add_library(cuneiform ${CF_CUNEIFORM_LIBTYPE}
    ${CUNEIFORM_SOURCES}
    ${CUNEIFORM_HEADERS}
    ${SNP_SRC}
)

target_link_libraries(cuneiform ${CUNEIFORM_LIBS})
library_hook(cuneiform)

add_executable(cuneiform-worker worker.cpp)
target_link_libraries(cuneiform-worker cuneiform)

if(WIN32)
    set(WORKER_INSTALL_DIR bin)
else()
    set(WORKER_INSTALL_DIR lib/cuneiform)
endif()

install(TARGETS cuneiform-worker
    RUNTIME DESTINATION ${WORKER_INSTALL_DIR}
    COMPONENT applications
)

install(
    FILES
        abstractrecognitionserver.h localrecognitionserver.h
    DESTINATION
        include/${CF_HEADER_INSTALL}/puma)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
