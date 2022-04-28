set (TEST_MODULE basepair)

set (MODULE_SRC "")
set (MODULE_DEPENDS "")

list (APPEND MODULE_SRC "basepair.test.cpp")

list (APPEND MODULE_DEPENDS rna)

include (testmodule)