set (TEST_MODULE predictor)

set (MODULE_SRC "")
set (MODULE_DEPENDS "")

list (APPEND MODULE_SRC "predictor.test.cpp")

list (APPEND MODULE_DEPENDS rna)

include (testmodule)