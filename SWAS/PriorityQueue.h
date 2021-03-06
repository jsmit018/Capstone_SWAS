//PriorityQueue.h: Jordan Smith
#ifndef PriorityQueue_H
#define PriorityQueue_H

#include <iostream>
#include <string>
#include "SimExec.h"

using namespace std; 

/*
Prioirty Queue - Aircraft Arrives with a Priority and is added the list accordingly
Defined using a template so that can be used to hold anything.  Type supplied when object is instantiated.
Template structure requires implementation to be provided in the header.
*/
template <class T>
class PriorityQueue
{
public:
	PriorityQueue(string name)
	{
		_head = 0; 
		_tail = 0; 
		_size = 0; 
		_name = name; 
	}

	void AddEntity(T* t, int priority)
	{
		Node* node = new Node(t, priority); 
		if (_head == 0) {	//empty list
			_head = _tail = node; 
		}
		else if (_head->_priority < priority) {
			//_tail = _tail->next = node; 
			node->next = _head; 
			_head = node; 
		}
		else {
			Node* holder = _head; 
			while ((holder != nullptr) ? (node->_priority <= holder->_priority) : false) {
				holder = holder->next; 
			}
			if (holder->next == 0)
				holder->next = node; 
			else {
				node->next = holder->next; 
				holder->next = node; 
			}

		}
		//cout << SimObj::GetSimulationTime() << ", queue " << _name << ", AddEntity, Entity , queue size, " << _size << endl; 
		_size++; 
		//cout << SimObj::GetSimulationTime() << ", queue " << _name << ", AddEntity, Entity , queue size, " << _size << endl; 
	}

	T* GetEntity()
	{
		if (_head == 0) return 0; 
		else {
			Node* n = _head; 
			T* t = _head->t; 
			_head = _head->next; 
			//			delete n; 
//			cout << SimObj::GetSimulationTime() << ", queue " << _name << ", GetEntity, Entity , queue size, " << _size << endl; 
			_size--; 
			//			cout << SimObj::GetSimulationTime() << ", queue " << _name << ", GetEntity, Entity , queue size, " << _size << endl; 
			return t; 
		}
	}

	T* ViewEntity()
	{
		return(_head->t); 
	}

	bool IsEmpty() { return (_size == 0);}
	int GetSize() { return _size;}
private:
	struct Node
	{
		Node(T* t, int priority) {
			this->t = t; 
			next = 0; 
			_priority = priority; 
		}
		T* t; 
		Node* next; 
		int _priority; 
	}; 

	Node* _head; 
	Node* _tail; 
	int _size; 
	string _name; 
}; 

#endif