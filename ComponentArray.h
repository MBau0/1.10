#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include <cassert>
#include <cstdint>

#define INDEX_DELETED -1
#define INDEX_FREE 0
#define INDEX_TAKEN 1

template <typename _Component>
class ComponentArray {
public:
   ComponentArray(size_t size) :
        _size(size),
        _back(0)
    {
        _components = new _Component[size] { 0 };
        _indices = new int8_t[size] { INDEX_FREE };
    }

   ~ComponentArray() {
       delete[] _components;
       delete[] _indices;
   }

   _Component* get() {
       assert(_back != _size);
       _indices[_back] = INDEX_TAKEN;
       return &_components[_back++];
   }

   void remove(_Component* component) {
       for(size_t i = _back; i >= 0; --i) {
           if(_components[i] == component) {
               _indices[i] = INDEX_DELETED;
           }
       }
   }

   struct Iterator {
       _Component operator*() {
           return *_ptr;
       }

       _Component* operator->() {
           return _ptr;
       }

       Iterator& operator++() {
           do {
               ++_ptr; 
               ++_index;
           } while(_index < _component_array->_back && _component_array->_indices[_index] != INDEX_TAKEN);

           return *this;
       }

       Iterator operator++(int) {
           auto temp = *this;
           do {
               ++_ptr;
               ++_index;
           } while(_index < _component_array->_back && _component_array->_indices[_index] != INDEX_TAKEN);
           return temp;
       }

       friend bool operator==(const Iterator& a, const Iterator& b) {
           return (a._ptr == b._ptr) && (a._index == b._index);
       }

       friend bool operator!=(const Iterator& a, const Iterator& b) {
           return (a._ptr != b._ptr) && (a._index != b._index);
       }

       _Component* _ptr = nullptr;
       size_t      _index = -1;
       ComponentArray<_Component>* _component_array;
   };

   Iterator begin() {
       Iterator it;
       it._ptr = &_components[0];
       it._index = 0;
       it._component_array = this;
       return it;
   }

   Iterator end() {
       Iterator it;
       it._ptr = &_components[_back];
       it._index = _back;
       it._component_array = this;
       return it;
   }

private:
    _Component* _components;
    int8_t* _indices;
    size_t _back;
    size_t _size;
};

#endif