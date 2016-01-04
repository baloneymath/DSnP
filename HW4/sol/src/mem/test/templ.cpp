#include <iostream>

using namespace std;

template <class T, size_t S>
class Mem
{
   T* _t;
public:
   Mem() { cout << "size = " << S << endl; }
};

int
main()
{
   
}
