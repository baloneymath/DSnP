/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
  // TODO: design your own class!!
  friend class BSTree<T>;
  friend class BSTree<T>::iterator;

  BSTreeNode(const T& d, BSTreeNode<T>* p = NULL, BSTreeNode<T>* l = NULL, BSTreeNode<T>* r = NULL):
    _data(d), _parent(p), _left(l), _right(r) {}

  T               _data;
  BSTreeNode<T>*  _parent;
  BSTreeNode<T>*  _left;
  BSTreeNode<T>*  _right;
};


template <class T>
class BSTree
{
  // TODO: design your own class!!
  public:
    BSTree() {
      _root = new BSTreeNode<T>(T());
      _root->_parent = _root;
      _root->_left = _root->_right = NULL;
      _size = 0;
    }
    ~BSTree() { clear(); delete _root; }

    class iterator
    {
      friend class BSTree;

      public:
      iterator(BSTreeNode<T>* n = 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
        BSTreeNode<T>* temp = _node;
        if (_node->_right) {
          temp = _node->_right;
          while (temp->_left) temp = temp->_left;
          _node = temp;
        }
        else {
          while (temp->_parent->_right == temp) temp = temp->_parent;
          _node = temp->_parent;
        }
        return (*this);
      }
      iterator operator ++ (int) {
        iterator now = (*this);
        ++(*this);
        return now;
      }
      iterator& operator -- () {
        BSTreeNode<T>* temp = _node;
        if (_node->_left) {
          temp = _node->_left;
          while (temp->_right) temp = temp->_right;
          _node = temp;
        }
        else {
          while (temp->_parent->_left == temp) temp = temp->_parent;
          _node = temp->_parent;
        }
        return (*this);
      }
      iterator operator -- (int) {
        iterator now = (*this);
        --(*this);
        return now;
      }

      iterator& operator = (const iterator& i) { _node = i._node;  return (*this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

      private:
      BSTreeNode<T>*  _node;
    };

    iterator begin() const {
      BSTreeNode<T>* temp = _root;
      while (temp->_left) temp = temp->_left;
      return iterator(temp);
    }
    iterator end() const { return iterator(_root); }
    bool empty() { return _size == 0; }
    size_t size() const { return _size; }

    void insert(const T& x) {
      if (_size == 0) _root->_left = new BSTreeNode<T>(x, _root);
      else {
        BSTreeNode<T>* temp = _root->_left;
        while (1) {
          if (compare(x, temp->_data)) {
            if (temp->_left == NULL) {
              temp->_left = new BSTreeNode<T>(x, temp);
              break;
            }
            else temp = temp->_left;
          }
          else {
            if (temp->_right == NULL) {
              temp->_right = new BSTreeNode<T>(x, temp);
              break;
            }
            else temp = temp->_right;
          }
        }
      }
      ++_size;
    }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }
    bool erase(iterator pos) {
      if (empty()) return false;
      BSTreeNode<T>* node = pos._node;
      if (node->_right == NULL) {
        if (node->_left) node->_left->_parent = node->_parent;
        if (node->_parent->_left == node) node->_parent->_left = node->_left;
        else node->_parent->_right = node->_left;
        delete node;
        --_size;
        return true;
      }
      else {
        iterator temp = pos;
        ++temp;
        swap(*pos, *temp);
        return erase(temp);
      }
    }
    bool erase(const T& x) {
      iterator pos = find(x);
      if (pos != end()) {
        erase(pos);
        return true;
      }
      return false;
    }
    void __clear(BSTreeNode<T>* node){
      if (node == NULL) return ;
      __clear(node->_left);
      __clear(node->_right);
      delete node;
    }
    void clear() {
      __clear(_root->_left);
      _root->_left = NULL;
      _size = 0;
    }

    void sort() const {}
    void print() const {}

  private:
    BSTreeNode<T>*  _root;
    size_t          _size;

    bool compare(const T& t1,const T& t2) {
      return !(t2 < t1);
    }

    iterator find(const T& x) {
      BSTreeNode<T>* node = _root->_left;
      while (node) {
        if (x == node->_data) return iterator(node);
        else if (compare(x, node->_data)) node = node->_left;
        else node = node->_right;
      }
      return end();
    }
};

#endif // BST_H
