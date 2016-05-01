#pragma once
#include <cstdint>
#include <cassert>

template <typename T, typename Offset = uint32_t>
class RelativePointer {
	Offset offset;

	T* toPointer() const {
		uintptr_t ptrValue = reinterpret_cast<uintptr_t>(this);
		ptrValue += offset;
		return reinterpret_cast<T*>(ptrValue);
	}
public:
	RelativePointer(Offset offset) 
		: offset(offset) {
		assert(offset > 0);
	}

	T* ptr() {
		return toPointer();
	}
	const T* ptr() const {
		return toPointer();
	}
	
	T& operator* () {
		return *ptr();
	}
	const T& operator* () const {
		return *ptr();
	}

	T* operator-> () {
		return ptr();
	}
	const T* operator-> () const {
		return ptr();
	}
			
	T& operator[] (int index) {
		return ptr()[index];
	}
	const T& operator[] (int index) const {
		return ptr()[index];
	}
};
