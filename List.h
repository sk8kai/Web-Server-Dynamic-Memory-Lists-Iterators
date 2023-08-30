#ifndef LIST_H
#define LIST_H
/* List.h
 *
 * doubly-linked, double-ended list with Iterator interface
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include <iostream>
#include <cassert> //assert
#include <cstddef> //NULL


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  //EFFECTS:  returns true if the list is empty
    bool empty() const {
        /*
        if (first) {
            return false;
        }
        else if (last) {
            return false;
        }
        else {
            return true;
        }
        */
        if (sizeList <= 0) {
            return true;
        }
        else {
            return false;
        }
    }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow.  Instead, keep track of the size
  //         with a private member variable.  That's how std::list does it.
    int size() const {
        //implement
        //int count = 0;
        //for (Iterator i = begin(); i != end(); ++i) {
          //  count++;
        //}
        
        return sizeList;
    }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
    T & front() {
        return first->datum;
    }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
    T & back() {
        return last->datum;
    }

  //EFFECTS:  inserts datum into the front of the list
    void push_front(const T &datum) {
        Node *p = new Node;
        p->datum = datum;
        p->next = first;
        p->prev = nullptr;
        if (empty()) {
            first = last = p;
        }
        else {
            first->prev = p;
            first = p;
        }
        sizeList++;
    }
    

  //EFFECTS:  inserts datum into the back of the list
    void push_back(const T &datum) {
        Node *p = new Node;
        p->datum = datum;
        p->next = nullptr;
        p->prev = last;
        if (empty()) {
            first = last = p;
        }
        else {
            last->next = p;
            last = p;
        }
        sizeList++;
    }

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the front of the list
    void pop_front() {
        assert(!empty());
        Node *victim = first;
        if (sizeList > 1) {
            first = first->next;
        }
        delete victim;
        --sizeList;
        if (sizeList == 1) {
            first->prev = nullptr;
        }
        if (empty()) {
            first = nullptr;
            last = nullptr;
        }
    }

    
    
  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the back of the list
    void pop_back() {
        assert(!empty());
        Node *victim = last;
        if (sizeList == 1) {
            delete victim;
            first = nullptr;
            last = nullptr;
        }
        else {
            last = last->prev;
            delete victim;

        }
        --sizeList;
    }

  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes all items from the list
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you can omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists
    
    ~List() {
        clear();
    }
    
    List() : sizeList(0), first(nullptr), last(nullptr) { }
    
    List(const List &other) : sizeList(0),first(other.first), last(nullptr) {
        copy_all(other);
    }
    
    List & operator=(const List &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        copy_all(other);
        return *this;
    }
    
private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };
    
    int sizeList;

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
    void copy_all(const List<T> &other) {
        for (Node *x = other.first; x; x = x->next) {
            push_back(x->datum);
            
            /*
            if (x == other.first) {
                first = x;
            }
            else if (!x->next) {
                last = x;
            }
             */
        }
    }

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty

public:
  ////////////////////////////////////////
  class Iterator {
      friend class List;
    //OVERVIEW: Iterator interface to List

    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Iterators.

    // Your iterator should implement the following public operators: *,
    // ++ (prefix), default constructor, == and !=.

  public:
    // This operator will be used to test your code. Do not modify it.
    // Requires that the current element is dereferenceable.
    Iterator () : node_ptr(nullptr) { }
      
    Iterator& operator--() {
      assert(node_ptr);
        //i wanna code this so if its end aka null ptr, itll go to the last one
    
      node_ptr = node_ptr->prev;
        
      return *this;
    }
      
    T & operator*() const {
        assert(node_ptr);
        return node_ptr->datum;
    }
      
    Iterator & operator++() {             //REQUIRED ON THE PROJECT , specifically ++it
        assert(node_ptr);
        node_ptr = node_ptr->next;
        return *this;
    }
      
    bool operator==(Iterator rhs) const {
        return node_ptr == rhs.node_ptr;
    }
      
    bool operator!=(Iterator rhs) const {
        return !(node_ptr == rhs.node_ptr);
    }
  

  private:
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here

    // add any friend declarations here

    // construct an Iterator at a specific position
    Iterator(Node *p) {
          node_ptr = p;
      }

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(first);
  }

  // return an Iterator pointing to "past the end"
    Iterator end() const {
        return Iterator(nullptr);  //had this originally, but it makes B false
        //return Iterator(last->next);  //this is showing error
    }

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: may invalidate other list iterators
  //EFFECTS: Removes a single element from the list container
    void erase(Iterator i) {
        if (sizeList == 1 || i == begin()) {
            pop_front();
        }
        else if (i.node_ptr == last) {
            pop_back();
        }
        else {
            Node *bef = i.node_ptr->prev;
            bef->next = i.node_ptr->next;
            Node *aft = i.node_ptr->next;
            aft->prev = i.node_ptr->prev;
            delete i.node_ptr;
            --sizeList;
        //needs testing
        }
    }

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: inserts datum before the element at the specified position.
    void insert(Iterator i, const T &datum) {
        if (empty() || i == begin()) {
            push_front(datum);
            /*
            first = i.node_ptr;
            first->datum = datum;
            first->prev = nullptr;
            first->next = nullptr;
            first = last;
             */
        }
        else if (i == end()) {      //if this is a nullptr, shouold be a pusback
            
            push_back(datum);
        }
        //
        else {
            Node *bef = i.node_ptr->prev;
            Node *n = new Node;
            n->datum = datum;
            n->prev = bef;
            n->next = i.node_ptr;
        
            bef->next = n;
            i.node_ptr->prev = n;
            ++sizeList;
        }
    }
};//List


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.
/*
template<typename T>
bool List<T>::empty() const {
  assert(false);
}

template<typename T>
int List<T>::size() const {
    assert(false);
}
 */
//actually retarded way of doing it
//forced us to require template bs every single time




#endif // Do not remove this. Write all your code above this line.
