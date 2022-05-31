#pragma once

#include "vector.hpp"

namespace tiny {
	template <typename T>
	class basic_string {
	public:
		inline static const size_t npos = static_cast<size_t>(-1);

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

		int compare(const basic_string& other) const noexcept;

		size_t find(const basic_string& other, size_t pos = 0) const noexcept;
		size_t find(const T* str, size_t pos = 0) const noexcept;
		size_t find(char c, size_t pos = 0) const noexcept;

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
			return _vector[idx - 1];
		}

		T& operator [](size_t idx) {
			return _vector[idx - 1];
		}

		basic_string& operator=(const basic_string& other) {
			this->_vector = other._vector;
			return *this;
		};

		basic_string& operator=(const T* str) {
			size_t otherSize = _getTSize(str);

			this->resize(otherSize);
			memcpy(this->begin(), str, otherSize * sizeof(T));
			_vector[otherSize] = 0;

			return *this;
		};
	protected:
		basic_string();
		basic_string(size_t count);

		basic_string(const T* other) {
			operator=(other);
		};

		basic_string(const basic_string& other) {
			operator=(other);
		};

	private:
		tiny::vector<T> _vector;

		size_t _getTSize(const T* str) const noexcept;
		int _compareTStr(const T* str1, const T* str2) const noexcept;
		int _ncompareTStr(const T* str1, const T* str2, size_t n) const noexcept;
	};

	template <typename T>
	inline basic_string<T>::basic_string()
		: basic_string((size_t)0) {
	}

	template <typename T>
	inline basic_string<T>::basic_string(size_t count) {
		this->resize(count);
	}

	template <typename T>
	inline void basic_string<T>::assign(size_t count, const T& value) {
		_vector.assign(count + 1, value);
		_vector[count] = 0;
	}

	template <typename T>
	inline constexpr const T* basic_string<T>::data() const noexcept {
		return _vector.data();
	}

	template <typename T>
	inline constexpr const T& basic_string<T>::back() const {
		return _vector[this->size() - 1];
	}

	template <typename T>
	inline constexpr const T& basic_string<T>::front() const {
		return _vector[this->size()];
	}

	template <typename T>
	inline constexpr T* basic_string<T>::begin() const noexcept {
		return _vector.begin();
	}

	template <typename T>
	inline constexpr T* basic_string<T>::end() const {
		return this->begin() + this->size();
	}

	template <typename T>
	inline constexpr bool basic_string<T>::empty() const noexcept {
		return this->size() == 0;
	}

	template <typename T>
	inline constexpr size_t basic_string<T>::size() const noexcept {
		return _vector.size() - 1; // remove terminatrion character
	}

	template <typename T>
	inline constexpr size_t basic_string<T>::capacity() const noexcept {
		return _vector.capacity() - 1;
	}

	template <typename T>
	inline constexpr size_t basic_string<T>::max_size() const noexcept {
		return _vector.max_size();
	}

	template <typename T>
	inline int basic_string<T>::compare(const basic_string& other) const noexcept {
		return this->_compareTStr(this->begin(), other.begin());
	}
	template <typename T>
	inline size_t basic_string<T>::find(const basic_string& other, size_t pos) const noexcept {
		return this->find(other.begin(), pos);
	}

	template <typename T>
	inline size_t basic_string<T>::find(const T* str, size_t pos) const noexcept {
		auto strSize = this->_getTSize(str);
		for (size_t i = pos; i < this->size(); i++)
		{
			if (!this->_ncompareTStr(this->begin() + i, str, strSize))
				return i;
		}

		return basic_string::npos;
	}

	template <typename T>
	inline size_t basic_string<T>::find(char c, size_t pos) const noexcept {
		for (size_t i = pos; i < this->size(); i++)
		{
			if (_vector[i] && _vector[i] == c)
				return i;
		}

		return basic_string::npos;
	}

	template <typename T>
	inline void basic_string<T>::resize(size_t count) {
		_vector.resize(count + 1);
		_vector[count] = 0;
	}

	template <typename T>
	inline void basic_string<T>::reserve(size_t count) {
		_vector.reserve(count + 1);
	}

	template <typename T>
	inline void basic_string<T>::erase(size_t pos) {
		_vector.erase(pos);
	}

	template <typename T>
	inline void basic_string<T>::erase(size_t first, size_t last) {
		_vector.erase(first, last);
	}

	template <typename T>
	inline void basic_string<T>::clear() noexcept {
		this->resize(0);
	}

	template <typename T>
	inline void basic_string<T>::shrink_to_fit() {
		_vector.shrink_to_fit();
	}

	template <typename T>
	inline constexpr T& basic_string<T>::at(size_t pos) const {
		_vector.at(pos - 1);
	}

	template <typename T>
	inline constexpr void basic_string<T>::insert(size_t pos, const T& value) {
		_vector.insert(pos - 1, value);
	}

	template <typename T>
	inline constexpr void basic_string<T>::push_back(const T& value) {
		_vector[this->size()] = value;
		_vector.push_back(0);
	}

	template <typename T>
	inline constexpr void basic_string<T>::pop_back() {
		this->resize(this->size() - 1);
	}

	//
	// private 
	//

	template <typename T>
	inline size_t basic_string<T>::_getTSize(const T* str) const noexcept {
		size_t strSize = 0;
		while (str[strSize])
			++strSize;

		return strSize;
	}

	template <typename T>
	inline int basic_string<T>::_compareTStr(const T* str1, const T* str2) const noexcept {
		while (*str1 && (*str1 == *str2))
		{
			++str1;
			++str2;
		}

		if (*str1 == *str2)
			return 0;

		return *str1 < *str2 ? -1 : 1;
	}

	template <typename T>
	inline int basic_string<T>::_ncompareTStr(const T* str1, const T* str2, size_t n) const noexcept {
		while (*str1 && (*str1 == *str2))
		{
			++str1;
			++str2;

			if (!--n)
				return 0;
		}

		if (*str1 == *str2)
			return 0;

		return *str1 < *str2 ? -1 : 1;
	}

	//
	//
	//

	class string : public basic_string<char> {
	public:
		inline string() = default;
		inline string(size_t count) : basic_string(count) {};
		inline string(const char* other) : basic_string(other) {};
		inline string(const string& other) : basic_string(other) {};
	};

	class wstring : public basic_string<wchar_t> {
	public:
		inline wstring() = default;
		inline wstring(size_t count) : basic_string(count) {};
		inline wstring(const wchar_t* other) : basic_string(other) {};
		inline wstring(const wstring& other) : basic_string(other) {};
	};
}