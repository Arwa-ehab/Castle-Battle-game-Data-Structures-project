#ifndef __PRIORITYQUEUE_H_
#define __PRIORITYQUEUE_H_
#include "PriorityNode.h"
//arwa
template <typename T>
class PriorityQueue
{
private:

	PriorityNode<T>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, int priority);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;

	//toArray function to be used ONLY when drawing the queue items
	const T* toArray(int& count);	//returns array of T (array of items)


	~PriorityQueue();
};
template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	
	frontPtr = nullptr;

}
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	if (frontPtr == nullptr)
		return true;
	else
		return false;
}
template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, int priority)
{
	PriorityNode<T>* newNodePtr = new PriorityNode<T>(newEntry, priority);
	// Insert the new node
	if (isEmpty())
	{
		frontPtr = newNodePtr; // The queue is empty
		return true;
	}
	else if (!frontPtr->getNext() || frontPtr->getPriority() < newNodePtr->getPriority())
	{
		if (frontPtr->getPriority() > newNodePtr->getPriority())
		{
			newNodePtr->setNext(frontPtr->getNext());
			frontPtr->setNext(newNodePtr);
	
			return true;
		}
		else {
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
		
			return true;
		}
	}
	else {
		PriorityNode<T>* start = frontPtr;
		PriorityNode<T>* next = frontPtr->getNext();
		while (next && next->getPriority() > newNodePtr->getPriority())
		{
			start = start->getNext();
			next = next->getNext();
		}
		newNodePtr->setNext(next);
		start->setNext(newNodePtr);

	}
	return true;

}
template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PriorityNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	delete nodeToDeletePtr;

	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/
template <typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/

//IMPORTANT:
//toArray function to be used ONLY when drawing the queue items

template <typename T>
const T* PriorityQueue<T>::toArray(int& count)
{

	//IMPORTANT:
	//toArray function to be used ONLY when drawing the queue items

	count = 0;

	if (!frontPtr)
		return nullptr;
	//counting the no. of items in the Queue
	PriorityNode<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr = new T[count];
	p = frontPtr;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
	//IMPORTANT:
	//toArray function to be used ONLY when drawing the queue items

}
#endif


