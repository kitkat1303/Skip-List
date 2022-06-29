/*
File name: SkipList.cpp
Author: Katarina McGaughy
Date: 11/3/2021
Description: The SkipList class initializes a list that has multiple levels and
has half the number of elements in each level above the previous. The purpose
is to allow for quicker traversal accross elements in the list. The elements in
the list consist of SkipListNodes which are initialized within the SkipListNode
struct nested within the SkipList class.
*/

#include "SkipList.h"
#include <random>
#include <iostream>
#include <cassert>

// public methods

/**
 * SkipList
 * cosntructor that initializes a SkipList with a max number of levels
 * that has a default value set to 1. It also initializes the heads_ and
 * tail_ array.
 * @param maxLevels_ : sets the maximum number of levels in the skip list
 * and if no data is given will set maxLevels_ to 1
 */
SkipList::SkipList(int maxLevels) : maxLevels_(maxLevels)
{
   // initialize the heads and tails array with number of levels and set to null
   // by calling the array () initializer
   heads_ = new SkipListNode *[maxLevels_]();
   tails_ = new SkipListNode *[maxLevels_]();
}

/**
 * ~SkipList
 * desctructor that clears the entire list.
 */
SkipList::~SkipList()
{
   clear();
}

/**
 * isEmpty
 * this function returns true if there are no elements within the SkipList.
 * @return : true if the list is empty
 */
bool SkipList::isEmpty() const
{
   return heads_[0] == nullptr;
}

/**
 * findNode
 * this function takes in an integer and level and returns a pointer
 * to the node if found
 * @param value: integer value to be searched for in the list
 * @param level: top level too be returned where found
 * @return: a pointer to the node where the value was found
 * and if not found, returns nullptr
 */
SkipList::SkipListNode *SkipList::findNode(int item, int &level) const
{

   SkipListNode *curr;
   if (heads_[level] != nullptr)
   {
      for (int i = maxLevels_ - 1; i >= 0; i--)
      {
         if (heads_[i] != nullptr)
         {
            level = i;
            curr = heads_[level];
            break;
         }
      }

      bool stillAtHead = true;

      // Look for the insertion point for each level:
      while (level >= 0)
      {
         if (stillAtHead)
         {
            curr = heads_[level];
         }

         SkipListNode *next = curr;

         // Keep advancing along the this level of the list until our item is 
         //larger or reach the end:
         while (item > next->data_)
         {
            stillAtHead = false;
            curr = next;
            next = next->next_;
            if (next == nullptr)
               break;
         }

         if (next != nullptr && item == next->data_)
            return next;

         // move down a level
         curr = curr->downLevel_;
         level--;
      }
   }
   return nullptr;
}

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
bool SkipList::insert(int item)
{
   // return false if item is already in the list
   if (contains(item))
   {
      return false;
   }

   SkipListNode *curr = nullptr;
   vector<SkipListNode *> insertionPoints(maxLevels_, nullptr);
   int level = 0;

   // If the list is not empty find the insertion points:
   // Find the top level
   if (heads_[level] != nullptr)
   {
      for (int i = maxLevels_ - 1; i >= 0; i--)
      {
         if (heads_[i] != nullptr)
         {
            level = i;
            curr = heads_[level];
            break;
         }
      }

      bool stillAtHead = true;

      // Look for the insertion point for each level:
      while (level >= 0)
      {
         if (stillAtHead)
         {
            curr = heads_[level];
         }

         SkipListNode *next = curr;

         // Keep advancing along the this level of the list until our item is 
         //larger or reach the end:
         while (item > next->data_)
         {
            stillAtHead = false;
            curr = next;
            next = next->next_;
            if (next == nullptr)
               break;
         }

         // store pointer is that level is not empty
         insertionPoints[level] = next;

         // move down a level
         curr = curr->downLevel_;
         level--;
      }
   }

   level = 0;
   SkipListNode *downNode = nullptr;

   // Start from bottom and keep inserting as long as the coin toss is in 
   //our favor:
   while (level < maxLevels_)
   {
      // Insert the node horizontally:
      SkipListNode *newNode = new SkipListNode(item);
      addBefore(newNode, insertionPoints[level], level);

      // Cleanup vertical pointers:
      newNode->downLevel_ = downNode;

      // Set the lower node's up pointer to current node:
      if (downNode != nullptr)
      {
         downNode->upLevel_ = newNode;
      }
      downNode = newNode;
      level++;

      if (!alsoHigher())
         break;
   }

   return true;
}

/**
 * erase
 * this function takes in an integer and searches the list for the integer.
 * If found, the node with the integer will be deleted from all levels of
 * the list. It checks to see if the list is empty via calling the
 * isEmpty function and calls the contains method to see if it is in the list.
 * @param item: integer to be inserted in the list
 * @PRE: none
 * @POST: list with the element containing the integer deleted if it was
 * found in the list
 * @return: true if the item was deleted
 */
bool SkipList::erase(int item)
{
   if (isEmpty())
   {
      return false;
   }

   // Get top node in the column and the level:
   int level = 0;
   SkipListNode *topNode = findNode(item, level);

   // If not found, nothing to erase.
   if (topNode == nullptr)
   {
      return false;
   }

   // Loop down the column and delete each node in the column:
   while (topNode != nullptr)
   {
      // if at tail
      if (topNode->next_ == nullptr)
      {
         tails_[level] = topNode->prev_;
      }
      else
      {
         topNode->next_->prev_ = topNode->prev_;
      }

      // if at head
      if (topNode->prev_ == nullptr)
      {
         heads_[level] = topNode->next_;
      }
      else
      {
         topNode->prev_->next_ = topNode->next_;
      }
      SkipListNode *tmp = topNode;
      topNode = topNode->downLevel_;
      delete tmp;
      level--;
   }
   return true;
}

/**
 * contains
 * this function takes in an integer and returns true if the integer is found
 * within the list. It calls the isEmpty function to see if the list is
 * empty.
 * @param: integer to search for
 * @return: true if the integer is found within the list
 */
bool SkipList::contains(int item) const
{
   // dummy level value
   int level = 0;
   if (findNode(item, level) != nullptr)
   {
      return true;
   }
   else
   {
      return false;
   }
}

// private methods

/** The constructor initializes a node, sets all pointers to nullptr and sets
   it's data
   @param data: an integer that is then set to the nodes data member variable
   PRE: none
   POST: node is initialized
   */
SkipList::SkipListNode::SkipListNode(int data)
{

   data_ = data;
   next_ = nullptr;
   prev_ = nullptr;
   upLevel_ = nullptr;
   downLevel_ = nullptr;
}

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
void SkipList::addBefore(SkipListNode *newNode, SkipListNode *nextNode,
                         int level)
{
   // If the list is empty:
   if (heads_[level] == nullptr)
   {
      heads_[level] = newNode;
      tails_[level] = newNode;
   }
   // if nextNode is null we are at the tail:
   else if (nextNode == nullptr)
   {
      newNode->prev_ = tails_[level];
      // assert(tails_[level] != nullptr);
      tails_[level]->next_ = newNode;
      tails_[level] = newNode;
   }
   // Insert at head ?
   else if (nextNode->prev_ == nullptr)
   {
      newNode->next_ = nextNode;
      nextNode->prev_ = newNode;
      heads_[level] = newNode;
   }
   // Insert in middle, easy squeezy
   else
   {
      newNode->prev_ = nextNode->prev_;
      newNode->next_ = nextNode;
      nextNode->prev_->next_ = newNode; // this is throwing exception
      nextNode->prev_ = newNode;
   }
}

/**
 * alsoHigher
 * this function returns true 50% of the time and is used in the insert
 * method to decide wether the new element inserted will be moved to the
 * next level of the SkipList.
 * PRE: none
 * POST: returns true 50% of the time
 * @return: true 50% of the time based on the random generator
 */
bool SkipList::alsoHigher() const
{
   static default_random_engine generator;  // generator
   static bernoulli_distribution coin(0.5); // distribution
   return coin(generator);
}

/**
 * clear
 * this function clears the entire list and is called in the desctructor
 */
void SkipList::clear()
{
   SkipListNode *curr;
   SkipListNode *temp;
   for (int i = 0; i < maxLevels_; i++)
   {
      curr = heads_[i];

      while (curr != nullptr)
      {
         SkipListNode *tmp = curr;
         curr = curr->next_;
         delete tmp;
      }

      tails_[i] = nullptr;
      heads_[i] = nullptr;
   }
}

/**
 * friend
 * this function is a friend of SkipList and overrides the output operator
 * @param os: ostream object
 * @param list: is the SkipList
 * @return: ostream object
 */
ostream &operator<<(ostream &os, const SkipList &list)
{

   for (int i = list.maxLevels_ - 1; i >= 0; i--)
   {
      if (list.heads_[i] == nullptr)
      {
         os << "Level: " << i << " -- empty" << endl;
      }
      else
      {
         SkipList::SkipListNode *curr = list.heads_[i];
         os << "Level: " << i << " -- " << curr->data_;

         curr = curr->next_;

         while (curr != nullptr)
         {
            os << ", " << curr->data_;
            curr = curr->next_;
         }
         os << endl;
      }
   }
   return os;
}
