
set(RELATIVE_DATADIR "share/cuneiform")
set(INSTALL_DATADIR "${CMAKE_INSTALL_PREFIX}/${RELATIVE_DATADIR}")

include(CheckTypeSize)
check_type_size("void*" PTRSIZE)
if(PTRSIZE EQUAL 8 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIBDIR "lib64")
else()
    set(LIBDIR "lib")
endif()

macro(library_hook name)
  set_target_properties(${name} PROPERTIES VERSION
  "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")
  install(
    TARGETS "${name}"
    ARCHIVE DESTINATION ${LIBDIR}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBDIR}
  )
endmacro()

macro(install_exe name)
  install(
    TARGETS "${name}"
    RUNTIME DESTINATION bin
  )
endmacro()

include(CheckCXXCompilerFlag)
macro(cif_visibility_hidden name)
  CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden HAVE_GCC_VISIBILITY)
  if(HAVE_GCC_VISIBILITY)
      set_property(TARGET ${name} PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
  endif() 
endmacro()
