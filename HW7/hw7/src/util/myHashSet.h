/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ()" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
      iterator(size_t nB=0, size_t nI=0, HashSet<Data> const * hs=NULL):
        _nBuckets(nB), _nIdx(nI), _hs(hs) {}
      iterator(const iterator& i):
        _nBuckets(i._nBuckets), _nIdx(i._nIdx), _hs(i._hs) {}
      ~iterator() {}

      const Data& operator * () const { return _hs->_buckets[_nBuckets][_nIdx]; }
      Data& operator * () { return _hs->_buckets[_nBuckets][_nIdx]; }

      iterator& operator ++ () {
        if (_nIdx == _hs->_buckets[_nBuckets].size() - 1) {
          if (_nBuckets == _hs->_numBuckets - 1) _nBuckets++;
          else {
            _nBuckets++;
            while (_nBuckets < _hs->_numBuckets && _hs->_buckets[_nBuckets].empty()) ++_nBuckets;
          }
          _nIdx = 0;
        }
        else _nIdx++;
        return (*this);
      }
      iterator operator ++ (int) {
        iterator temp = (*this);
        ++(*this);
        return temp;
      }
      iterator& operator -- () {
        if (_nIdx == 0) {
          if (_nBuckets == _hs->_numBuckets) _nBuckets--;
          else {
            _nBuckets--;
            while (_nBuckets > 0 && _hs->_buckets[_nBuckets].empty()) --_nBuckets;
          }
          _nIdx = _hs->_buckets[_nBuckets].size() - 1;
        }
        else _nIdx--;
        return (*this);
      }
      iterator operator -- (int) {
        iterator temp = (*this);
        --(*this);
        return temp;
      }

      iterator& operator = (const iterator& i) { _nBuckets = i._nBuckets; _nIdx = i._nIdx; _hs = i._hs; return (*this); }

      bool operator != (const iterator& i) const { return !(_nBuckets == i._nBuckets && _nIdx == i._nIdx); }
      bool operator == (const iterator& i) const { return (_nBuckets == i._nBuckets && _nIdx == i._nIdx); }

   private:
     size_t _nBuckets, _nIdx;
     const HashSet<Data>* _hs;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const
   {  for (int i = 0; i < _numBuckets; ++i)
        if (!_buckets[i].empty()) 
          return iterator(i, 0, this);
      return end();
   }
   // Pass the end
   iterator end() const {
     return iterator(_numBuckets, 0, this);
   }
   // return true if no valid data
   bool empty() const {
     return (_numBuckets == 0)? true : false;
   }
   // number of valid data
   size_t size() const {
     size_t s = 0;
     for (size_t i = 0; i < _numBuckets; ++i)
       s += _buckets[i].size();
     return s;
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const {
     vector<Data>& temp = _buckets[bucketNum(d)];
     for (size_t i = 0; i < temp.size(); ++i)
       if (d == temp[i]) return true;
     return false;
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const {
     vector<Data>& temp = _buckets[bucketNum(d)];
     for (size_t i = 0; i < temp.size(); ++i)
       if (d == temp[i]) { d = temp[i]; return true; }
     return false;
   }

   // update the entry in hash that is equal to d
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) {
     bool flag = false;
     vector<Data>& temp = _buckets[bucketNum(d)];
     for (size_t i = 0; i < temp.size(); ++i)
       if (d == temp[i]) { temp[i] = d; flag = true; }
     if (!flag) temp.push_back(d);
     return flag;
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) {
     vector<Data>& temp = _buckets[bucketNum(d)];
     if (!check(d)) { temp.push_back(d); return true; }
     return false;
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return false otherwise (i.e. nothing is removed)
   bool remove(const Data& d) {
     vector<Data>& temp = _buckets[bucketNum(d)];
     for (size_t i = 0; i < temp.size(); ++i)
       if (d == temp[i]) 
       { temp.erase(temp.begin() + i); return true; }
     return false;
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
