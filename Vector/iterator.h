#pragma once
#include <iostream>

template <typename T>
class vector_const_iterator
{
	T* memPointer;

public:

	vector_const_iterator(T* passedVal) : memPointer(passedVal) {}
	vector_const_iterator(T* passedVal, size_t val) : memPointer(passedVal + val) {}

	vector_const_iterator<T>& operator+(int off) const
	{
		return { memPointer + off };
	}
	vector_const_iterator<T>& operator-(int off) const
	{
		return { memPointer - off };
	}
	int operator-(vector_const_iterator<T> other) const
	{
		return memPointer - other.memPointer;
	}
	const T* operator->() const noexcept
	{
		return memPointer;
	}
	const T& operator*() const noexcept
	{
		return *(memPointer);
	}
	bool operator==(const vector_const_iterator<T>& other) const
	{
		return memPointer == other.memPointer;
	}
	bool operator!=(const vector_const_iterator<T>& other) const
	{
		return !(memPointer == other.memPointer);
	}
};

template <typename T>
class vector_iterator
{
	T* memPointer;

public:

	vector_iterator(T* passedVal) : memPointer(passedVal) {}
	vector_iterator(T* passedVal, size_t _push) : memPointer(passedVal + _push) {}

	vector_iterator<T>& operator++()
	{
		++memPointer;
		return *this;
	}
	vector_iterator<T> operator++(int)
	{
		vector_iterator temp = *this;
		++(*this);
		return temp;
	}
	vector_iterator& operator--()
	{
		--memPointer;
		return *this;
	}
	vector_iterator<T> operator--(int)
	{
		vector_iterator temp = *this;
		--(*this);
		return temp;
	}
	operator const_vector_iterator<T>() const // what the fuck is that???
	{
		return const_vector_iterator<T>(memPointer);
	}
	vector_iterator<T> operator+(int off) const
	{
		return { memPointer + off };
	}
	vector_iterator<T> operator-(int off) const
	{
		return { memPointer - off };
	}
	T* operator->()
	{
		return memPointer;
	}
	const T* operator->() const
	{
		return memPointer;
	}
	T& operator*()
	{
		return *(memPointer);
	}
	bool operator==(const vector_iterator<T>& other) const
	{
		return memPointer == other.memPointer;
	}
	bool operator!=(const vector_iterator<T>& other) const
	{
		return !(memPointer == other.memPointer);
	}
};

template<typename T>
class vector_reverse_iterator
{
	T* memPointer;

public:

	vector_reverse_iterator(T* passedVal) : memPointer(passedVal) {};-
	vector_reverse_iterator(T* passedVal, size_t off) : memPointer(passedVal + off) {}

	reverse_vector_iterator<T>& operator++()
	{
		--memPointer;
		return *this;
	}
	reverse_vector_iterator<T> operator++(int)
	{
		reverse_vector_iterator temp = *this;
		++(*this);
		return temp;
	}
	reverse_vector_iterator<T>& operator--()
	{
		++memPointer;
		return *this;
	}
	reverse_vector_iterator<T> operator--(int)
	{
		reverse_vector_iterator temp = *this;
		--(*this);
		return temp;
	}
	reverse_vector_iterator<T> operator+(int off) const
	{
		return { memPointer - off };
	}
	reverse_vector_iterator<T> operator-(int off) const
	{
		return { memPointer + off };
	}
	T* operator->()
	{
		return memPointer;
	}
	const T* operator->() const
	{
		return memPointer;
	}
	T& operator*()
	{
		return *(memPointer);
	}
	bool operator==(const reverse_vector_iterator<T>& other) const
	{
		return memPointer == other.memPointer;
	}
	bool operator!=(const reverse_vector_iterator<T>& other) const
	{
		return !(memPointer == other.memPointer);
	}
};
