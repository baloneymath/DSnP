/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}
   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d) {
     _data.push_back(d);
     size_t t = _data.size() - 1;
     while (t > 0) {
       if (_data[(t-1)/2] < _data[t]) break;
       swap (_data[(t-1)/2], _data[t]);
       t = (t-1)/2;
     }
   }
   void delMin() {
     size_t p = 0;
     size_t t = _data.size() - 1;
     size_t c = 1;
     swap(_data[0], _data[t]);
     _data.resize(t);
     while (t > c) {
       if (c + 1 < t &&_data[c+1] < _data[c]) ++c;
       if (_data[p] < _data[c]) break;
       swap(_data[p], _data[c]);
       p = c;
       c = p*2 + 1;
     }
   }
   void delData(size_t i) {
     size_t p = i;
     while (p > 0) {
       swap(_data[p], _data[(p-1)/2]);
       p = (p-1)/2;
     }
     delMin();
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
