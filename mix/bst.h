#ifndef bst_h
#define bst_h
#include "jobclass.h"
struct TreeNode
{
   struct TreeNode *leftPtr;  // pointer to left subtree
   job data;                  // node value
   struct TreeNode *rightPtr; // pointer to right subtree
}; // end structure treeNode

typedef struct TreeNode *TreeNodePtr; // synonym for TreeNode*

typedef struct
{
   int size;
   TreeNodePtr root=NULL;
} BST;

void insertNode(BST *, job ,string);
void inorder(TreeNodePtr t);
void insertNode(BST *b, job g,string sor)
{
   TreeNodePtr t = b->root, new_node;
   int inserted = 0;
   new_node = new TreeNode;
   if (new_node)
   {
      new_node->leftPtr = NULL;
      new_node->rightPtr = NULL;
      new_node->data = g;
   }
   /*First Node*/
   if (b->root == NULL)
      b->root = new_node;
   else
   {
      while (!inserted)
      {
         if (new_node->data <= t->data)
         {
            /* move/insert to the left*/
            if (t->leftPtr == NULL)
            {
               // cout<<"1";
               t->leftPtr = new_node;
               inserted = 1;
            }
            else
            {
               t = t->leftPtr;
            }
         }
         else
         {
            /* move/ insert to the right*/
            if (t->rightPtr == NULL)
            {
               t->rightPtr = new_node;
               inserted = 1;
            }
            else
            {
               t = t->rightPtr;
            }
         }
      } // end while

   } // end else;
   b->size++;
}
void inorder(TreeNodePtr t)
{
   if (t != NULL)
   {
      inorder(t->leftPtr);
      t->data.display();
      inorder(t->rightPtr);
   }
}
// end function

#endif