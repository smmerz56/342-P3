using namespace std;
#include <fstream>
#include <iostream>
#ifndef LIST_H
#define LIST_H

//----------------------------------------------------------------------------
// ADT List: a Template List class. A collection of ordered objects.
//
// Assumptions:
//    -- The user will only be using employee and nodedata objects.
//    -- The txt files being read in are formatted correctly.
//    -- All printing of the T info is up to the T class.
//    -- Making the memory for the data is up to the user.
//    -- If the user enters an empty list into merge or intersect, it is a 
//       user error, nothing happens.
//    -- In remove and retrieve, if the user targets a node that doesnt
//       exist, the address of the assigned pointer is returned. The lists
//       remain unchanged.
//    -- Im assuming you remember that I could of got three lines of code
//       down to 1 (head->data = new T(*leftcurrent->data)in both 
//       intersect and copy. I decided to not go back and change it out of 
//       excruciating fear that I would mess my code up.  
//----------------------------------------------------------------------------

template <typename T> class List
{

   friend ostream& operator<<(ostream& output, const List<T>& thelist)
   {
      List<T>::Node* current = thelist.head;
      while(current != NULL) {
         output << *current->data;
         current = current->next;
      }
      return output;
   }

   private:
   // the nodes which make up the list
   struct Node {
      T* data;
      Node* next;
   };
   Node* head;

   public:
   // constructors and destructor
   List();
   List(const List&);
   ~List();

   void buildList(ifstream& infile);
   bool insert(T* dataPtr);
   void copy(const List&);

   bool remove(const T&, T*&);
   bool retrieve(const T&, T*&);

   void merge(List&, List&);
   void intersect(const List&, const List&);

   bool isEmpty() const;
   void makeEmpty();

   List<T>& operator=(const List&);
   bool operator==(const List&) const;
   bool operator!=(const List&) const;
};
//----------------------------------------------------------------------------
// Constructor
// this sets the head node of the newly created list to null
template <typename T> 
List<T>::List()
{
   head = nullptr;
}

// --------------------------------------------------------------------------
// Copy constructor
// Copy constructor for IntSet class
template <typename T> 
List<T>::List(const List<T>& listToCopy)
{
   copy(listToCopy);
}
//----------------------------------------------------------------------------
// Destructor
// calls makeEmpty to delete everything in the list
template <typename T> 
List<T>::~List()
{
   makeEmpty();
}
//----------------------------------------------------------------------------
// intersect
// this takes two sorted lists and finds the items in common in both lists
// and then creates a new list with those items. Neither of the list past
// in change, unless one of them is the current object.

template <typename T> 
void List<T>::intersect(const List<T>& leftList, const List<T>& rightList)                                              
{

   if(this != &leftList && this != &rightList) {
      this->makeEmpty(); 
   }
   Node* leftCurrent = leftList.head;
   Node* rightCurrent = rightList.head;
   Node* intersectCurrent;
   Node* fakehead = nullptr;
   T* newData; 

   while(leftCurrent != nullptr && rightCurrent != nullptr) {
      if(*leftCurrent->data == *rightCurrent->data) {
         if(fakehead == nullptr) {
            fakehead = new Node();
            newData = new T; 
            fakehead->data = newData;
            *fakehead->data = *leftCurrent->data;
            intersectCurrent = fakehead;
         } else {
            intersectCurrent->next = new Node();
            intersectCurrent = intersectCurrent->next;
            newData = new T;
            intersectCurrent->data = newData;
            *intersectCurrent->data = *leftCurrent->data;
         }
         leftCurrent = leftCurrent->next;
         rightCurrent = rightCurrent->next;
      } else if(*leftCurrent->data < *rightCurrent->data) {
         leftCurrent = leftCurrent->next;
      } else if(*rightCurrent->data < *leftCurrent->data) {
         rightCurrent = rightCurrent->next;
      }
   }
   if(*this == rightList || *this == leftList) {
      this->makeEmpty();
   }
   head = fakehead;
   fakehead = nullptr;
}
//----------------------------------------------------------------------------
// merge
// takes 2 lists and merges them into one sorted list. No new memory is
// created. When the program finishes, both parameter lists are empty
// unless one is the current object.
// I made newRightHead because I originally had (const List<T>& leftside,..)
// and I wasnt able change leftside/rightside. I didnt want to rewrite my
// code after I removed the const's.
template <typename T> 
void List<T>::merge(List<T>& leftside, List<T>& rightside)                                          
{

   if(leftside.head != nullptr && rightside.head != nullptr) {

      if(this->head != leftside.head && this->head != rightside.head) {
         this->makeEmpty();
      }
      Node* current = leftside.head;   //current pointer on leftside
      Node* next;                          //2nd pointer on leftside
      Node* rightCurrent = rightside.head;//1st pointer on rightside
      Node* newRightHead = rightside.head;//2nd pointer on rightside

      leftside.head = nullptr;
      rightside.head = nullptr;

      if(*rightCurrent->data < *current->data) {
         head = newRightHead;
         newRightHead = newRightHead->next;

         while(newRightHead != nullptr && *newRightHead->data < 
                                                *current->data) {
            rightCurrent = rightCurrent->next;
            newRightHead = newRightHead->next;
         }
         rightCurrent->next = current;
         rightCurrent = newRightHead;
      } else {
         head = current;
      }
      next = current->next;
      while(rightCurrent != nullptr) {
         if(next == nullptr && newRightHead != nullptr) {
            current->next = newRightHead;
            rightCurrent = nullptr;
         } else {
            if(*rightCurrent->data < *next->data) {
               newRightHead = newRightHead->next;
               current->next = rightCurrent;
               rightCurrent->next = next;
               current = rightCurrent;
               rightCurrent = newRightHead;
            } else {
               current = next;
               next = current->next;
            }
         }
      }
      
   } else {
      if(this->head != leftside.head && this->head != rightside.head) {
         this->makeEmpty();
         if(leftside.head != nullptr) {
            this->head = leftside.head;
            leftside.head = nullptr;
         }
         if(rightside.head != nullptr) {
            this->head = rightside.head;
            rightside.head = nullptr;
         }
      }
   }
}
//----------------------------------------------------------------------------
// Copy
// used in the copy constructor and operator=. Copies past in list to
// current list
template <typename T> 
void List<T>::copy(const List<T>& listToCopy)
{
   Node* ogPtr = listToCopy.head;
   if(ogPtr == nullptr) {
      head = nullptr;
   } else {
      head = new Node();
      Node* current = head;
      T* copyData = new T;

      head->data = copyData; 
      *head->data = *ogPtr->data;

      ogPtr = ogPtr->next;
      while(ogPtr != nullptr) {
         current->next = new Node();
         current = current->next;
         copyData = new T;
         current->data = copyData;
         *current->data = *ogPtr->data;

         ogPtr = ogPtr->next;
      }
   }
}
//----------------------------------------------------------------------------
// operator==
// compares two lists
template <typename T> 
bool List<T>::operator==(const List<T>& rightSide)const
{
   Node* ogPtr = rightSide.head; // pointer for the past in list
   Node* current = head;         // pointer for the current list
   if(head == nullptr && rightSide.head != nullptr) {
      return false;
   }
   while(current != nullptr) {
      if(current == nullptr && ogPtr != nullptr) {
         return false;
      }
      if(ogPtr == nullptr && current != nullptr) {
         return false;
      }
      if(*ogPtr->data != *current->data) {
         return false;
      } else {
         ogPtr = ogPtr->next;
         current = current->next;
      }
   }
   return true;
}
//----------------------------------------------------------------------------
// operator!=
// compares two lists
template <typename T> 
bool List<T>::operator!=(const List& rightSide) const
{
   Node* ogPtr = rightSide.head; // pointer to the past in list
   Node* current = head;         // pointer to the current list

   if(head == nullptr && rightSide.head != nullptr) {
      return true;
   }
   while(current != nullptr) {
      if(current == nullptr && ogPtr != nullptr) {
         return true;
      }
      if(ogPtr == nullptr && current != nullptr) {
         return true;
      }
      if(*ogPtr->data != *current->data) {
         return true;
      } else {
         ogPtr = ogPtr->next;
         current = current->next;
      }
   }
   return false;
}
//----------------------------------------------------------------------------
// operator=
// this assigns one list to another
template <typename T> 
List<T>& List<T>::operator=(const List& rightSide)
{
   if(this != &rightSide) {
      this->makeEmpty();
      copy(rightSide);
   }
   return *this;
}
//---------------------------------------------------------------------------
// buildList
// continually insert new items into the list
template <typename T> 
void List<T>::buildList(ifstream& infile)
{

   T* ptr;
   bool successfulRead;
   bool success;

   for(;;) {
      ptr = new T;
      successfulRead = ptr->setData(infile);
      if(infile.eof()) {
         delete ptr;
         ptr = NULL;
         break;
      }
      if(successfulRead) {
         success = insert(ptr);
      } else {
         delete ptr;
         ptr = NULL;
      }
      if(!success) {
         break;
      }
   }
}

//---------------------------------------------------------------------------
// insert
// insert an item into list; operator< of the T class
// has the responsibility for the sorting criteria
template <typename T> 
bool List<T>::insert(T* dataPtr)
{
   Node* ptr = new Node;
   if(ptr == NULL) {
      return false;
   }
   ptr->data = dataPtr;

   if(isEmpty() || *ptr->data < *head->data) {
      ptr->next = head;
      head = ptr;
   } else {
      Node* current = head;

      while(current->next != nullptr) {
         if(*ptr->data > *current->next->data) {
            current = current->next;
         } else {
            break;
         }
      }
      ptr->next = current->next;
      current->next = ptr;
   }

   return true;
}

//---------------------------------------------------------------------------
// isEmpty
// check to see if List is empty as defined by a NULL head
template <typename T> 
bool List<T>::isEmpty() const
{
   return head == nullptr;
}

//---------------------------------------------------------------------------
// makeEmpty
// empty the list, deallocate all memory for all the nodes and each object
template <typename T> 
void List<T>::makeEmpty()
{

   Node* current = head;
   Node* next;

   while(current != nullptr) {
      next = current->next;
      delete current->data;
      current->data = nullptr;
      delete current;
      current = next;
   }
   head = nullptr;
   current = nullptr;
   next = nullptr;
}
//---------------------------------------------------------------------------
// remove
// Remove the first parameter from the list and set the 2nd parameter to
// the removed object if it is in the list.
template <typename T> 
bool List<T>::remove(const T& empToRemove, T*& removedEmpPointer)
{

   if(head != nullptr) {     // if list is empty, do nothing
      Node* previous = head; // pointer for helping navigate the list
      Node* current;         // pointer used to find empToRemove

      if(*head->data == empToRemove) {
         removedEmpPointer = head->data;
         head = previous->next;
         previous->data = nullptr;
         delete previous;

         previous = nullptr;
         current = nullptr;
         return true;
      }

      current = previous->next;
      while(current != nullptr) {
         if(*current->data == empToRemove) {
            removedEmpPointer = current->data;
            current->data = nullptr;
            previous->next = current->next;
            delete current;

            previous = nullptr;
            current = nullptr;
            return true;
         } else {
            previous = current;
            current = previous->next;
         }
      }
      previous = nullptr;
      current = nullptr;

      return false;
   }
   return false;
}
//---------------------------------------------------------------------------
// retrieve
// finds the target in the list and sets the second parameter to the target
template <typename T> 
bool List<T>::retrieve(const T& target, T*& empPointer)
{

   if(head != nullptr) {
      Node* previous = head;
      Node* current;

      if(*head->data == target) {
         empPointer = head->data;

         previous = nullptr;
         current = nullptr;
         return true;
      }

      current = previous->next;
      while(current != nullptr) {
         if(*current->data == target) {
            empPointer = current->data;

            previous = nullptr;
            current = nullptr;
            return true;
         } else {
            previous = current;
            current = previous->next;
         }
      }
      previous = nullptr;
      current = nullptr;

      return false;
   }
   return false;
}

#endif