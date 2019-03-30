#include <vector>

#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define DEFAULT_NUM_DATA 3
#define DEFAULT_NUM_INDEX 2

#define ERROR false
#define NO_ERROR true

class DataNode* toDataNode(void *);
class IndexNode* toIndexNode(void *);
class Node* toNode(void *);

class Node
{
public:
    int id;
    int max_size;
    int min_size;
    int depth;
    class Node* parent;
    std::vector<int> key;

    Node() {}
    int get_size();
    int get_depth();
    bool is_full();
    bool is_DataNode();
    bool is_IndexNode();
    bool is_overflow();
    bool is_Leaf();
    void increase_depth();
    class Node* get_parent();
    int get_key_at_index(int);
    class Node* get_new_node(int numDataPointers = DEFAULT_NUM_DATA);
    class IndexNode* get_new_IndexNode(int numIndexPointers = DEFAULT_NUM_INDEX);
    int insert_element(int);
    bool search_element(int);
};

class BPlusTree
{
private:
    int numDataPointers;
    int numIndexPointers;
    int depth;
    void* root;
    
public:
    BPlusTree(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX);
    bool insert_element(int key, bool pro_active_split = false);
    void printBTree();
    Node* search_element(int);
    bool isDataNode();
};

class IndexNode : public Node
{
private:
    std::vector<void*> child;

public:
    IndexNode(int numIndexPointers = DEFAULT_NUM_INDEX, int depth = 0, int id = 0);
    int get_child_size();
    int lower_bound_on_child (int);
    void* get_child_at_index(int);
    void set_child_at_index(void*, int position = -1);
    void splitNode();
    int get_numIndexPointers();
    void printIndexNode();
    bool isChildData();
};

class DataNode : public Node
{
private:
    // Key "value" pair
    std::vector<void*> value;
    DataNode* left;
    DataNode* right;

public:
    DataNode(int numDataPointers = DEFAULT_NUM_DATA, int depth = 0, int id = 0);
    class DataNode* get_new_DataNode(int numDataPointers = DEFAULT_NUM_DATA);
    int get_separator(int, int);
    void splitNode();
    int get_numDataPointers();
    void printDataNode();
};


#endif // end of BPLUSTREE_H