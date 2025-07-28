#pragma once
#include <iostream>
#include <vector>

template <typename T>
class PriorityQueue
{
	static size_t leftChild(size_t i);
	static size_t rightChild(size_t i);
	static int parent(int i);

	void heapify(size_t ind);
	std::vector<T> data;

public: // zashto nqmqm size;

	PriorityQueue() = default;
	PriorityQueue(const std::vector<T>& v);

	bool empty() const;
	const T& peek() const;
	void pop();
	void insert(const T& el);
	size_t size() const;
};

template<typename T>
void PriorityQueue<T>::heapify(size_t ind)
{
	int currentElementIndex = ind;
	while (true)
	{
		int leftChildIndex = leftChild(currentElementIndex);
		int rightChildIndex = rightChild(currentElementIndex);

		bool shouldGoLeft = leftChildIndex < data.size() && data[leftChildIndex] > data[currentElementIndex];
		bool shouldGoRight = rightChildIndex < data.size() && data[rightChildIndex] > data[currentElementIndex];

		if (shouldGoLeft && shouldGoRight)
		{
			if (data[leftChildIndex] > data[rightChildIndex])
			{
				std::swap(data[leftChildIndex], data[currentElementIndex]);
				currentElementIndex = leftChildIndex;
			}
			else
			{
				std::swap(data[currentElementIndex], data[rightChildIndex]);
				currentElementIndex = rightChildIndex;
			}
		}
		else if (shouldGoLeft)
		{
			std::swap(data[currentElementIndex], data[leftChildIndex]);
			currentElementIndex = leftChildIndex;
		}
		else if (shouldGoRight)
		{
			std::swap(data[currentElementIndex], data[rightChildIndex]);
			currentElementIndex = rightChildIndex;
		}
		else break;
	}
}

template<typename T>
size_t PriorityQueue<T>::leftChild(size_t i)
{
	return 2 * i + 1;
}

template<typename T>
size_t PriorityQueue<T>::rightChild(size_t i)
{
	return 2 * i + 2;
}

template<typename T>
int PriorityQueue<T>::parent(int i)
{
	return (i - 1) / 2;
}

template<typename T>
PriorityQueue<T>::PriorityQueue(const std::vector<T>& v)
{
	data = v;

	for (int i = v.size() / 2 - 1; i >= 0; --i)
		heapify(i);
}

template<typename T>
bool PriorityQueue<T>::empty() const
{
	return data.empty();
}

template<typename T>
const T& PriorityQueue<T>::peek() const
{
	if (empty())
		throw std::runtime_error("Empty queue");
	return data[0];
}

template<typename T>
void PriorityQueue<T>::pop()
{
	if (empty())
		throw std::runtime_error("Empty queueu!");

	data[0] = data[data.size() - 1];
	data.pop_back();

	if (data.size() != 0)
		heapify(0);
}

template<typename T>
void PriorityQueue<T>::insert(const T& el)
{
	data.push_back(el);
	int ind = data.size() - 1;
	int parentInd = parent(ind);

	while (ind > 0 && data[ind] > data[parentInd])
	{
		std::swap(data[ind], data[parentInd]);
		ind = parentInd;
		parentInd = parent(ind);
	}
}

template<typename T>
size_t PriorityQueue<T>::size() const
{
	return data.size();
}
