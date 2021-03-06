/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator temp = *(this); _node = _node->_next; return temp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator temp = *(this); _node = _node->_prev; return temp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_head->_next); }
   iterator end() const { return iterator(_head); }
   bool empty() const {
     return _head->_prev == _head;
   }
   size_t size() const {
     size_t num = 0;
     for (iterator it = begin(); it != end(); ++it) num++;
     return num;
   }

   void push_back(const T& x) {
     DListNode<T>* temp = new DListNode<T>(x, _head->_prev, _head);
     _head->_prev->_next = temp;
     _head->_prev = temp;
   }
   void pop_front() {
     _head->_next = _head->_next->_next;
     _head->_next->_next->_prev = _head;
     delete _head->_next->_next;
     delete _head->_next->_prev;
   }
   void pop_back() {
     _head->_prev = _head->_prev->_prev;
     _head->_prev->_prev->_next = _head;
     delete _head->_prev->_prev;
     delete _head->_prev->_next;
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
     if (empty()) return false;
     pos._node->_prev->_next = pos._node->_next;
     pos._node->_next->_prev = pos._node->_prev;
     delete pos._node;
     return true;
   }
   bool erase(const T& x) {
     for (iterator it = begin(); it != end(); ++it)
       if (it._node->_data == x) {
         erase(it);
         return true;
       }
     return false;
   }

   void clear() {
     for (iterator it = begin(); it != end(); ++it)
       delete it._node;
     _head->_prev = _head->_next = _head;
   }  // delete all nodes except for the dummy node

   void sort() const {
     for (iterator it = begin(); it != end(); ++it)
       for (iterator jt = it; jt != end(); ++jt)
         if (jt._node->_data < it._node->_data) {
           swap(it._node->_data, jt._node->_data);
         }
   }

private:
   DListNode<T>*  _head;  // = dummy node if list is empty

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
