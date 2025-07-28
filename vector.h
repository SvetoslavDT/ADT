#pragma once
#include <iostream>
#include <memory>
#include "iterator.h"

namespace constraints
{
	size_t GROWTH_FACTOR = 2;
}

template <typename T, typename Allocator = std::allocator<T>>
class vector
{
	T* data = nullptr;
	size_t size = 0;
	size_t capacity = 0;

	Allocator allocator;

	void copy(const vector& other);
	void move(vector&& other);
	void free();

	size_t calculateCapacity() const;

public:

	using iterator = vector_iterator<T>;
	using const_iterator = vector_const_iterator<T>;
	using reverse_iterator = vector_reverse_iterator<T>;

	vector() = default;
	explicit vector(size_t count);
	vector(size_t size, const T& initial);

	vector(const vector& other);
	vector& operator=(const vector& other);

	vector(vector&& other);
	vector& operator=(vector&& other);

	void push_back(const T& element);
	void push_back(T&& element);
	void pop_back();

	void erase(const_iterator position);
	void erase(const_iterator first, const_iterator second);

	void clear();

	template<typename... Args>
	void emplace_back(Args&&... args);

	T& operator[](size_t ind);
	const T& operator[](size_t ind) const;

	T* data();

	void resize(size_t n);
	void reserve(size_t n);
	void shrink_to_fit();

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	iterator begin()
	{
		return iterator(data);
	}
	iterator end()
	{
		return iterator(data + size);
	}
	const_iterator cbegin() const
	{
		return const_iterator(data);
	}
	const_iterator cend() const
	{
		return const_iterator(data + size);
	}
	reverse_iterator rbegin()
	{
		return size() > 0 ? reverse_iterator(&data[size() - 1]) : reverse_iterator(nullptr);
	}
	reverse_iterator rend()
	{
		return size() > 0 ? reverse_iterator(data - 1) : reverse_iterator(nullptr);
	}

	T& front()
	{
		return data[0];
	}
	const T& front() const
	{
		return data[0];
	}
	T& back()
	{
		return data[size() - 1];
	}
	const T& back() const
	{
		return data[size() - 1];
	}

	~vector();

};


template<typename T, typename Allocator>
void vector<T, Allocator>::copy(const vector<T, Allocator>& other)
{
	data = allocator.allocate(other.capacity());

	for (size_t i = 0; i < size; ++i)
		allocator.construct(&data[i], other[i]);
	
	size = other.size;
	capacity = other.capacity;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::move(vector<T, Allocator>&& other)
{
	data = other.data;
	other.data = nullptr;

	size = other.size;
	other.size = 0;

	capacity = other.capacity;
	other.capacity = 0;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::free()
{
	for (size_t i = 0; i < size; i++)
	{
		allocator.destroy(&data[i]);
	}
	allocator.deallocate(_data, capacity());
}

template <typename T, typename Allocator>
size_t vector<T, Allocator>::calculateCapacity() const
{
	if (capacity == 0)
		return 1;
	return capacity * constraints::GROWTH_FACTOR;
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(size_t count) : data(allocator.allocate(count)), size(count), capacity(count)
{
	for (size_t i = 0; i < size(); i++)
		allocator.construct(&data[i]);
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(size_t size, const T& initial) : size(size), capacity(size), data(allocator.allocate(count))
{
	for (size_t i = 0; i < size(); ++i)
		allocator.construct(&data[i], initial);
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(const vector& other)
{
	copy(other);
}

template<typename T, typename Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& other)
{
	if (this != &other)
	{
		free();
		copy(other);
	}
	return *this;
}

template <typename T, typename Allocator>
vector < T, Allocator>::vector(vector&& other)
{
	move(std::move(other));
}

template <typename T, typename Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector&& other)
{
	if (this != &other)
	{
		free();
		move(std::move(other));
	}

	return *this;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::push_back(const T& el)
{
	if (size == capacity)
		reserve(calculateCapacity())

	allocator.construct(data[size++], el)
}

template <typename T, typename Allocator>
void vector<T, Allocator>::push_back(T&& el)
{
	if (size == capacity)
		reserve(calculateCapacity());

	allocator.construct(&data[size++], std::move(el));
}

template <typename T, typename Allocator>
void vector<T, Allocator>::pop_back()
{
	if (empty())
		return;

	erase(--end());
}

template <typename T, typename Allocator>
void vector<T, Allocator>::erase(const_iterator position)
{
	erase(position, position + 1);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::erase(const_iterator first, const_iterator last)
{
	int deleted_count = last - first;

	if (deleted_count <= 0)
		return;

	size_t begin_offset = first - begin();
	size_t end_offset = last - begin();

	if (last != cend())
	{
		size_t construct_idx = begin_offset;
		for (size_t i = end_offset; i < size(); ++i)
		{
			allocator.construct(&data[construct_idx++], std::move(data[i]));
		}
	}

	for (size_t i = end_offset; i < size(); ++i)
		allocator.destroy(&data[i]);

	size -= deleted_count;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::clear()
{
	for (size_t i = 0; i < size(); ++i)
	{
		allocator.destroy(&data[i]);
	}

	size = 0;
}

template<typename T, typename Allocator>
template<typename ...Args>
void vector<T, Allocator>::emplace_back(Args&& ...args)
{
	if (size == capacity)
		reserve(calculateCapacity());

	allocator.construct(&data[size++], std::forward<Args>(args)...);
}

template<typename T, typename Allocator>
T& vector<T, Allocator>::operator[](size_t ind)
{
	return data[ind];
}

template<typename T, typename Allocator>
const T& vector<T, Allocator>::operator[](size_t ind) const
{
	return data[ind];
}

template<typename T, typename Allocator>
T* vector<T, Allocator>::data()
{
	return data;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::resize(size_t newSize)
{
	if (n < size)
	{
		for (size_t i = n; i < size(); i++)
			allocator.destroy(&data[i]);
		size = n;
	}
	else if (n > size)
	{
		if (n <= capacity)
		{
			for (size_t i = size(); i < n; ++i)
				allocator.construct(&data[i]);

			size = n;
		}
		else
		{
			T* newData = allocator.allocate(n);

			for (size_t i = 0; i < size(); ++i)
				allocator.construct(&newData[i], std::move(data[i]));   // ?? Tuk Dimitriev ima greshka

			for (size_t i = size(); i < n; ++i)
				allocator.construct(&newData[i]);

			for (size_t i = 0; i < size(); ++i)
				allocator.destroy(&data[i]);

			allocator.deallocate(data, capacity());

			data = newData;
			capacity = n;
			size = n;
		}
	}
}

template<typename T, typename Allocator>
void vector<T, Allocator>::reserve(size_t n)
{
	if (n <= capacity)
		return;

	T* newData = allocator.allocate(n);

	for (size_t i = 0; i < size(); ++i)
		allocator.construct(&newData[i], std::move(data[i]));

	allocator.deallocate(data, capacity);

	data = newData;
	capacity = n;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::shrink_to_fit()
{
	if (capacity() == size())
		return;

	T* newData = allocator.allocate(size());
	for (size_t i = 0; i < size(); ++i)
		allocator.construct(newData + i, std::move(data[i]));

	allocator.deallocate(data, capacity());

	data = newData;
	capacity = size;
}

template<typename T, typename Allocator>
inline size_t vector<T, Allocator>::size() const
{
	return size;
}

template<typename T, typename Allocator>
inline size_t vector<T, Allocator>::capacity() const
{
	return capacity;
}

template<typename T, typename Allocator>
inline bool vector<T, Allocator>::empty() const
{
	return (size() == 0);
}

template<typename T, typename Allocator>
vector<T, Allocator>::~vector()
{
	free();
}
