#include <iostream>

using namespace std;

#define SIZE_T   sizeof(size_t)
#define SIZE_T_1 (sizeof(size_t) - 1)

template <class T>
class MemMgr
{
   // S is the size of each element; make it multiple of SIZE_T
   #define S ((sizeof(T) + SIZE_T_1) / SIZE_T * SIZE_T)

   static const size_t _memSize[30];

public:
   MemMgr() { cout << "Basic size = " << S << endl; }

   size_t getSize(size_t t) const {
      if (t <= _memSize[0]) return 0;
      if (t <= _memSize[2]) return (t <= _memSize[1])? 1: 2;
      if (t <= _memSize[6]) {
         if (t <= _memSize[4]) return (t <= _memSize[3])? 3: 4;
         return (t <= _memSize[5])? 5: 6;
      }
      if (t <= _memSize[14]) {
         if (t <= _memSize[10]) {
            if (t <= _memSize[8]) return (t <= _memSize[7])? 7: 8;
            return (t <= _memSize[9])? 9: 10;
         }
         else {
            if (t <= _memSize[12]) return (t <= _memSize[11])? 11: 12;
            return (t <= _memSize[13])? 13: 14;
         }
      }
      if (t<= _memSize[22]) {
         if (t <= _memSize[18]) {
            if (t <= _memSize[16]) return (t <= _memSize[15])? 15: 16;
            return (t <= _memSize[17])? 17: 18;
         }
         else {
            if (t <= _memSize[20]) return (t <= _memSize[19])? 19: 20;
            return (t <= _memSize[21])? 21: 22;
         }
      }
      else {

         if (t <= _memSize[26]) {
            if (t <= _memSize[24]) return (t <= _memSize[23])? 23: 24;
            return (t <= _memSize[25])? 25: 26;
         }
         else {
            if (t <= _memSize[28]) return (t <= _memSize[27])? 27: 28;
            return 29;
         }
      }
   }

   size_t getMem(size_t t) const { return _memSize[t]; }

};

template <class T> const size_t MemMgr<T>::_memSize[30] = {
   S,       S << 1,  S << 2,  S << 3,  S << 4,
   S << 5,  S << 6,  S << 7,  S << 8,  S << 9,
   S << 10, S << 11, S << 12, S << 13, S << 14,
   S << 15, S << 16, S << 17, S << 18, S << 19,
   S << 20, S << 21, S << 22, S << 23, S << 24,
   S << 25, S << 26, S << 27, S << 28, S << 29
};

int
main()
{
   MemMgr<unsigned> mm;

   size_t t;
   while (1) {
      cin >> t;
      size_t s = mm.getSize(t);
      cout << "_memSize[" << s << "] = " << mm.getMem(s) << endl;
   }
}
