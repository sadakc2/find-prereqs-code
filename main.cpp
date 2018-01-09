/*
NAME: Christina Sadak
PROGRAM: Program 6
DUE DATE: December 13th, 2017 @ 12PM
PURPOSE: Given the courses needed for graduation and their prerequisites, create a list of courses in an order that will
  allow the student to progress successfully.
        Prints out a list of courses such that the prerequisites for every course appear before that course.
*/

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstring>
#include "list.h"
#include "hash.h"

//declares a struct that will include a course's name and index in the adjacency list
struct GRAPHITEM{string course; int courseIndex;};

int hashFunction(int tableSize, string inputString);
bool stringCompare(string currentString, string searchString);

void printMain(list<GRAPHITEM> *adjList, int arraySize);

using namespace std;

int main()
{
  cout<<endl;
  cout<<"Program6"<<endl;
  cout<<endl;

  int tableSize = 1000;//sets the hashTable size to 1000 because the maximum number of items in the hashTable will be 500


  string inputString;
  ifstream classFile;
  char filename[20];
  cout<<"Enter the filename: ";
  cin>>filename;
  classFile.open(filename, ios::in);//open's thacker's file

  if(!classFile.is_open())//checks to make sure the file can be opened. Exits if not.
  {
    cout<<"Failed opening file. Exiting."<<endl;
    return 1;
  }
  else//continues with the code if the file is successfully opened
  {
    cout<<endl;
    cout<<"COURSES: "<<endl;

    hash<GRAPHITEM> myHashTable(tableSize);//creates a hashTable object that will be of type GRAPHITEM (a struct)

    string foundCourse;
    int foundIndex;
    int insertIndex;
    string searchString;
    int indexCounter = 0;
    int indegreeCount = 0;
    int hashTableSize;

    //creates three objects of type GRAPHITEM
    GRAPHITEM graphItemBASE;
    GRAPHITEM graphItemPREREQ;
    GRAPHITEM graphItemTOPSORT;

    //make adjList and indegreeArray
    list<GRAPHITEM> *adjList;
    int arraySize = 500;//sets array size to 500 because there won't be more than 500 courses to read in
    adjList = new list<GRAPHITEM>[arraySize];

    /*
    intitially sets each indegreeArray to -1 so that it can be incrememnted easily, but also so that all of the slots with zeros going into the
    zeroIndegreeLL truly have no incoming edges and weren't set to zero by default
    */
    int indegreeArray[arraySize];
    for(int i=0; i<arraySize; i++)
    {
      indegreeArray[i] = -1;
    }

    list<int> zeroIndegreeLL;//creates a list object using my doubly-linked list class

    string baseCourse;
    string prereqCourse;
    bool checkSearch;
    int baseCourseIndex;
    string baseCourseName;

    while(classFile>>inputString)//reads courses in from Thacker's file
    {

      baseCourse = inputString;
      //checks to see if the base course is already in the hash table
      checkSearch = myHashTable.search(tableSize, inputString, stringCompare, hashFunction, foundCourse, foundIndex);

      if(checkSearch == false)//item was not found
      {
          //insert into hash table
          graphItemBASE.course = inputString;
          graphItemBASE.courseIndex = indexCounter;
          //changes the initial indegreeArray value of that index from -1 to 0 so it can be incremented properly
          indegreeArray[graphItemBASE.courseIndex] = 0;
          myHashTable.insert(graphItemBASE, tableSize, hashFunction, inputString, indexCounter);

          //update indexCounter
          indexCounter++;

          //get info about this item in the hash table
          graphItemBASE.course = baseCourse;

          //insert into adjList at correct index
          adjList[graphItemBASE.courseIndex].push_front(graphItemBASE);
      }

      else//item was found so it is in its position in the adjList
      {
          graphItemBASE.course = inputString;
          graphItemBASE.courseIndex = foundIndex;
      }

      classFile>>inputString;//reads the next item after the base course

      while(inputString != "#")//continues if the item is not a pound because it means we're done with an input line, meaning the course is a prereq for the base course
      {
        prereqCourse = inputString;

        //checks to see if the prereqCourse is in the hashTable already
        checkSearch = myHashTable.search(tableSize, prereqCourse, stringCompare, hashFunction, foundCourse, foundIndex);
        if(checkSearch == false)//item was not found
        {
            //insert into hash table
            graphItemPREREQ.course = prereqCourse;
            graphItemPREREQ.courseIndex = indexCounter;
            //changes the initial indegreeArray value of that index from -1 to 0 so it can be incremented properly
            indegreeArray[graphItemPREREQ.courseIndex] = 0;
            myHashTable.insert(graphItemPREREQ, tableSize, hashFunction, inputString, indexCounter);

            //update indexCounter
            indexCounter++;

            //insert into adjList at correct index
            adjList[graphItemPREREQ.courseIndex].push_front(graphItemPREREQ);//pushes prereq course where it belongs in adjList

            //push_back the base course into the prereq course's index
            adjList[graphItemPREREQ.courseIndex].push_back(graphItemBASE);

            //base course got a prereq so update its indegree
            indegreeArray[graphItemBASE.courseIndex]++;
        }
        else//item was found in hash table
        {
            graphItemPREREQ.course = prereqCourse;
            graphItemPREREQ.courseIndex = foundIndex;
            //dont want to insert into hash table again, but still want to push_back the base course into the into the prereq's slot
            adjList[graphItemPREREQ.courseIndex].push_back(graphItemBASE);

            //base course got a prereq so update its indegree
            indegreeArray[graphItemBASE.courseIndex]++;
        }

        //read the next item from the file
        classFile>>inputString;

      }//end of !# while loop

       //found a #, move on to the next line and save tge first item in the line because it's a base course

    }//end of eof while loop

    //closes the courses input file
    classFile.close();

    //goes through the indegree array one time to create the initial zeroIndegreeLL
    for(int x=0; x<arraySize; x++)
    {
      if(indegreeArray[x] == 0)//if the course has no incoming edges, push it into the zeroIndegreeLL
      {
        zeroIndegreeLL.push_back(x);
      }
    }

    //TOPOLOGICAL SORT
    
    iter<int> zeroItr = zeroIndegreeLL.begin();
    iter<GRAPHITEM> innerItr;
    iter<GRAPHITEM> LLinnerItr;

    //continue until at the end of the zeroIndegreeLL
    while(zeroItr!=zeroIndegreeLL.end())
    {
        innerItr=adjList[*zeroItr].begin();//sets another iterator the beginning of the LL in the appropriate adjList slot
        LLinnerItr = innerItr;

        cout<<(*innerItr).course<<endl;
        innerItr++;//moves iter to the next node in that slot's LL

        //go through this while loop until you're at the end of the current slot's LL
        while(innerItr!=adjList[(*LLinnerItr).courseIndex].end())
        {
          graphItemTOPSORT.courseIndex = (*innerItr).courseIndex;
          indegreeArray[graphItemTOPSORT.courseIndex]--;//decrements the current node's course's indegree
          if(indegreeArray[graphItemTOPSORT.courseIndex] == 0)//checks to see if it became zero and pushes it into the zeroIndegreeLL if so
          {
            zeroIndegreeLL.push_back(graphItemTOPSORT.courseIndex);
          }

          innerItr++;
        }

        zeroIndegreeLL.pop_front();//removes the first node from the zeroIndegreeLL because it was visited and doesn't need to continue occupying space
        zeroItr++;
    }

  }//end of else (in regards to being able to open the file or not)

  cout<<endl;
  return 0;
}//end of main


//hash function from the book, modified to take a string and convert it to a character array
int hashFunction(int tableSize, string inputString)//hash wants a string so get the string part of the struct and pass it
{
    int length = inputString.length();
    char characterArray[length+1];//sets an array of characters with one more slot in the array than the length of the item because we need to manually insert the terminating null character
    int hashIndex;

    inputString.copy(characterArray, length);//turns the string passed in into an array of characters
    characterArray[length]='\0';//manully sets the terminating null character

    unsigned int *Lkey = (unsigned int *)characterArray;//unsigned means the value will always be positive
    int intLength = strlen(characterArray)/4;//divids the length of the character array by four so it can be processed four bytes at a time
    unsigned int sum = 0;

    for(int i=0; i<intLength; i++)//loops through each character in the four byte pieces and adds the values
    {
      sum += Lkey[i];
    }

    int extra = strlen(characterArray) - intLength*4;//processes four bytes at a time
    char temp[4];//creates a temporary array of characters to process only four bytes
    Lkey = (unsigned int *)temp;
    Lkey[0] = 0;

    for(int i=0; i<extra; i++)
    {
       temp[i] = characterArray[intLength*4+i];
    }

    sum += Lkey[0];

    hashIndex = sum % tableSize;//converts the result to be in the range 0 to M-1

    return hashIndex;
}

//compares two strings for equality and returns either true or false. Is called in the search method in the hash class
bool stringCompare(string currentString, string searchString)
{
    if(currentString == searchString)
      return true;
    else
      return false;
}

//created print function in main to avoid the templating issue with the list's print all function. This way structs can be printed
void printMain(list<GRAPHITEM> *adjList, int arraySize)
{
  iter<GRAPHITEM> structItr;
  int adjListCounter = 0;

  for(int x=0; x<arraySize; x++)
  {
    for(structItr=adjList[x].begin(); structItr!=adjList[x].end(); structItr++)
    {
        cout<<(*structItr).course<<" "<<(*structItr).courseIndex<<" ";
        adjListCounter++;
    }
    cout<<endl;
  }
}
