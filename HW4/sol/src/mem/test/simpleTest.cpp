#include <iostream>
#include "../memMgr.h"
//#include "memMgr.cpp"

// #include "myUsage.h"

/*
template <class T> const size_t MemMgr<T>::_memSize[R] = {
   S,       S << 1,  S << 2,  S << 3,  S << 4, 
   S << 5,  S << 6,  S << 7,  S << 8,  S << 9, 
   S << 10, S << 11, S << 12, S << 13, S << 14,
   S << 15, S << 16, S << 17, S << 18, S << 19,
   S << 20, S << 21, S << 22, S << 23, S << 24,
   S << 25, S << 26, S << 27, S << 28, S << 29 
};
*/

using namespace std;

class A
{
#ifdef MEM_MGR_H
   USE_MEM_MGR(A);
#endif // MEM_MGR_H

public:
   A() {}
   ~A() {}

private:
   int   _a[8];
//   int   _b;
//   int   _c;
   char  _d[2];
};

#ifdef MEM_MGR_H
MEM_MGR_INIT(A);
#endif // MEM_MGR_H

//#define MEM_DEBUG

int main()
{
//   MyUsage u;

   #ifdef MEM_DEBUG
   cout << "size of A = " << sizeof(A) << endl;
   #endif // MEM_DEBUG

   for (size_t i = 0; i < 3; ++i) {
      A* p = new A;
      #ifdef MEM_DEBUG
      cout << ">> p   = " << p << endl;
      #endif // MEM_DEBUG
      delete p;
 
      A* a = new A[5];
      #ifdef MEM_DEBUG
      cout << ">> a   = " << a << endl;
      cout << ">> a+1 = " << a+1 << endl;
      cout << ">> a+2 = " << a+2 << endl;
      cout << endl << endl;
      #endif // MEM_DEBUG
      delete []a;

      A* b = new A[10];
      #ifdef MEM_DEBUG
      cout << ">> b   = " << b << endl;
      cout << ">> b+1 = " << b+1 << endl;
      cout << ">> b+2 = " << b+2 << endl;
      cout << endl << endl;
      #endif // MEM_DEBUG
      delete []b;

      A* c = new A[20];
      #ifdef MEM_DEBUG
      cout << ">> c   = " << c << endl;
      cout << ">> c+1 = " << c+1 << endl;
      cout << ">> c+2 = " << c+2 << endl;
      cout << endl << endl;
      #endif // MEM_DEBUG
      delete []c;

      #ifdef MEM_MGR_H
      A::memReset();
      #endif // MEM_MGR_H

//      u.report();
   }
}
