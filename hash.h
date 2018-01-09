/*
NAME: Christina Sadak
PROGRAM: Program 5 (RE-USING for Program 6)
PURPOSE: Creates a hash table class. Handles collisions using linear probe.
*/

#ifndef hash_H
#define hash_H

#include<iostream>
#include<string>
#include<cstring>

using namespace std;

template<typename E>
struct HASHTABLE{string item; int itemIndex; bool isEmpty;};
template<typename E>
class hash
{
    public:
        hash();
        hash(int hashTableSize);
        void insert(E inputItem, int hashTableSize, int(*doHash)(int hashTableSize, string inputString), string inputString, int indexCounter);
        bool search(int hashTableSize, string searchItem, bool(*cmp)(string first, string second), int(*doHash)(int hashTableSize, string inputString), string& courseFound, int& indexFound);
        long int goodSearchCount();
        long int badSearchCount();
        long int totalProbesCount();
        E getItem(int position);
        int getIndex(int position);
        bool getisEmpty(int position);
        int adjArraySize();

    private:
        int hashTableSize;
        E inputItem;
        HASHTABLE<E> * hashTable;
        int hashIndex;
        long int goodSearch;
        long int badSearch;
        long int totalProbes;
        int indexCounter;

};

//method definitions

template<typename E>
hash<E> :: hash()
{
  hashTableSize = 0;
  goodSearch=0;
  badSearch=0;
  totalProbes=0;
}

//parameterized constructor that will be passed the command line argument from main that dictates size of hash table
template<typename E>
hash<E> :: hash(int hashTableSize)
{
  goodSearch=0;//initializes goodSearch to 0
  badSearch=0;//initializes badSearch to 0
  totalProbes=0;//initializes totalProbes to 0
  hashTable = new HASHTABLE<E>[hashTableSize];//array of structs one field for data item one for flag if empty
  indexCounter=0;

  for(int setBool=0; setBool<hashTableSize; setBool++)//loops through hash table and initializes the isEmpty field of the struct for each element to true
  {
     hashTable[setBool].isEmpty=true;
  }

}

//insert method calls the hashFunction and then puts each of the 200,000 items from the file into the hash table into the appropriate slot in the table
template<typename E>
void hash<E> :: insert(E inputItem, int hashTableSize, int(*doHash)(int hashTableSize, string inputString), string inputString, int indexCounter)
{

    hashIndex = doHash(hashTableSize, inputString);//calls hashFunction

    if(hashTable[hashIndex].isEmpty == true)
    {
      //if position is Empty
      hashTable[hashIndex].item = inputString;//put item in the slot
      hashTable[hashIndex].itemIndex = indexCounter;
      hashTable[hashIndex].isEmpty = false;//change the slot's isEmpty field to false since it was just filled

    }
    else//if the position was not empty (collision)
    {
      while((hashTable[hashIndex].isEmpty == false))//look until an empty slot is found
      {
         if(hashIndex==hashTableSize-1)//if we reach the end of the table before finding an empty spot, loop back to the begining and continue looking
         {
           hashIndex=0;
         }
        //go until an empty spot is FOUND
        else
        {
          hashIndex++;
        }
      }
      //broke out of while loop, so an empty spot was FOUND so insert the item here
      hashTable[hashIndex].item = inputString;
      hashTable[hashIndex].itemIndex = indexCounter;
      hashTable[hashIndex].isEmpty = false;

    }

    //cout<<hashTable[hashIndex].item<< " "<<hashTable[hashIndex].itemIndex<<endl;

}

//method that searches for the value passed in in the table. Takes the table size, the item we want to find, two functions (one to compare to string values for equality and one to hash the value) and the value we want to compare with
template <typename E>
bool hash<E> :: search(int hashTableSize, string searchItem, bool(*cmp)(string first, string second), int(*doHash)(int hashTableSize, string inputString), string& courseFound, int& indexFound)
{
  bool returnVal=true;
  string currentItem;

  hashIndex = doHash(hashTableSize, searchItem);//hashes the searchItem to then look in that position

    if(hashTable[hashIndex].isEmpty == false)//something is in the slot
    {
        if(cmp(hashTable[hashIndex].item, searchItem))//item in slot matches item we're looking for
        {
          goodSearch++;
          totalProbes++;
          courseFound = hashTable[hashIndex].item;
          indexFound = hashTable[hashIndex].itemIndex;
          return returnVal;
        }
        else//something in slot that isn't what we want
        {
          while((hashTable[hashIndex].isEmpty == false))//look until the next empty slot to see if it's there
          {
            totalProbes++;

            currentItem = hashTable[hashIndex].item;

            if(cmp(currentItem, searchItem))//if item is found before next empty slot, successful
            {
              goodSearch++;
              courseFound = hashTable[hashIndex].item;
              indexFound = hashTable[hashIndex].itemIndex;
              return returnVal;
            }

            else if(hashIndex==hashTableSize-1)
            {
              hashIndex=0;
            }

            else//next empty slot was found and item is not there, unsuccessful
            {
              hashIndex++;
            }
          }
          //broke out of while loop so we found an empty slot and the item wasnt there
          badSearch++;
          returnVal=false;
          return returnVal;
        }
    }
    else
    {
      //nothing was in initial slot
      badSearch++;
      totalProbes++;
      returnVal=false;
      return returnVal;
    }
}

//reports how many successful searches there were
template<typename E>
long int hash<E> :: goodSearchCount()
{
  return goodSearch;
}

//reports how many unsuccessful searches there were
template<typename E>
long int hash<E> :: badSearchCount()
{
    return badSearch;
}

//reports the total number of times a slot was checked
template<typename E>
long int hash<E> :: totalProbesCount()
{
    return totalProbes;
}

template<typename E>
int hash<E> :: adjArraySize()
{
    return indexCounter;
}

template<typename E>
E hash<E> :: getItem(int position)
{
  return hashTable[position].item;
}

template<typename E>
int hash<E> :: getIndex(int position)
{
  return hashTable[position].itemIndex;
}

template<typename E>
bool hash<E> :: getisEmpty(int position)
{
  return hashTable[position].isEmpty;
}

#endif
