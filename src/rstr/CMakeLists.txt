add_definitions(-DDPUMA_SNAP)

set(RSTR_SOURCES
    abris.cpp
    abrtools.cpp
    acc_tabs.cpp
    accents.cpp
    alcutfun.cpp
    alphabet.cpp
    atlant.cpp
    baton.cpp
    belarus.cpp
    bold.cpp
    cell.cpp
    celldump.cpp
    cg_hcut.cpp
    cg_main.cpp
    cg_smcut.cpp
    cg_tools.cpp
    compkit.cpp
    context.cpp
    contools.cpp
    corners.cpp
    cutgraph.cpp
    cutpoint.cpp
    defpoint.cpp
    diff.cpp
    differ.cpp
    diffr.cpp
    diffrang.cpp
    diffrb.cpp
    diffrv.cpp
    dm1.cpp
    dm2.cpp
    dmacc.cpp
    dmbox.cpp
    dms.cpp
    embbox.cpp
    erect2.cpp
    erection.cpp
    findiff.cpp
    fontprop.cpp
    fontser.cpp
    glue.cpp
    il1peak.cpp
    il1tg.cpp
    iot.cpp
    lang_fil.cpp
    lettab.cpp
    lineout.cpp
    match_wd.cpp
    mmx_addr.cpp
    p2_bold.cpp
    p2_cour.cpp
    pass3.cpp
    passe.cpp
    perc.cpp
    prop.cpp
    proport.cpp
    proptab.cpp
    punct.cpp
    rastror.cpp
    rcm.cpp
    rcontext.cpp
    rstr_bl.cpp
    rstr_con.cpp
    rstr_lik.cpp
    rstr_p2.cpp
    scalar.cpp
    shaving.cpp
    shevron.cpp
    skew.cpp
    snap.cpp
    space.cpp
    spec_sym.cpp
    st_tools.cpp
    stic_big.cpp
    stic_cha.cpp
    stic_def.cpp
    stic_dis.cpp
    stic_frt.cpp
    stic_mak.cpp
    sweeper.cpp
    tm.cpp
    tools.cpp
    ukr.cpp
)

set(RSTR_HEADERS)

add_library(cuneiform-rstr ${LIBTYPE}
    ${SNP_SRC}
    ${RSTR_SOURCES}
    ${RSTR_HEADERS}
)

cif_visibility_hidden(cuneiform-rstr)

set_property(TARGET cuneiform-rstr PROPERTY COMPILE_DEFINITIONS __RSTR__)

target_link_libraries(cuneiform-rstr
    cuneiform-ccom
    cuneiform-cline
    cuneiform-cstr
    cuneiform-ctb
    cuneiform-evn
    cuneiform-fon
    cuneiform-mmx
    cuneiform-msk
    cuneiform-pass2
    cuneiform-rbal
    cuneiform-rcutp
    cuneiform-rling
    cuneiform-rsadd
    cuneiform-std
    cuneiform-leo
)

cf_install_library(cuneiform-rstr)

if(TESTS)
    add_library(cuneiform-rstr-static STATIC
        ${RSTR_SOURCES}
    )

    target_link_libraries(cuneiform-rstr-static 
        cuneiform-rling
        cuneiform-cline
        cuneiform-leo
        cuneiform-rcutp
        cuneiform-pass2
        cuneiform-rsadd
        cuneiform-rbal
    )
endif()

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-rstr UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
