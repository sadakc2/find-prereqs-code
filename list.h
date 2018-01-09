/*
NAME: Christina Sadak
CLASS: CSCI 271
PROGRGAM: 2 (RE-USING for Program 6)
DUE DATE: 10/19/2017
PURPOSE: reverse engineers the STL class. Includes list class, node class, and iterator class.
*/

#ifndef list_H
#define list_H

#include<iostream>
using namespace std;

template<typename E> class iter;//prototypes iter class

template<typename E> class list;//prototypes list class


template< typename E >//makes the node class a template
class Node
{
        public:

                Node(E, Node*, Node*);//constructor that takes data type and Node Ptr
                Node(){};//constructor that takes data type and Node Ptr
                E& getData();//returns the data stored in node
                void setData(E data);
                Node<E>* getNext();
                Node<E>* getPrev();
                void setNext(Node* next);
                void setPrev(Node* prev);

        private:
                E data;//stores data
                Node<E>*  next;
                Node<E>* prev;
};



template< typename E >//makes the node class a template
class iter
{
        public:
                friend class list<E>;//allows list to be iter's friend
                iter<E> operator++();//pre ++
                iter<E>  operator++(int);//post ++
                iter<E> operator--(int); //post --
                iter<E> operator--();//pre --
                bool operator==(iter<E>);//comparative equals
                bool operator!=(iter<E>);//comparative not equals
                E operator*();//dereference

        private:
                Node<E> *iteratorPtr;
};


template<typename E>//makes list a template
class list
{

	friend class iter<E>;//allows iter to be list's friend

  	public:

  		list();//constructor
  		list(const list<E>&, const list<E>&);//copy constructor
 		~list();//destructor
 		bool empty();
 		void push_front(E);
 		void push_back(E);
 		void pop_front();
 		void pop_back();
  		E& front();//returns reference to first item in list
  		E& back();//returns reference to last item in list
  		iter<E> begin();//returns an iterator to first thing in list
  		iter<E> end();//returns iterator to after last thing in list
  		int size();
  		void print_all();
  		void erase(iter<E>);//erases the iterator passed in
  		void clear();
  		void operator=(const list<E>&);//similar to copy constructor


	private:
 		Node<E>* dummyHeadPtr;
 		Node<E>* dummyTailPtr;
  		int listSize;

};

//BEGINING LIST METHOD DEFINITIONS
template<typename E>
list<E> :: list()//constructor
{
  dummyHeadPtr = new Node<E>;
  dummyHeadPtr->setPrev(dummyHeadPtr);//sets dummyHead's previous to itself to keep from falling off front of list

  dummyTailPtr = new Node<E>;
  dummyTailPtr->setNext(dummyTailPtr);//sets dummyTail's next to itself to keep from falling off end of list

  dummyHeadPtr->setNext(dummyTailPtr);//sets dummyHead's next to dummyTail
  dummyTailPtr->setPrev(dummyHeadPtr);//sets dummyTail's previous to dummyHead

  listSize = 0;//sets list size equal to zero even though there are really two nodes in the list. The user doesn't care about dummyHead and dummyTail though.
}

template<typename E>
list<E> :: list(const list<E>& originalList, const list<E>& passedList)//copy constructor
{
    originalList = passedList;//calls the operator=() method to create another list that is a copy of the original list
}

template<typename E>
list<E> :: ~list()
{
    clear();//calls the clear method to pop_front nodes until the list size is zero. Once list size is zero, then dummyHead and dummyTail get deleted.

    delete dummyHeadPtr;
    delete dummyTailPtr;

}

//operator= method- copies one linked list into another
template<typename E>
void list<E> :: operator=(const list<E>& passedList)
{
  listSize = 0;

  //creates two initial dummyNodes like the default constructor
  dummyHeadPtr = new Node<E>;
  dummyHeadPtr->setPrev(dummyHeadPtr);

  dummyTailPtr = new Node<E>;
  dummyTailPtr->setNext(dummyTailPtr);

  dummyHeadPtr->setNext(dummyTailPtr);
  dummyTailPtr->setPrev(dummyHeadPtr);

  Node<E>* loopPtr;
  loopPtr = passedList.dummyHeadPtr->getNext();//sets loopPtr equal to the passedList's dummyHead's next.

  while(loopPtr != passedList.dummyTailPtr)//loops through until the loopPtr's next equals the passedList's dummyTail
  {
    push_back(loopPtr->getData());//gets the data from loopPtr and pushes it into the new list.
    loopPtr = loopPtr->getNext();//goes to next item in the list
  }

}

//empty method- checks to see if the list is empty before performing operations to prevent an error
template<typename E>
bool list<E>:: empty()
{
  Node<E>* testPtr;
  testPtr = dummyHeadPtr->getNext();
  if(testPtr == dummyTailPtr)
  {
    return true;
  }//returns true if the linked list is empty
	else
		return false;//returns false if there is anything in the linked list
}//ends empty method

//push_front method- Inserts a node at the front of the list
template<typename E>
void list<E>::push_front(E value)
{

  Node<E>* newPtr;
  Node<E>* tempPtr;

  tempPtr = dummyHeadPtr->getNext();//saves pointer to the node after dummyHead

  newPtr = new Node<E>(value, tempPtr, tempPtr->getPrev());//creates a new node. Constructor takes parameters in this order: data value, next, previous

  dummyHeadPtr->setNext(newPtr);//sets dummyHead's next to the new pointer to insert the node right after dummyHead
  tempPtr -> setPrev(newPtr);//sets tempPtr's previous to the new node

  listSize++;//increments size since a node was added to the list
}//ends push_front function

//push_back method- Inserts a node at the end of the list
template<typename E>
void list<E> ::  push_back(E value)
{
    Node<E>* newPtr;
    Node<E>* tempPtr;

    tempPtr = dummyTailPtr->getPrev();//saves pointer to dummyTail's original previous
    newPtr = new Node<E>(value, dummyTailPtr, tempPtr);//creates new node

    dummyTailPtr->setPrev(newPtr);//sets dummyTail's new previous to the newPtr
    tempPtr->setNext(newPtr);//sets dummyTail's original previous's next to the new node

  listSize++;//increments size since node was added to the list

}

//pop_back method- removes the last value in the list
template<typename E>
void list<E> :: pop_back()
{
    Node<E>* tempPtr1;
    Node<E>* tempPtr2;

    if(empty())
    {
      cout<<"Cannot pop. Empty List. Exiting."<<endl;
      return;
    }//alerts if at end of list since we can't pop anything off an empty list

    tempPtr1 = dummyTailPtr->getPrev();//stores dummyTail's pointer to its previous
    tempPtr2=tempPtr1->getPrev();//stores pointer to the node before dummyTail's previous (two before dummyTail)

    dummyTailPtr->setPrev(tempPtr2);//sets dummyTail's previous to be the node originally two before dummyTail
    tempPtr2->setNext(dummyTailPtr);//sets dummyTail's new previous's next to point to the dummyTail

    delete tempPtr1;//delets the node that was originally before the dummyTail

    listSize--;//decrements listSize since a node was removed from list
}

//pop_front method- removes the first value in the list
template<typename E>
void list<E> :: pop_front()
{
  if(empty())
  {
    return;
  }

  Node<E>* tempPtr1;
  Node<E>* tempPtr2;

  tempPtr1 = dummyHeadPtr->getNext();
  tempPtr2 = tempPtr1->getNext();

  dummyHeadPtr->setNext(tempPtr2);
  tempPtr2->setPrev(dummyHeadPtr);

  delete tempPtr1;//deleted node immediately after dummyHead

  listSize--;//decrements listSize since a node was removed

}

//begin returns an iterator to the first item in the list
template<typename E>
iter<E> list<E> :: begin()
{
	iter<E> beginItr;//declares an iterator

	beginItr.iteratorPtr = dummyHeadPtr->getNext();//sets the iterator to dummyHead's next using object.part from the iterator class

	return beginItr;
}

//end returns an iterator to after the last item in the list
template<typename E>
iter<E> list<E> :: end()
{
	iter<E> endItr;
	endItr.iteratorPtr = dummyTailPtr;//sets the iterator to dummyTailusing object.part from iterator class

	return endItr;
}

//clear method- removes all items from list (except dummyHead and dummyTail)
template<typename E>
void list<E> :: clear()
{
  for(int i=listSize; i>=0; i--)
  {
    pop_front();//pops items off the top of the list until the size goes down to zero. Pop_front takes care of deleting the node and decrementing list size
  }
}

//front method- returns a reference to the first item in the list
template<typename E>
E& list<E> :: front()
{
  return dummyHeadPtr->getNext()->getData();//returns dummyHead's next's data and the return type makes it a reference
}

//back method- returns a reference to the last item in the list
template<typename E>
E& list<E> :: back()
{
  return dummyTailPtr->getPrev()->getData();//returns dummyTail's previous's data and the return type makes it a reference
}

//print_all method prints out list
template<typename E>
void list<E>::print_all()
{
	if(empty())//if the list is empty, it states that and kicks out of the method
	{
		cout<<"Cannot print. The list is empty."<<endl;
		return;
	}//end if statement


	Node<E>* nextPtr;
  	nextPtr = dummyHeadPtr->getNext();
  	while(nextPtr!=dummyTailPtr->getNext())
  	{
  		cout<<nextPtr->getData()<<endl;
    		nextPtr = nextPtr->getNext();
  	}

}//end print function

//erase method- removes the item iterator indicates from the list
template<typename E>
void list<E> :: erase(iter<E> iteratorE)//tales an iterator from main
{
  	Node<E>* tempPtr;
	Node<E>* tempPtr1;
  	Node<E>* tempPtr2;

  	tempPtr = iteratorE.iteratorPtr;//sets a tempPtr to the iterator's pointer part

	tempPtr1 = tempPtr->getNext();//goes to item after the iterator
	tempPtr2 = tempPtr->getPrev();//goes to item before iterator

	tempPtr1->setPrev(tempPtr2);
	tempPtr2->setNext(tempPtr1);

  	delete tempPtr;//deletes appropriate item
  	listSize--;//decrements listSize
}

//size method- returns the number of values in the list
template<typename E>
int list<E> :: size()
{
  return listSize;
}


//BEGINING NODE METHOD DEFINITIONS

//Node constructor- creates nodes that have three fields: data, next, previous
template< typename E >
Node< E >::Node(E item, Node<E>* nextPtr, Node<E>* prevPtr)
{
        data = item;
        next = nextPtr;
        prev = prevPtr;
}//end LinkedListNode constructor

//getData method
template< typename E >
E& Node< E > :: getData()
{
        return data;
}//end function getData

//setData Method
template< typename E >
void Node<E> :: setData(E item)
{
         data = item;
}

//getNext method
template< typename E >
Node<E>* Node<E> :: getNext()
{
        return next;
}

//getPrev method
template< typename E >
Node<E>* Node<E> :: getPrev()
{
        return prev;
}

//setNext method
template< typename E >
void Node<E> :: setNext(Node* nextPtr)
{
        next = nextPtr;
}

//setPrev method
template< typename E >
void Node<E> :: setPrev(Node* prevPtr)
{
        prev = prevPtr;
}

//BEGINING ITER  METHOD DEFINITIONS

//overloads pre++ operator
template<typename E>
iter<E> iter<E> :: operator++()//pre increment. moves iterator to the next and returns modified val of iterator
{

    iteratorPtr = iteratorPtr->getNext();//moves iteratorPtr to the next node
    return *this;//returns current dereferenced pointer

}

//overloads post++
template<typename E>
iter<E> iter<E> :: operator++(int)//post increment. moves iterator to the next and returns original value of iterator
{
  	iter<E> tempItr;
	tempItr = *this;//temperary iterator stores the value of current iterator
	iteratorPtr = iteratorPtr->getNext();//moves iterator the the next after storing the value from the previous location of iterator

	return tempItr;//returns original value of iterator
}

//pre-- method
template<typename E>
iter<E> iter<E> :: operator--()//pre decrement. move iterator to the previous
{

	iteratorPtr = iteratorPtr->getPrev();

	return *this;
}

//post--
template<typename E>
iter<E> iter<E> :: operator--(int)//post decrement. move iterator to the previous but return the original iterator before moving
{
	iter<E> tempItr;
	tempItr = *this;
	iteratorPtr = iteratorPtr->getPrev();

	return tempItr;
}

//overloads ==
template<typename E>
bool iter<E> :: operator==(iter<E> compareItr)//return true or false based on iterator comparison.
{
  if(iteratorPtr == compareItr.iteratorPtr)//checks to see if the pointer in the list is equal to the pointer part of the iterator passed in
    return true;
  else
    return false;
}

//overloads !=
template<typename E>
bool iter<E> :: operator!=(iter<E> compareItr)//return true or false based on iterator comparison.
{
  if(iteratorPtr != compareItr.iteratorPtr)//checks to see if the pointer in the list not is equal to the pointer part of the iterator passed in
    return true;
  else
    return false;
}

//overloads dereference operator
template<typename E>
E iter<E> :: operator*()//returns the value in the node pointed to by the iterator
{
	E returnVal;

	returnVal = iteratorPtr->getData();//gets and stores the value at the pointer

	return returnVal;
}

#endif
