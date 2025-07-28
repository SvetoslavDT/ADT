#pragma once
#include <iostream>

template<typename T>
class SinglyLinkedList
{

	struct Node
	{
		T data;
		T* next;

		Node(const T& data)
		{
			this->data = data;
			next = nullptr;
		}
	};

	Node* head = nullptr;
	Node* tail = nullptr;
	size_t size = 0;

public:

	SinglyLinkedList() = default;
	SinglyLinkedList(const SinglyLinkedList<T>& other);
	SinglyLinkedList(SinglyLinkedList<T>&& other);
	SinglyLinkedList& operator=(const SinglyLinkedList<T>& other);
	SinglyLinkedList& operator=(SinglyLinkedList<T>&& other);
	~SinglyLinkedList();

	void push_front(const T& data);
	void push_front(T&& data);
	void push_back(const T& data);
	void push_back(T&& data);

	void pop_front();

	T& front();
	T& back();

	const T& front() const;
	const T& back() const;

	size_t getSize() const;
	bool empty() const;

	template<typename U>
	friend SinglyLinkedList<U> concat(SinglyLinkedList<U>& lhs, SinglyLinkedList<U>& rhs);

	void print() const;

	class SllIterator
	{
		Node* currentElementPtr;

		friend class SinglyLinkedList;

	public:

		typedef std::forward_iterator_tag iterator_category;

		SllIterator(Node* currentElementPtr = nullptr) : currentElementPtr(currentElementPtr) {}

		T& operator*()
		{
			return currentElementPtr->data;
		}
		T* operator->()
		{
			return &currentElementPtr->data;
		}
		SllIterator& operator++()
		{
			if(currentElementPtr)
				currentElementPtr = currentElementPtr->next;
			return *this;
		}
		SllIterator operator++(int)
		{
			SllIterator temp(*this);
			++(*this);
			return temp;
		}
		SllIterator& operator+=(size_t off)
		{
			while (off--)
				++(*this);

			return *this;
		}
		SllIterator operator+(int i) const
		{
			SllIterator res(*this);
			return res += i;
		}
		bool operator==(const SllIterator& other) const
		{
			return currentElementPtr == other.currentElementPtr;
		}
		bool operator!=(const SllIterator& other) const
		{
			return !(*this == other);
		}
	};
	class ConstSllIterator
	{
		Node* currentElementPtr;

		friend class SinglyLinkedList;

	public:

		typedef std::forward_iterator_tag iterator_category;

		ConstSllIterator(const SllIterator& nonConstIter) : currentElementPtr(nonConstIter.currentElementPtr) {}
		ConstSllIterator(Node* currentElementPtr = nullptr) : currentElementPtr(currentElementPtr) {}

		T& operator*()
		{
			return currentElementPtr->data;
		}
		T* operator->()
		{
			return &currentElementPtr->data;
		}
		ConstSllIterator& operator++()
		{
			if (currentElementPtr)
				currentElementPtr = currentElementPtr->next;
			return *this;
		}
		ConstSllIterator operator++(int)
		{
			ConstSllIterator temp(*this);
			++(*this);
			return temp;
		}
		ConstSllIterator& operator+=(size_t off)
		{
			while (off--)
				++(*this);

			return *this;
		}
		ConstSllIterator operator+(int i)
		{
			ConstSllIterator result(*this);;
			return result += i;
		}
		bool operator==(const ConstSllIterator& other) const
		{
			return currentElementPtr == other.currentElementPtr;
		}
		bool operator!=(const ConstSllIterator& other) const
		{
			return !(*this == other);
		}
	};

	ConstSllIterator begin() // ?? tova zashto be Dimi
	{
		return SllIterator(head);
	}
	SllIterator end()
	{
		return SllIterator(nullptr);
	}
	ConstSllIterator cbegin() const
	{
		return ConstSllIterator(head);
	}
	ConstSllIterator cend() const
	{
		return ConstSllIterator(nullptr);
	}

	SllIterator insertAfter(const T& element, const ConstSllIterator& it);
	SllIterator removeAfter(const ConstSllIterator& it);

	private:

		void copyFrom(const SinglyLinkedList<T>& other);
		void moveFrom(SinglyLinkedList<T>&& other);
		void free();
};

template<typename T>
void SinglyLinkedList<T>::free()
{
	Node* cur = head;

	while (cur)
	{
		Node* toDelete = cur;
		cur = cur->next;
		delete toDelete;
	}

	head = tail = nullptr;
	size = 0;
}

template<typename T>
void SinglyLinkedList<T>::copyFrom(const SinglyLinkedList<T>& other)
{
	Node* cur = other.head;

	while (cur)
	{
		push_back(cur->data);
		cur = cur->next;
	}
}

template<typename T>
void SinglyLinkedList<T>::moveFrom(SinglyLinkedList<T>&& other)
{
	head = other.head;
	tail = other.tail;
	size = other.size;

	other.head = nullptr;
	other.tail = nullptr;
	other.size = 0;
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other)
{
	copyFrom(other);
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& other)
{
	moveFrom(std::move(other));
}

template<typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
template<typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList<T>&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

template<typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
	free();
}

template<typename T>
void SinglyLinkedList<T>::push_front(const T& data)
{
	Node* newNode = new Node(data);
	if (empty())
	{
		head = tail = newNode;
	}
	else
	{
		newNode->next = head;
		head = newNode;
	}
	++size;
}

template<typename T>
void SinglyLinkedList<T>::push_front(T&& data)
{
	Node* newNode = new Node(std::move(data));
	if (empty())
	{
		head = tail = newNode;
	}
	else
	{
		newNode->next = head;
		head = newNode;
	}
	++size;
}

template<typename T>
void SinglyLinkedList<T>::push_back(const T& data)
{
	Node* newNode = new Node(data)

	if (empty())
	{
		head = newNode;
	}
	else
	{
		tail->next = newNode;
	}

	tail = newNode;
	++size;
}

template<typename T>
void SinglyLinkedList<T>::push_back(T&& data)
{
	Node* newNode = new Node(std::move(data));

		if (empty())
		{
			head = newNode;
		}
		else
		{
			tail->next = newNode;
		}

	tail = newNode;
	++size;
}

template<typename T>
void SinglyLinkedList<T>::pop_front()
{
	if (!head)
		throw std::logic_error("Empty list!");
	
	if (head == tail)
	{
		delete head;
		head = tail = nullptr;
	}
	else
	{
		Node* toDelete = head;
		head = head->next;
		delete toDelete;
	}

	--size;
}

template<typename T>
T& SinglyLinkedList<T>::front()
{
	if (!head)
		throw std::logic_error("Empty list!");
	return head->data;
}

template<typename T>
T& SinglyLinkedList<T>::back()
{
	if (!tail)
		throw std::logic_error("Empty list!");
	return tail->data;
}

template<typename T>
const T& SinglyLinkedList<T>::front() const
{
	if (!head)
		throw std::logic_error("Empty list!");
	return head->data;
}

template<typename T>
const T& SinglyLinkedList<T>::back() const
{
	if (!tail)
		throw std::logic_error("Empty list!");
	return tail->data;
}

template<typename T>
size_t SinglyLinkedList<T>::getSize() const
{
	return size;
}

template<typename T>
bool SinglyLinkedList<T>::empty() const
{
	return getSize() == 0;
}

template<typename T>
void SinglyLinkedList<T>::print() const
{
	Node* cur = head;

	while (cur)
	{
		std::cout << cur->data << ' ';
		if (cur->next)
			std::cout << "->" << ' ';
		cur = cur->next;
	}

	std::cout << std::endl;
}

template<typename T>
SinglyLinkedList<T> concat(SinglyLinkedList<T>& lhs, SinglyLinkedList<T>& rhs)
{
	SinglyLinkedList<T> result;

	if (!lhs.head)
	{
		result.head = rhs.head;
		result.tail = rhs.tail;
	}
	else if (!rhs.head)
	{
		result.head = lhs.head;
		result.tail = lhs.tail;
	}
	else
	{
		lhs.tail->next = rhs.head;
		result.head = lhs.head;
		result.tail = rhs.tail;
	}

	result.size = rhs.getSize() + lhs.getSize();
	rhs.size = lhs.size = 0;
	rhs.head = rhs.tail = lhs.head = lhs.tail = nullptr;

	return result;
}

template<typename T>
typename SinglyLinkedList<T>::SllIterator SinglyLinkedList<T>::insertAfter(const T& element, const typename SinglyLinkedList<T>::ConstSllIterator& it)
{
	if (it == end())
		return end();

	Node* newNode = new Node(element);
	Node* itNode = it->currentElementPtr;

	newNode->next = itNode->next;
	itNode->next - newNode;

	++size;

	return SinglyLinkedList<T>::SllIterator(newNode);
}

template<typename T>
typename SinglyLinkedList<T>::SllIterator SinglyLinkedList<T>::removeAfter(const typename SinglyLinkedList<T>::ConstSllIterator& it) // ?? Cqlata funkciq i ima neshto
{
	if (it == end() || getSize() == 1)
		return end();

	Node* toDelete = (it + 1).currentElementPtr;

	Node* newNext = toDelete->next;

	it.currentElementPtr->next = newNext;
	size--;

	if (toDelete == tail)
	{
		tail = it.currentElementPtr;
	}

	delete toDelete;

	return SinglyLinkedList<T>::SllIterator(newNext);
}