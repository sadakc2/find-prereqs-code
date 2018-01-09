# find-prereqs-code
Given the courses needed for graduation and their prerequisites, create a list of courses in an order that will allow the student to progress successfully. This code uses an array of linked lists of structs as well as a hash table.
The input should be character strings of length less than or equal to 10 characters.   Each string will represent a course.  The first course on each line will be the base course.  The rest of the courses on the same line will be the prerequisite courses that must be completed before the base course can be taken.  There will be a # sign at the end of each line to make input easier. A course may have no prerequisites.
There should be no more than 500 courses to consider.

This code prints out a list of courses such that the prerequisites for every course appear before that course.  It implements the solution in a very efficient way (for the computer).
This program utilizes an efficient/fast topological sort algorithm using linked lists. Instead of using the C++ List STL, I use my own reverse-engineered doubly linked list class.
To find where a string is in the array, it uses a hash table and hash function because linear search is inefficient and binary search requires keeping the array sorted.
