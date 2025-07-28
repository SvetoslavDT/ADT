#pragma once
#include <iostream>

template <typename T>
class Queue
{
	T* data;
	size_t count;
	size_t capacity;

	size_t getIter;
	size_t putIter;

	void resize();
	void copyFrom(const Queue<T>& other);
	void free();

public:

	Queue();
	Queue(const Queue<T>& other);
	Queue& operator=(const Queue<T>& other);
	~Queue();

	bool empty() const;
	void enqueue(const T& el);
	void dequeue();
	const T& peek() const;
};

template<typename T>
void Queue<T>::resize()
{
	T* temp = new T[capacity * 2];
	for (int i = 0; i < capacity; ++i)
	{
		temp[i] = peek();
		dequeue();
	}

	getIter = 0;
	putIter = capacity;

	count = capacity;
	capacity *= 2;

	delete[] data;
	data = temp;
}

template<typename T>
void Queue<T>::copyFrom(const Queue<T>& other)
{
	data = new T[other.capacity];
	for (int i = other.getIter; i != other.putIter; (i += 1) %= other.capacity)
		data[i] = other.data[i];

	count = other.count;
	capacity = other.capacity;

	getIter = other.getIter;
	putIter = other.putIter;
}

template<typename T>
void Queue<T>::free()
{
	delete[] data;
	other = capacity = 0;
}

template<typename T>
Queue<T>::Queue()
{
	const size_t DEFAULT_CAPACITY = 4;
	data = new T[DEFAULT_CAPACITY];
	capacity = DEFAULT_CAPACITY;
	count = 0;
	putIter = 0;
	getIter = 0;
}

template <typename T>
Queue<T>::Queue(const Queue<T>& other)
{
	copyFrom(other);
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other)
{
	if (this != &othen)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
Queue<T>::~Queue()
{
	free();
}

template<typename T>
bool Queue<T>::empty() const
{
	return count == 0;
}

template<typename T>
void Queue<T>::enqueue(const T& el)
{
	if (count == capacity)
		resize();

	data[putIter] = el;

	(++putIter) %= capacity;
	++count;
}

template<typename T>
void Queue<T>::dequeue()
{
	if (count == 0)
		throw std::runtime_error("Queue is empty!");

	(++getIter) %= capacity;
	--count;
}

template<typename T>
const T& Queue<T>::peek() const
{
	if (empty())
		throw std::runtime_error("Queue is empty!");

	return data[getIter];
}