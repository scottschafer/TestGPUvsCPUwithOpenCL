#define kernel inline
#define global
#define get_global_id(X) X

#include "testStructs.cl"

#undef kernel
#undef global
#undef get_global_id
