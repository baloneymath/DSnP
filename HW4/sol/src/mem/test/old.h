/****************************************************************************
  FileName     [ memMgr.h ]
  PackageName  [ ]
  Synopsis     [ Define Memory Manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2007 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MEM_MGR_H
#define MEM_MGR_H

#include <iostream>
#include <stdlib.h>

using namespace std;

//--------------------------------------------------------------------------
// Define MACROs
//--------------------------------------------------------------------------
#define DEFINE_MEM_MGR(T,N)                                                 \
public:                                                                     \
   void* operator new(size_t t) { return (void*)(_memMgr->alloc(t)); }      \
   void* operator new[](size_t t) { return (void*)(_memMgr->allocArr(t)); } \
   void  operator delete(void* p) { _memMgr->free((T*)p); }                 \
   void  operator delete[](void* p) { _memMgr->freeArr((T*)p); }            \
   static void memReset() { _memMgr->reset(); }                             \
private:                                                                    \
   static MemoryMgr<T, N>* const _memMgr

#define SIZE_T   sizeof(size_t)
#define SIZE_T_1 (sizeof(size_t) - 1)

//--------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------
template <class T, size_t N> class MemoryMgr;


//--------------------------------------------------------------------------
// Class Definitions
//--------------------------------------------------------------------------
// T is the class that use this memory manager
// N is number of T elements in each memory block
// S is the size of each element
//
// Make it a private class;
// Only friend to MemoryMgr;
//
template <class T, size_t N, size_t S>
class MemoryBlock
{
   friend class MemoryMgr<T,N>;

   // Constructor/Destructor
   MemoryBlock(MemoryBlock<T, N, S>* n) : _nextBlock(n) {
      _begin = (T*)malloc(S * N); }
   ~MemoryBlock() { free(_begin); }

   // Member functions
   T* getElm(size_t i) const { return (_begin + i); }
   MemoryBlock<T, N, S>* getNextBlock() const { return _nextBlock; }

   // Data members
   T*                       _begin;
   MemoryBlock<T, N, S>*    _nextBlock;
};

// Make it a private class;
// Only friend to MemoryMgr;
//
template <class T, size_t N>
class RecycleList
{
   friend class MemoryMgr<T, N>;

   // Constructor/Destructor
   RecycleList() : _first(0) {}
   ~RecycleList() {}

   // Member functions
   T* popFront() {
      if (!_first) return 0;
      T* ret = _first;
      _first = (T*)(*(size_t*)ret);
      return ret;
   }
   void  pushFront(T* p) {
      *(size_t*)p = size_t(_first);
      _first = p;
   }
   
   void reset() { _first = 0; }

   // Data members
   T*           _first;
};

template <class T, size_t N>
class MemoryMgr
{
   // S is the size of each element; make it multiple of SIZE_T
   #define S ((sizeof(T) + SIZE_T_1) / SIZE_T * SIZE_T)

   // Private member functions
   //
   // n is the number of elements to claim from memory block
   T* getMem(size_t n) {
      // [TODO]
      // Check the _recycleList first...
      // n may change... depends on your recycling scheme...
      //
      if ((_index + n) > N) {
         _index = 0;
         _activeBlock = new MemoryBlock<T, N, S>(_activeBlock);
      }
      T* ret = _activeBlock->getElm(_index);
      _index += n;
      #ifdef MEM_DEBUG
      cout << ">> ptr  = " << ret << endl;
      #endif // MEM_DEBUG
      return ret;
   }
   // n is the number of elements to claim from memory block
   size_t getRecycleIndex(size_t n) {
      // [TODO]
      // Not done yet...
      return 0;
   }

public:
   // Make _index = N in the beginning ==> illegal index
   MemoryMgr() : _index(N), _activeBlock(0) {
      // Initialize _recycleList here...
      // _recycleList = new RecycleList<T, N>[...];
   }
   ~MemoryMgr() {}

   void reset() {
      while (_activeBlock) {
         MemoryBlock<T, N, S>* nextBlock = _activeBlock->getNextBlock();
         delete _activeBlock;
         _activeBlock = nextBlock;
      }
      _index = N;
   }
   T* alloc(size_t t) {
      #ifdef MEM_DEBUG
      cout << "Calling alloc...(" << t << ")" << endl;
      #endif // MEM_DEBUG
      assert(t == sizeof(T));
      return getMem(1);
   }
   T* allocArr(size_t t) {
      #ifdef MEM_DEBUG
      cout << "Calling allocArr...(" << t << ")" << endl;
      #endif // MEM_DEBUG
      size_t n = (t - SIZE_T)/sizeof(T) + 1;
      #ifdef MEM_DEBUG
      cout << ">> size = " << n << endl;
      #endif // MEM_DEBUG
      T* p = getMem(n);
      T* ret = (T*)(size_t(p) + S - SIZE_T);
      #ifdef MEM_DEBUG 
      cout << ">> ret  = " << ret << endl;
      #endif // MEM_DEBUG
      return ret;
   }
   void  free(T* p) {
      #ifdef MEM_DEBUG
      cout << "Calling free...(" << p << ")" << endl;
      #endif // MEM_DEBUG
   }
   void  freeArr(T* p) {
      #ifdef MEM_DEBUG
      cout << "Calling freeArr...(" << p << ")" << endl;
      #endif // MEM_DEBUG
      #ifdef MEM_DEBUG
      size_t n = *(size_t *)p;
      cout << ">> size = " << n << endl;
      #endif // MEM_DEBUG
   }

private:
   size_t                   _index;
   MemoryBlock<T, N, S>*    _activeBlock;
   RecycleList<T, N>*       _recycleList;
};

#endif // MEM_MGR_H
