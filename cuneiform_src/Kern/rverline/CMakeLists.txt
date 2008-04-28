include_directories(BEFORE src/h src/usage)

add_library(rverline ${LIBTYPE}
${SNP_SRC}
src/usage/amu_geom.cpp
src/algoritm/by_image.cpp
src/usage_puma/pum_comm.cpp
src/algoritm/verifyln.cpp
src/root/vl_kern.cpp
src/root/vl_resid.cpp
src/service/vl_data.cpp
src/service/vl_rule.cpp
src/service/vl_snap.cpp
)