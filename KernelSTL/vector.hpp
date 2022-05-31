#pragma once

#include "common.hpp"
#define Debug(msg, ...) do {DbgPrintEx(0, 0, "[Tiny]: " msg "\n", __VA_ARGS__);}while(0)
namespace tiny {
template <typename T>
class vector {
public:
	vector();
	~vector();
	vector(size_t count);
	//template <typename... Args>
	//vector(Args&&... args);

	vector(const vector& other) {
		operator=(other);
	};

	void assign(size_t count, const T& value);

	constexpr const T* data() const noexcept;
	constexpr const T& back() const;
	constexpr const T& front() const;

	constexpr T* begin() const noexcept;
	constexpr T* end() const;

	constexpr bool empty() const noexcept;
	constexpr size_t size() const noexcept;
	constexpr size_t capacity() const noexcept;
	constexpr size_t max_size() const noexcept;

	void resize(size_t count);
	void reserve(size_t count);
	void erase(size_t pos);
	void erase(size_t first, size_t last);
	void clear() noexcept;
	void shrink_to_fit();

	constexpr T& at(size_t pos) const;

	constexpr void insert(size_t pos, const T& value);
	constexpr void push_back(const T& value);
	constexpr void pop_back();

	constexpr const T& operator [](size_t idx) const {
		return _buffer[idx];
	}

	T& operator [](size_t idx) {
		return _buffer[idx];
	}

	vector& operator=(const vector& other) {
		this->_freeBuffer();

		if (other._capacity) {
			this->reserve(other._capacity);

			for (size_t i = 0; i < other._size; ++i)
				this->_buffer[i] = other._buffer[i];

			this->_size = other._size;
		}

		return *this;
	};
private:
	T* _buffer;
	size_t _size;
	size_t _capacity;

	void _reserve(size_t count);
	void _freeBuffer();
};

template <typename T>
inline vector<T>::~vector() {
	this->_freeBuffer();
}

template <typename T>
inline vector<T>::vector()
	: _buffer(nullptr), _size(0), _capacity(0) {
}

template <typename T>
inline vector<T>::vector(size_t count)
	: vector() {
	this->resize(count);
}

//template <typename T>
//template <typename... Args>
//inline vector<T>::vector(Args&&... args) {
//	int temp[] = { (this->push_back(args), 0)... };
//	UNREFERENCED_PARAMETER(temp);
//}

template <typename T>
inline void vector<T>::assign(size_t count, const T& value) {
	this->clear();
	this->reserve(count);

	_size = count;
	while (count--)
		_buffer[count] = value;
}

template <typename T>
inline constexpr const T* vector<T>::data() const noexcept {
	return _buffer;
}

template <typename T>
inline constexpr const T& vector<T>::back() const {
	return _buffer[_size];
}

template <typename T>
inline constexpr const T& vector<T>::front() const {
	return _buffer[0];
}

template <typename T>
inline constexpr T* vector<T>::begin() const noexcept{
	return _buffer;
}

template <typename T>
inline constexpr T* vector<T>::end() const {
	return _buffer + _size;
}

template <typename T>
inline constexpr bool vector<T>::empty() const noexcept {
	return _size == 0;
}

template <typename T>
inline constexpr size_t vector<T>::size() const noexcept {
	return _size;
}

template <typename T>
inline constexpr size_t vector<T>::capacity() const noexcept {
	return _capacity;
}

template <typename T>
inline constexpr size_t vector<T>::max_size() const noexcept {
	return static_cast<size_t>(-1) / sizeof(T);
}

template <typename T>
inline void vector<T>::resize(size_t count) {
	if (count == _size)
		return;

	if (!count)
		return this->clear();

	if (count <= _size)
		return this->erase(count, _size);

	this->reserve(count);
	
	for (size_t i = _size; i < count; ++i)
		_buffer[i] = T();

	_size = count;
}

template <typename T>
inline void vector<T>::reserve(size_t count) {
	if (count <= _capacity)
		return;

	this->_reserve(count);
}

template <typename T>
inline void vector<T>::erase(size_t pos) {
	if (pos == _size - 1)
	{
		this->pop_back();
		return;
	}

	_buffer[pos].~T();
	memcpy(_buffer + pos, _buffer + pos + 1, (_size-- - pos) * sizeof(T));
}

template <typename T>
inline void vector<T>::erase(size_t first, size_t last) {
	if (first == last)
		return;

	for (auto i = first; i < last; ++i)
		_buffer[i].~T();

	memcpy(_buffer + first, _buffer + last, (_size - last) * sizeof(T));
	_size -= last - first;
}

template <typename T>
inline void vector<T>::clear() noexcept {
	while (!this->empty())
		this->pop_back();
}

template <typename T>
inline void vector<T>::shrink_to_fit() {
	if (_size == _capacity)
		return;

	this->_reserve(_size);
}

template <typename T>
inline constexpr T& vector<T>::at(size_t pos) const {
	if (pos >= _size)
		return nullptr;

	return _buffer[pos];
}

template <typename T>
inline constexpr void vector<T>::insert(size_t pos, const T& value) {
	if (_size == _capacity)
		this->reserve(_capacity + 1);

	memcpy(_buffer + pos + 1, _buffer + pos, (_size - pos) * sizeof(T));
	_buffer[pos] = value;
	_size++;
}

template <typename T>
inline constexpr void vector<T>::push_back(const T& value) {
	if (_size == _capacity)
		this->reserve(_capacity + 1);

	_buffer[_size++] = value;
}

template <typename T>
inline constexpr void vector<T>::pop_back() {
	_buffer[--_size].~T();
}

//
// private
//

template <typename T>
inline void vector<T>::_reserve(size_t count) {
	if (!count)
		return this->_freeBuffer();

	auto newBuffer = reinterpret_cast<T*>(ALLOC_MEMORY(count * sizeof(T)));
	if (!newBuffer)
		ExRaiseStatus(STATUS_MEMORY_NOT_ALLOCATED);

	for (size_t i = 0; i < _size; ++i)
		newBuffer[i] = _buffer[i];

	auto currentSize = _size;
	this->_freeBuffer();

	_buffer = newBuffer;
	_capacity = count;
	_size = currentSize;
}

template <typename T>
inline void vector<T>::_freeBuffer() {
	if (!_buffer)
		return;

	this->clear();

	FREE_MEMORY(_buffer);
	_buffer = nullptr;
	_capacity = 0;
}
}