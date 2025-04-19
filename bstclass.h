#ifndef bstclass_h
#define bstclass_h
#include "jobclass.h"
#include <iostream>
#include <fstream>
using namespace std;

class TreeNode:public job {
public:
    job data;
    TreeNode* leftPtr;
    TreeNode* rightPtr;
    
    TreeNode(const job& j) : data(j), leftPtr(nullptr), rightPtr(nullptr) {}
};

class BST {
private:
    TreeNode* root;
    int size;

    void inorder(TreeNode* t) {
        if (t != nullptr) {
            inorder(t->leftPtr);
            t->data.display();
            inorder(t->rightPtr);
        }
    }

    void search(TreeNode* node, int id, const job& edit) {
        if (node != nullptr) {
            if (node->data.compare("equal", id)) {
                node->data = edit;
            } else if (node->data.compare("less", id)) {
                search(node->leftPtr, id, edit);
            } else {
                search(node->rightPtr, id, edit);
            }
        }
    }

    bool search_check(TreeNode* node, int id) {
        if (!node) return false;
        if (node->data.compare("equal", id)) return true;
        if (node->data.compare("less", id))
            return search_check(node->leftPtr, id);
        else
            return search_check(node->rightPtr, id);
    }
    void get_job_from_bst(TreeNode* node, int id, job& getdata)
    {
        if (node != nullptr) {
            if (node->data.compare("equal", id)) {
                getdata.setjob(node->data);
            } else if (node->data.compare("less", id)) {
                get_job_from_bst(node->leftPtr, id, getdata);
            } else {
                get_job_from_bst(node->rightPtr, id, getdata);
            }
        }
    }
    void clone_to_file(TreeNode* node, ofstream& out,string stats) {
        if (node != nullptr) {
            clone_to_file(node->leftPtr, out,stats);
            int tag_num;
            string jobtype, company, requir, loca, stas;
            double max, min;
            node->data.getdata(tag_num, jobtype, company, loca, max, min, stas, requir);
            if(stats!="NULL")
                stas=stats;
            out << "\"" << tag_num << "\" \"" << jobtype << "\" \"" << company << "\" \"" << requir
                << "\" \"" << loca << "\" \"" << max << "\" \"" << min << "\" \"" << stas << "\"" << endl;
            clone_to_file(node->rightPtr, out,stats);
        }
    }

public:
    BST() : root(nullptr), size(0) {}

    void insertNode(const job &g, const string& sor) {
        TreeNode* t = root;
        TreeNode* new_node = new TreeNode(g);
        if (!root) {
            root = new_node;
        } else {
            bool inserted = false;
            while (!inserted) {
                int tag;
                string jobtype, company, requir, loca, stas;
                double max1, min1, max2, min2;
    
                t->data.getdata(tag, jobtype, company, loca, max1, min1, stas, requir);
                new_node->data.getdata(tag, jobtype, company, loca, max2, min2, stas, requir);
    
                bool goLeft = false;
    
                if (sor == "max") {
                    goLeft = (max2 <= max1);
                } else if (sor == "min") {
                    goLeft = (min2 <= min1);
                } else {
                    goLeft = (new_node->data <= t->data);
                }
    
                if (goLeft) {
                    if (!t->leftPtr) {
                        t->leftPtr = new_node;
                        inserted = true;
                    } else {
                        t = t->leftPtr;
                    }
                } else {
                    if (!t->rightPtr) {
                        t->rightPtr = new_node;
                        inserted = true;
                    } else {
                        t = t->rightPtr;
                    }
    
            }
        }
        size++;
    }
    }

    void displayInOrder() {
        inorder(root);
    }

    void editJob(int id, const job& updatedJob) {
        search(root, id, updatedJob);
    }

    bool exists(int id) {
        return search_check(root, id);
    }

    void saveToFile(const string& filename,string stats) {
        ofstream out(filename, ios::app);
        if (out.is_open()) {
            clone_to_file(root, out,stats);
            out.close();
        }
    }
    void read_bst(int id,job &getinfo)
    {
        get_job_from_bst(root,id,getinfo);
    }
    int getSize() const {
        return size;
    }
};
#endif