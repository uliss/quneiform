add_library(cuneiform-alphabets ${LIBTYPE}
alphabetfactory.cpp
alphabet.cpp
baltic.cpp
bulgar.cpp
croatian.cpp
czech.cpp
danish.cpp
digits.cpp
dutch.cpp
easteurope.cpp
english.cpp
estonian.cpp
french.cpp
german.cpp
hungar.cpp
italian.cpp
kazakh.cpp
kazakhenglish.cpp
latvian.cpp
lithuanian.cpp
polish.cpp
portuguese.cpp
roman.cpp
russian.cpp
russianenglish.cpp
serbian.cpp
slovenian.cpp
spanish.cpp
swedish.cpp
turkish.cpp
ukrainian.cpp
uzbek.cpp
)

library_hook(cuneiform-alphabets)

set(ALPHABET_HEADER_INSTALL "alphabets")
install(FILES alphabetfactory.h alphabet.h DESTINATION include/${CF_HEADER_INSTALL}/${ALPHABET_HEADER_INSTALL})

target_link_libraries(cuneiform-alphabets 
    cuneiform-compat 
    cuneiform-common
)
