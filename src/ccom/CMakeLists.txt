set(CF_CCOM_SOURCE ccom.cpp ccom_loc.cpp)

add_definitions(-D__CCOM__) # Sets FUN_IMPO and FUN_EXPO properly.

add_library(${CF_CCOM} ${LIBTYPE} ${CF_CCOM_SOURCE})

cf_install_library(${CF_CCOM})
cf_cppcheck(${CF_CCOM})
