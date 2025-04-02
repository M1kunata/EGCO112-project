#ifndef bst_h
#define bst_h
#include "jobclass.h"
class tree
{
    private:
        tree* right;
        job* sort;
        tree* left; 
    public:
        tree();
        void leftside(tree* a);
        void rightside(tree *a);
};
tree::tree()
{
    left=NULL;
    right=NULL;
}
void tree::leftside(tree *a)
{
    left=a;
}
void tree::rightside(tree *a)
{
    right=a;
}
class bst:public tree
{
    private:
        int size=0;
        tree* root=NULL;
    public:
       // bst(tree* root=NULL);
        void insert(bst *g,job *s);
};
void bst::insert(bst *g,job *s)
{
    tree* t=g->root;
    tree* newtree;
    int inserted=0;
    newtree=new tree();
    if(!g->root) g->root=newtree;
    t->leftside(newtree);
}





#endif