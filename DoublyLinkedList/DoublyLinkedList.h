#pragma once
#include <iostream>

template<typename T>
class DoublyLinkedList
{
	struct Node
	{
		T data;
		Node* next;
		Node* prev;

		Node(const T& data) : data(data), prev(nullptr), next(nullptr) {}
	};

	Node* head = nullptr;
	Node* tail = nullptr;
	size_t count = 0;

	void copyFrom(const DoublyLinkedList& other);
	void moveFrom(DoublyLinkedList&& other);
	void free();

public:

	DoublyLinkedList() = default;
	DoublyLinkedList(const DoublyLinkedList& other);
	DoublyLinkedList(DoublyLinkedList&& other);
	DoublyLinkedList& operator=(const DoublyLinkedList& other);
	DoublyLinkedList& operator=(DoublyLinkedList&& other);
	~DoublyLinkedList();

	void push_back(const T& data);
	void push_back(T&& data);
	void push_front(const T& data);
	void push_front(T&& data);

	void pop_back();
	void pop_front();

	const T& front() const;
	const T& back() const;

	T& front();
	T& back();

	void print() const;
	size_t getSize() const;

	bool empty() const;

	void clear();

	class DllIterator
	{
		friend class DoublyLinkedList;
		Node* currentElementPtr;
		DoublyLinkedList& list; // ??

	public:

		typedef std::bidirectional_iterator_tag iterator_category;

		DllIterator(DoublyLinkedList& list, Node* currentElementPtr = nullptr) : list(list), currentElementPtr(currentElementPtr) {}

		T& operator*()
		{
			return currentElementPtr->data;
		}
		T* operator->()
		{
			return &currentElementPtr->data;
		}
		DllIterator& operator++()
		{
			if (currentElementPtr)
				currentElementPtr = currentElementPtr->next;
			return *this;
		}
		DllIterator operator++(int)
		{
			DllIterator temp(*this);
			++(*this);
			return temp;
		}
		DllIterator& operator--()
		{
			if (currentElementPtr)
				currentElementPtr = currentElementPtr->prev;
			else
				currentElementPtr = list.tail;
			return *this;
		}
		DllIterator operator--(int)
		{
			DllIterator temp(*this);
			--(*this);
			return temp;
		}
		bool operator==(const DllIterator& other) const
		{
			return currentElementPtr == other.currentElementPtr;
		}
		bool operator!=(const DllIterator& other)
		{
			return !(*this == other);
		}
	};
	class ConstDllIterator
	{
		friend class DoublyLinkedList;

		Node* currentElementPtr;
		const DoublyLinkedList& list;

	public:

		typedef std::bidirectional_iterator_tag iterator_category

		ConstDllIterator(const DoublyLinkedList& list, Node* currentElementPtr = nullptr) : list(list), currentElementPtr(currentElementPtr) {}
		ConstDllIterator(const DllIterator& iter) : list(iter.list), currentElementPtr(iter.currentElementPtr) {}

		const T& operator*() const
		{
			return currentElementPtr->data;
		}
		const T* operator->() const
		{
			return &currentElementPtr->data;
		}
		ConstDllIterator& operator++()
		{
			if (currentElementPtr)
				currentElementPtr = currentElementPtr->next;
			return *this;
		}
		ConstDllIterator operator++(int)
		{
			ConstDllIterator temp(*this);
			++(*this);
			return temp;
		}
		ConstDllIterator& operator--()
		{
			if (currentElementPtr)
				currentElementPtr = currentElementPtr->prev;
			else
				currentElementPtr = list.tail;
			return *this;
		}
		ConstDllIterator operator--(int)
		{
			ConstDllIterator temp(*this);
			--(*this);
			return temp;
		}
		bool operator==(const ConstDllIterator& other) const
		{
			return currentElementPtr == other.currentElementPtr;
		}
		bool operator!=(const ConstDllIterator& other) const
		{
			return !(*this == other);
		}
	};

	DllIterator insert(const T& el, const ConstDllIterator& it);
	DllIterator remove(const DllIterator& it);

	DllIterator begin()
	{
		return DllIterator(*this, head);
	}
	DllIterator end()
	{
		return DllIterator(*this, nullptr);
	}
	ConstDllIterator cbegin() const
	{
		return ConstDllIterator(*this, head);
	}
	ConstDllIterator cend() const
	{
		return ConstDllIterator(*this, nullptr);
	}
};

template<typename T>
void DoublyLinkedList<T>::copyFrom(const DoublyLinkedList& other)
{
	Node* otherIter = other.head;
	while (otherIter)
	{
		push_back(otherIter->data);
		otherIter = otherIter->next;
	}
}

template<typename T>
void DoublyLinkedList<T>::moveFrom(DoublyLinkedList&& other)
{
	head = other.head;
	tail = other.tail;
	count = other.count;

	other.head = other.tail = nullptr;
	other.count = 0;
}

template<typename T>
void DoublyLinkedList<T>::free()
{
	Node* current = head;

	while (current)
	{
		Node* toDelete = current;
		current = current->next;
		delete toDelete;
	}
	
	head = tail = nullptr;
	count = 0;
}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr)
{
	copyFrom(other);
}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& other)
{
	moveFrom(std::move(other));
}

template<typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	free();
}

template<typename T>
void DoublyLinkedList<T>::push_back(const T& data)
{
	Node* newNode = new Node(data);

	if (!head)
	{
		head = tail = newNode;
	}
	else
	{
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
	++count;
}

template<typename T>
void DoublyLinkedList<T>::push_back(T&& data)
{
	Node* newNode = new Node(std::move(data));

	if (!head)
	{
		head = tail = newNode;
	}
	else
	{
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
	++count;
}

template<typename T>
void DoublyLinkedList<T>::push_front(const T& data)
{
	Node* newNode = new Node(data);

	if (!head)
	{
		head = tail = newNode;
	}
	else
	{
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}

	++count;
}

template<typename T>
void DoublyLinkedList<T>::push_front(T&& data)
{
	Node* newNode = new Node(std::move(data));

	if (!head)
	{
		head = tail = newNode;
	}
	else
	{
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}

	++count;
}

template<typename T>
void DoublyLinkedList<T>::pop_back()
{
	if (empty())
		throw std::runtime_error("Empty list!");
	if (head == tail)
	{
		delete head;
		head = tail = nullptr;
	}
	else
	{
		tail->prev->next = nullptr;
		Node* toDelete = tail;
		tail = tail->prev;
		delete toDelete;
	}
	--count;
}

template<typename T>
void DoublyLinkedList<T>::pop_front()
{
	if (empty())
		throw std::runtime_error("Empty list!");
	if (head == tail)
	{
		delete head;
		head = tail = nullptr;
	}
	else
	{
		head->next->prev = nullptr;
		Node* toDelete = head;
		head = head->next;
		delete toDelete;
	}
	--count;
}

template<typename T>
const T& DoublyLinkedList<T>::front() const
{
	if (isEmpty())
		throw std::runtime_error("The list is empty!");
		return head->data;
}

template<typename T>
const T& DoublyLinkedList<T>::back() const
{
	if (isEmpty())
		throw std::runtime_error("The list is empty!");
	return tail->data;
}

template<typename T>
T& DoublyLinkedList<T>::front()
{
	if (isEmpty())
		throw std::runtime_error("The list is empty!");
	return head->data;
}

template<typename T>
T& DoublyLinkedList<T>::back()
{
	if (isEmpty())
		throw std::runtime_error("The list is empty!");
	return tail->data;
}

template<typename T>
void DoublyLinkedList<T>::print() const
{
	Node* current = head;
	while (current)
	{
		std::cout << head->data << ' ';
		if (head->next)
			std::cout << "<-> ";
		current = current->next;
	}
	std::cout << std::endl;
}

template<typename T>
size_t DoublyLinkedList<T>::getSize() const
{
	return count;
}

template<typename T>
bool DoublyLinkedList<T>::empty() const
{
	return getSize() == 0;
}

template<typename T>
void DoublyLinkedList<T>::clear()
{
	free();
}

template<typename T>
typename DoublyLinkedList<T>::DllIterator DoublyLinkedList<T>::insert(const T& element, const ConstDllIterator& it)
{
	if (it == cbegin())
	{
		push_front(el);
		return begin();
	}
	else if (it == cend())
	{
		push_back(el);
		return DllIterator(*this, tail);
	}
	else
	{
		Node* current = it.currentElementPtr;
		Node* newNode = new Node(el);
		
		current->prev->next = newNode;
		newNode->prev = current->prev;
		newNode->next = current;
		current->prev = newNode;

		++count;
		
		return DllIterator(*this, newNode);
	}
}

template<typename T>
DoublyLinkedList<T>::DllIterator DoublyLinkedList<T>::remove(const DllIterator& it)
{
	Node* toDelete = it.currentElementPtr;
	
	if (!toDelete)
		throw std::runtime_error("Cannot remove end iterator!");
	if (toDelete == head)
	{
		pop_front();
		return begin();
	}
	else if (toDelete == tail)
	{
		pop_back();
		return end();
	}
	else
	{
		toDelete->prev->next = toDelete->next;
		toDelete->next->prev = toDelete->prev;
		Node* nextNode = toDelete->next;
		delete toDelete;
		--count;

		return DllIterator(*this, nextNode);
	}
}
