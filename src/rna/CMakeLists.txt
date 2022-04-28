set (MODULE rna)

set (MODULE_SRC "")

list (APPEND MODULE_SRC "base.h")
list (APPEND MODULE_SRC "basepair.h")
list (APPEND MODULE_SRC "predictor.h")
list (APPEND MODULE_SRC "internal/include/base.h")
list (APPEND MODULE_SRC "internal/include/basepair.h")
list (APPEND MODULE_SRC "internal/include/predictor.h")
list (APPEND MODULE_SRC "internal/src/basepair.cpp")
list (APPEND MODULE_SRC "internal/src/predictor.cpp")

set (MODULE_SET_LANGUAGE ON)
set (MODULE_LANGUAGE "CXX")


set (MODULE_INCLUDE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include (module)