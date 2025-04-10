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
void search(TreeNodePtr, int id,job);
void clone_to_file(TreeNodePtr);
bool search_check(TreeNodePtr,int id);
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
void search(TreeNodePtr node,int id ,job edit) {
   if(node!=NULL)
   {
   if (node->data.compare("equal",id)) 
   {
      node->data=edit;
   }
   if (node->data.compare("less",id))
      search(node->leftPtr, id,edit);
   else
      search(node->rightPtr, id,edit);  
   }
}
   // end function
bool search_check(TreeNodePtr check,int id)
{
      if (!check) return false;
      if (check->data.compare("equal",id)) return true;
      if (check->data.compare("less",id))
          return search_check(check->leftPtr, id);
      else
          return search_check(check->rightPtr, id);
}
void clone_to_file(TreeNodePtr clone)
{
   if(clone!=NULL)
   {
      ofstream rewrite("job_data.txt",ios::app);
      clone_to_file(clone->leftPtr);
      int tag_num;
      string jobtype,company,requir,loca,stas;
      double max,min;
      clone->data.getdata(tag_num,jobtype,company,loca,max,min,stas,requir);
      rewrite << "\"" << tag_num << "\" \"" << jobtype << "\" \"" << company << "\" \"" << requir << "\" \"" << loca << "\" \"" << max << "\" \"" << min << "\" \"" << stas << "\"" << endl;
      clone_to_file(clone->rightPtr);
      rewrite.close();
   }
}