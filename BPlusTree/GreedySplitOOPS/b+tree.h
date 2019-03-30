#pragma once
#include "node.h"

class BPlusTree{
  public:
    BPlusTree(int numIndexPointers,int numDataPointers);
    IndexNode* toIndexNode(void *p);
    DataNode* toDataNode(void *p);
    void insert(BPlusTree *tree, int key);
    void splitLeafChild(IndexNode *x,int i);
    void splitNonLeafChild(IndexNode *x,int i);
    void insertNonFull(void *node, int key, int depth);
    DataNode* search(void *node, int key, int depth);
    void LevelOrderTraversal(void *node, int depth);

    void* root;
    int numIndexPointers;
    int numDataPointers;
    int depth;
};