#include "../memMgr.h"

template <class T> const size_t MemMgr<T>::_memSize[R] = {
   S,       S << 1,  S << 2,  S << 3,  S << 4,
   S << 5,  S << 6,  S << 7,  S << 8,  S << 9,
   S << 10, S << 11, S << 12, S << 13, S << 14,
   S << 15, S << 16, S << 17, S << 18, S << 19,
   S << 20, S << 21, S << 22, S << 23, S << 24,
   S << 25, S << 26, S << 27, S << 28, S << 29
};
