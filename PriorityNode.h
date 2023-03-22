#ifndef __PRIORITYNODE_H_
#define __PRIORITYNODE_H_
//arwa
template < typename T>
class PriorityNode
{
private:
	T item; // A data item
	int priority;
	PriorityNode<T>* next; // Pointer to next node
public:
	PriorityNode();
	PriorityNode(const T& r_Item, const int& prior);	//passing by const ref.
	PriorityNode(const T& r_Item, PriorityNode<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setPriority(const int& prior);
	int getPriority();
	void setNext(PriorityNode<T>* nextNodePtr);
	T getItem() const;
	PriorityNode<T>* getNext() const;
}; // end Node


template < typename T>
PriorityNode<T>::PriorityNode()
{
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item, const int& prior)
{
	item = r_Item;
	priority = prior;
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item, PriorityNode<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void PriorityNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template<typename T>
void PriorityNode<T>::setPriority(const int& prior)
{
	priority = prior;
}

template<typename T>
int PriorityNode<T>::getPriority()
{
	return priority;
}

template < typename T>
void PriorityNode<T>::setNext(PriorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T PriorityNode<T>::getItem() const
{
	return item;
}

template < typename T>
PriorityNode<T>* PriorityNode<T>::getNext() const
{
	return next;
}

#endif