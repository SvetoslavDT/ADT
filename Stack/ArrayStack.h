#pragma once
#include <iostream>

template<typename T>
class ArrayStack
{
	T* data;
	size_t size;
	size_t capacity;

	void copyFrom(const ArrayStack<T>& other); // ?? trqbva li mi <T>
	void resize(size_t);
	void free();

public:

	ArrayStack();
	ArrayStack(const ArrayStack<T>& other);
	ArrayStack& operator=(const ArrayStack<T>& other);
	~ArrayStack();

	void push(const T& element);
	void pop();
	const T& peek() const;

	size_t getSize() const;
	bool empty() const;
};

template<typename T>
void ArrayStack<T>::copyFrom(const ArrayStack<T>& other)
{
	data = new T[other.capacity];
	for (size_t i = 0; i < other.size; ++i)
	{
		data[i] = other.data[i];
	}
	size = other.size;
	capacity = other.capacity;
}

template<typename T>
void ArrayStack<T>::resize(size_t newCapacity)
{
	T* newData = new T[newCapacity];

	for (size_t i = 0; i < size; ++i)
		newData[i] = std::move(data[i]);

	delete[] data;
	data = newData;

	capacity = newCapacity;
}

template <typename T>
void ArrayStack<T>::free()
{
	delete[] data;
	size = 0;
	capacity = 0;
}

template<typename T>
ArrayStack<T>::ArrayStack() : size(0), capacity(4)
{
	data = new T[capacity];
}

template<typename T>
ArrayStack<T>::ArrayStack(const ArrayStack<T>& other)
{
	copyFrom(other);
}

template<typename T>
ArrayStack<T>& ArrayStack<T>::operator=(const ArrayStack<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
ArrayStack<T>::~ArrayStack()
{
	free();
}

template<typename T>
void ArrayStack<T>::push(const T& el)
{
	if (size >= capacity)
		resize(capacity * 2);

	data[size++] = el;
}

template<typename T>
void ArrayStack<T>::pop()
{
	if (empty())
		throw std::runtime_error("Stack is empty!");

	T el = data[--size];

	if (size * 2 <= capacity && capacity > 1)
		resize(capacity / 2);
}

template<typename T>
const T& ArrayStack<T>::peek() const
{
	if (empty())
		throw std::runtime_error("Stack is empty!");

	return data[size - 1];
}

template<typename T>
size_t ArrayStack<T>::getSize() const
{
	return size;
}

template <typename T>
bool ArrayStack<T>::empty() const
{
	return getSize() == 0;
}
