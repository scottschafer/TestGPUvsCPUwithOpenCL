/**
 * This file includes the gpuOrganism.cl kernel code with enough preprocessor munging to make it valid C++.
 *
 * It's not general purpose (for that, https://github.com/triSYCL/triSYCL would be a better bet), but it works in this limited case. 
 */

#define kernel inline
#define global
#define get_global_id(X) X

#include "gpuOrganism.cl"

#undef kernel
#undef global
#undef get_global_id
