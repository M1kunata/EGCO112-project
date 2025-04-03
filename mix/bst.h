#ifndef bst_h
#define bst_h
#include "jobclass.h"
struct TreeNode {                                           
    struct TreeNode *leftPtr; // pointer to left subtree
    job* data; // node value                               
    struct TreeNode *rightPtr; // pointer to right subtree
}; // end structure treeNode 
        
typedef struct TreeNode * TreeNodePtr; // synonym for TreeNode*
                    
typedef struct {
	int size;
	TreeNodePtr root;
}BST;

void insertNode( BST *,job * );
void insertNode( BST *b, job* t ){
    TreeNodePtr t =b->root,new_node;
    int inserted=0;
   new_node =(TreeNodePtr) malloc(sizeof(struct TreeNode));
   if(new_node){
       new_node->leftPtr=NULL;
       new_node->rightPtr=NULL;
       new_node->data=t;
  }
       /*First Node*/
       if(!b->root)	b->root=new_node;
       else {
while(!inserted){
  if(new_node->data >=){
    /* move/insert to the left*/
     if(t->leftPtr==NULL)
     {
        t->leftPtr=new_node;
        inserted=1;
     }
        else{
        t=t->leftPtr;
     }
     }
  else{
         /* move/ insert to the right*/
     if(t->rightPtr==NULL){
     t->rightPtr=new_node;
     inserted=1;}
     else{
        t=t->rightPtr;
     }
   }
   }//end while
  
 }//end else;
 b->size++;
 }
 
//end function



#endif