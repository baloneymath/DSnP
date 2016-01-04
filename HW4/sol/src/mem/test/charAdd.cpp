#include <iostream>

using namespace std;

//typedef char TT;
typedef int TT;

int
main()
{
   TT* p = new TT[65536];
   cout << (void*)p << endl;

   size_t t;
   cin >> t;
   TT* q = p + t;
   cout << (void*)q << endl;

   size_t s = size_t(q - p);
   cout << s << endl;

   TT *r = q - s;
   cout << (void*)r << endl;
}
