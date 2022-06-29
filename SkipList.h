/*
File name: SkipList.h
Author: Katarina McGaughy
Date: 11/3/2021
Description: The SkipList class initializes a list that has multiple levels and
has half the number of elements in each level above the previous. The purpose
is to allow for quicker traversal accross elements in the list. The elements in
the list consist of SkipListNodes which are initialized within the SkipListNode
struct nested within the SkipList class.
*/

#include <iostream>
#ifndef SKIPLIST
#define SKIPLIST
using namespace std;

class SkipList
{
private:
   struct SkipListNode
   {

      /** The constructor initializes a node, sets all pointers to nullptr and 
       * sets it's data
       * @param data: an integer that is then set to the nodes data member variable
       * PRE: none
       * POST: node is initialized
      */
      explicit SkipListNode(int data);

      // data for SkipListNode
      int data_;

      // link to next at same level
      SkipListNode *next_;

      // link to previous at same level
      SkipListNode *prev_;

      // link up one level
      SkipListNode *upLevel_;

      // link down one level
      SkipListNode *downLevel_;
   };

   // maximum # of levels of SkipList, levels are 0 to maxLevels-1
   int maxLevels_;

   // array of maxLevels_ SkipListNode pointers as head pointers.
   SkipListNode **heads_;

   // array of maxLevels_ SkipListNode pointers as tail pointers.
   SkipListNode **tails_;

   /**
    * addBefore
    * this function takes in a pointer to a newNode, a pointer to nextNode,
    * and a level and places the newNode before the nextNode at that level.
    * @param newNode: pointer to a newNode,
    * @param nextNode: pointer to the nextNode,
    * @param level: level where the nodes exist
    * PRE: must have a pointer to newNode and nextNode along with the level
    * POST: the newNode is connected to the nextNode
    */
   void addBefore(SkipListNode *newNode, SkipListNode *nextNode, int level);

   /**
    * alsoHigher
    * this function returns true 50% of the time and is used in the insert
    * method to decide wether the new element inserted will be moved to the
    * next level of the SkipList.
    * PRE: none
    * POST: returns true 50% of the time
    * @return: true 50% of the time based on the random generator
    */
   bool alsoHigher() const; // this should be static

   /**
    * clear
    * this function clears the entire list and is called in the desctructor
    */
   void clear();

   /**
    * findNode
    * this function takes in an integer and level and returns a pointer
    * to the node if found
    * @param value: integer value to be searched for in the list
    * @param level: top level too be returned where found
    * @return: a pointer to the node where the value was found
    * and if not found, returns nullptr
    */
   SkipList::SkipListNode *findNode(int value, int &level) const;

public:
   /**
    * SkipList
    * cosntructor that initializes a SkipList with a max number of levels
    * that has a default value set to 1. It also initializes the heads_ and
    * tail_ array.
    * @param maxLevels_ : sets the maximum number of levels in the skip list
    * and if no data is given will set maxLevels_ to 1
    */
   explicit SkipList(int maxLevels = 1);

   /**
    * ~SkipList
    * desctructor that clears the entire list.
    */
   virtual ~SkipList();

   /**
    * isEmpty
    * this function returns true if there are no elements within the SkipList.
    * @return : true if the list is empty
    */
   bool isEmpty() const;

   /**
    * insert
    * this function takes in an integer item and inserts it in the list
    * if it does not already exist in the list. It calls the contains function
    * to check.
    * @param item: integer that will be stored in a SkipListNode
    * @PRE: none
    * @POST: new node with the data item inserted in the list at various levels
    * depending on the alsoHigher method
    * @return: true if the item was inserted in the list
    */
   bool insert(int item);

   /**
    * erase
    * this function takes in an integer and searches the list for the integer.
    * If found, the node with the integer will be deleted from all levels of
    * the list. It checks to see if the list is empty via calling the
    * isEmpty function and calls the contains method to see if it is in the list
    * @param item: integer to be inserted in the list
    * @PRE: none
    * @POST: list with the element containing the integer deleted if it was
    * found in the list
    * @return: true if the item was deleted
    */
   bool erase(int item);

   /**
    * contains
    * this function takes in an integer and returns true if the integer is found
    * within the list. It calls the isEmpty function to see if the list is
    * empty.
    * @param: integer to search for
    * @return: true if the integer is found within the list
    */
   bool contains(int item) const;

   /**
    * friend
    * this function is a friend of SkipList and overrides the output operator
    * @param os: ostream object
    * @param list: is the SkipList
    * @return: ostream object
    */
   friend ostream &operator<<(ostream &os, const SkipList &list);
};
#endif
