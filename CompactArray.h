#ifndef COMPACT_ARRAY_H
#define COMPACT_ARRAY_H

#include <cstdint>
#include <vector>
#include <cassert>
#include <limits>

template <typename T>
class CompactArray {
public:
	CompactArray(uint16_t capacity ) :
		_capacity	( capacity ),
		_size		( 0 ),
		_back		( 0 )
	{
		_data = new T[capacity];
		_indicies.push_back(0);
	}

	~CompactArray() {
		delete[] _data;
	}

	CompactArray(const CompactArray&& rhs) = delete;

	CompactArray& operator=(const CompactArray& rhs) = delete;

	 T* get() {
		 assert(_back != 65535); // max value of an index 16 bits , std::numeric_limits was giving errors so i just hardcoded
		 uint16_t index = _indicies.back();
		 _indicies.pop_back();
		 if (index == _back) {
	 		_indicies.push_back(index + 1);
 			++_back; 
		 }
		 ++_size;
		 _data[index].set_index(index);
		 return &_data[index];
	}

	void remove(T* t) {
		_indicies.push_back(t->get_index());
		--_size;
	}

	const bool valid(int index) const {
		if (index < _back) {
			for (auto i : _indicies) {
				if (index == i) {
					return false;
				}
			}

			return true;
		}

		return false;
	}

	T* at(int index) const {
		return (valid(index)) ? &_data[index] : nullptr;
	}

	struct Iterator {
		T operator*() {
			return *_ptr;
		}

		T* operator->() {
			return _ptr;
		}

		Iterator& operator++() {
			do {
				++_ptr;
				++_index;
			} while (_compact_array->_data[_index].get_index() == -1 && _index < _compact_array->_back);

			return *this;
		}

		Iterator operator++(int) {
			auto temp = *this;
			do {
				++_ptr;
				++_index;
			} while (_compact_array->_data[_index].get_index() == -1 && _index < _compact_array->_back);
			return temp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) {
			return (a._ptr == b._ptr) && (a._index == b._index);
		}

		friend bool operator!=(const Iterator& a, const Iterator& b) {
			return (a._ptr != b._ptr) && (a._index != b._index);
		}

		T* _ptr = nullptr;
		size_t _index = -1;
		CompactArray<T>* _compact_array;
	};

	Iterator begin() {
		Iterator it;
		it._ptr = &_data[0];
		it._index = 0;
		it._compact_array = this;
		return it;
	}

	Iterator end() {
		Iterator it;
		it._ptr = &_data[_back];
		it._index = _back;
		it._compact_array = this;
		return it;
	}

	uint16_t size() const {
		return _size;
	}
private:
	T*        _data;
	uint16_t  _capacity;
	uint16_t  _size;
	uint16_t  _back;
	std::vector<uint16_t> _indicies;
};

#endif