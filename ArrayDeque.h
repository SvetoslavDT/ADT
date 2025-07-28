#pragma once
#include <memory>
#include <stdexcept>
#include <algorithm>

template<typename T, typename AllocatorType = std::allocator<T>>
class deque
{
	T* data =nullptr;
	size_t size = 0;
	size_t capacity = 0;
	size_t head = 0;
	size_t tail = 0;

	AllocatorType allocator;

	void moveIndex(size_t& index, bool forward) const;

	void copyFrom(const deque<T>& other);
	void moveFrom(deque<T>&& other);
	void free();

	void resize(size_t capacity = 0);

	size_t calculateCapacity() const;

public:

	deque();

	deque(const deque<T>& other);
	deque& operator=(const deque<T>& other);

	deque(deque<T>&& other);
	deque& operator=(deque<T>&& other);

	~deque();

	void push_front(const T& el);
	void push_front(T&& el);

	void push_back(const T& el);
	void push_back(T&& el);

	void pop_front();
	void pop_back();

	template<typename... Args>
	void emplace_front(Args&&... args);
	template<typename... Args>
	void emplace_back(Args&&... args);

	T& operator[](size_t ind);
	const T& operator[](size_t ind) const;

	T& front();
	T& back();

	const T& front() const;
	const T& back() const;

	size_t getSize() const;
	bool empty() const;

	class DequeIterator
	{
		friend class deque;

		deque& d;
		size_t index;
		DequeIterator(deque& d, int index) : d(d), index(index) {} // ?? zashto tova e v private ( na CONST sushto)

	public:

		DequeIterator& operator++()
		{
			++index;
			return *this;
		}
		DequeIterator operator++(int)
		{
			DequeIterator temp(*this);
			++index;
			return temp;
		}
		DequeIterator& operator--()
		{
			--index;
			return *this;
		}
		DequeIterator operator--(int)
		{
			DequeIterator temp(*this);
			--index;
			return temp;
		}
		DequeIterator operator+(int offset)
		{
			return DequeIterator(d, index + offset);
		}
		DequeIterator operator-(int offset)
		{
			return DequeIterator(d, index - offset);
		}
		T& operator*()
		{
			return d[index];
		}
		bool operator==(const DequeIterator& other)
		{
			return index == other.index;
		}
		bool operator!=(const DequeIterator& other)
		{
			return index != other.index;
		}
	};
	class ConstDequeIterator
	{
		friend class deque;

		const deque& d;
		size_t index;
		ConstDequeIterator(const deque& d, size_t index) : d(d), index(index) {}

	public:

		ConstDequeIterator& operator++()
		{
			++index;
			return *this;
		}
		ConstDequeIterator operator++(int)
		{
			ConstDequeIterator temp(*this);
			++index;
			return temp;
		}
		ConstDequeIterator& operator--()
		{
			--index;
			return *this;
		}
		ConstDequeIterator operator--(int)
		{
			ConstDequeIterator temp(*this);
			--index;
			return temp;
		}
		ConstDequeIterator operator+(int offset)
		{
			return ConstDequeIterator(d, index + offset);
		}
		ConstDequeIterator operator-(int offset)
		{
			return ConstDequeIterator(d, index - offset);
		}
		T& operator*()
		{
			return d[index];
		}
		bool operator==(const ConstDequeIterator& other)
		{
			return index == other.index;
		}
		bool operator !=(const ConstDequeIterator& other)
		{
			return index != other.index;
		}
	};

	DequeIterator begin()
	{
		return DequeIterator(*this, 0);
	}
	DequeIterator end()
	{
		return DequeIterator(*this, getSize());
	}
	ConstDequeIterator cbegin() const
	{
		return ConstDequeIterator(*this, 0);
	}
	ConstDequeIterator cend() const
	{
		return ConstDequeIterator(*this, getSize());
	}
};

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::moveIndex(size_t& index, bool forward) const
{
	if (capacity == 0)
		throw std::runtime_error("Trying to move index on deque with no capacity!");
	if (forward)
		(++index) %= capacity;
	else
		index = (index == 0) ? capacity - 1 : index - 1;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::copyFrom(const deque<T>& other)
{
	capacity = other.capacity;
	data = allocator.allocate(other.capacity);
	for (size_t i = 0; i != other.getSize(); i++)
		push_back(other[i]);
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::moveFrom(deque<T>&& other)
{
	capacity = other.capacity;
	data = other.data;
	size = other.size;
	head = other.head;
	tail = other.head;

	other.data = nullptr;
	other.size = other.capacity = other.head = other.tail = 0;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::free()
{
	if (head == tail && size >= 1)
	{
		allocator.destroy(data + head);
		moveIndex(head, true);
	}
	while (head != tail)
	{
		allocator.destroy(data + tail);
		moveIndex(head, forward);
	}
	allocator.deallocate(data, capacity);
	data = nullptr;
	size = capacity = head = tail = 0;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::resize(size_t newCapacity)
{
	if (newCapacity == 0)
		newCapacity = calculateCapacity;

	T* newData = allocator.allocate(newCapacity);
	size_t includedElementsCount = std::min(size, newCapacity);
	for (size_t i = 0; i < includedElementsCount; i++)
	{
		allocator.construct(newData + i, std::move(operator[](i)));
		allocator.destroy(data + ((head + i) % capacity));
	}
	allocator.deallocate(data, capacity);
	data = newData;

	head = 0;
	tail = size;
	capacity = newCapacity;
}

template<typename T, typename AllocatorType>
size_t deque<T, AllocatorType>::calculateCapacity() const
{
	return capacity ? capacity * 2 : 1;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::push_front(const T& el)
{
	if (size >= capacity)
		resize();

	moveIndex(head, false);
	allocator.construct(data + head, el);
	++size;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::push_front(T&& el)
{
	if (size >= capacity)
		resize();

	moveIndex(head, false);
	allocator.construct(data + head, std::move(el));
	++size;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::push_back(const T& el)
{
	if (size >= capacity)
		resize();

	allocator.construct(data + tail, el);
	moveIndex(tail, true);
	++size;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::push_back(T&& el)
{
	if (size >= capacity)
		resize();

	allocator.construct(data + tail, std::move(el));
	moveIndex(tail, true);
	++size;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::pop_front()
{
	if(empty())
		throw std::out_of_range("deque is empty");

	allocator.destroy(data + head);
	moveIndex(head, true);
	--size;
}

template<typename T, typename AllocatorType>
void deque<T, AllocatorType>::pop_back()
{
	if (empty())
		throw std::out_of_range("deque is empty");

	moveIndex(tail, false);
	allocator.destroy(data + tail);
	--size;
}

template<typename T, typename AllocatorType>
T& deque<T, AllocatorType>::operator[](size_t ind)
{
	return data[(head + ind) % capacity];
}

template<typename T, typename AllocatorType>
const T& deque<T, AllocatorType>::operator[](size_t ind) const
{
	return data[(head + ind) % capacity];
}

template<typename T, typename AllocatorType>
T& deque<T, AllocatorType>::front()
{
	return data[head];
}

template<typename T, typename AllocatorType>
T& deque<T, AllocatorType>::back()
{
	return data[(tail == 0 ? capacity : tail) - 1];
}

template<typename T, typename AllocatorType>
const T& deque<T, AllocatorType>::front() const
{
	return data[head];
}

template<typename T, typename AllocatorType>
const T& deque<T, AllocatorType>::back() const
{
	return data[(tail == 0 ? capacity : tail) - 1];
}

template<typename T, typename AllocatorType>
size_t deque<T, AllocatorType>::getSize() const
{
	return size;
}

template<typename T, typename AllocatorType>
bool deque<T, AllocatorType>::empty() const
{
	return getSize() == 0;
}

template<typename T, typename AllocatorType>
template<typename ...Args>
void deque<T, AllocatorType>::emplace_front(Args && ...args)
{
	if (size >= capacity)
		resize();

	moveIndex(head, false);
	allocator.construct(data + head, std::forward<Args>(args)...);
	++size;
}

template<typename T, typename AllocatorType>
template<typename ...Args>
void deque<T, AllocatorType>::emplace_back(Args && ...args)
{
	if (size >= capacity)
		resize();

	allocator.construct(data + tail, std::forward<Args>(args)...);
	moveIndex(tail, true);
	++size;
}

template<typename T, typename AllocatorType>
deque<T, AllocatorType>::deque() : size(0), capacity(8), head(0), tail(0)
{
	data = allocator.allocate(capacity);
}

template<typename T, typename AllocatorType>
deque<T, AllocatorType>::deque(const deque<T>& other)
{
	copyFrom(other);
}

template<typename T, typename AllocatorType>
deque<T, AllocatorType>& deque<T, AllocatorType>::operator=(const deque<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T, typename AllocatorType>
deque<T, AllocatorType>::deque(deque<T>&& other)
{
	moveFrom(std::move(other));
}

template<typename T, typename AllocatorType>
deque<T, AllocatorType>& deque<T, AllocatorType>::operator=(deque<T>&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

template<typename T, typename AllocatorType>
deque<T, AllocatorType>::~deque()
{
	free();
}