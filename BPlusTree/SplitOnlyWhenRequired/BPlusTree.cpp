#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>
#include "BPlusTree.h"
using namespace std;

class DataNode* toDataNode(void *x) {
    return (DataNode*)x;
}

class IndexNode* toIndexNode(void *x) {
    return (IndexNode*)x;
}

class Node* toNode(void *x) {
    return (Node*)x;
}


/* *******************************************************************************
 * Member declarations of a BPlusTree
 * *******************************************************************************/
BPlusTree::BPlusTree(int numDataPointers, int numIndexPointers) {
    this->numDataPointers = numDataPointers;
    this->numIndexPointers = numIndexPointers;
    this->root = NULL;
}

// Print BPlusTree in BFS manner
void BPlusTree::printBTree() {
    class Node* temp = toNode(this->root);
    queue<Node*> s;
    s.push(temp);

    while(!s.empty()) {
        temp = s.front();
        s.pop();
        
        if (temp->is_DataNode())
            toDataNode(temp)->printDataNode();
        else
            toIndexNode(temp)->printIndexNode();
            
        if (!temp->is_Leaf()) {
            // Go through all its child
            // Push all its child into stack
            for(int i = 0; i < toIndexNode(temp)->get_child_size(); i++) {
                s.push(toNode(toIndexNode(temp)->get_child_at_index(i)));
            }
        }
    }
}

// By default it will do pro_active_splitting
bool BPlusTree::insert_element(int key, bool pro_active_split) {
    if (this->root == NULL) {
        this->root = new DataNode(this->numDataPointers);
    }

    if (pro_active_split) {
        
        // Search till the leaf and split everything comes in between that is full
        // Special case : Splitting root, if it is full
        if (toNode(this->root)->is_Leaf()) {
            if (toDataNode(this->root)->is_full()) {
                toDataNode(this->root)->insert_element(key);
                toDataNode(this->root)->splitNode();
                this->root = toDataNode(this->root)->get_parent();
            }
            else {
                toNode(this->root)->insert_element(key);
            }
            return NO_ERROR;
        }
        
        if (toNode(this->root)->is_full()) {

            toIndexNode(this->root)->splitNode();
            this->root = toNode(this->root)->get_parent();
        }

        class Node* temp = toNode(this->root);
        // Search till the leaf and split everything comes in between that is full
        while (!temp->is_Leaf()) {
        
            if (temp->is_full()) {
                toIndexNode(temp)->splitNode();
                temp = temp->get_parent();
            }

            // Search for the appropriate child
            int position = toIndexNode(temp)->lower_bound_on_child(key);

            // Get next node to search on
            temp = toNode(toIndexNode(temp)->get_child_at_index(position));
        }

        // We got the leaf
        if (temp->is_Leaf()) {

            // If leaf is full
            if (temp->is_full()) {
                temp->insert_element(key);
                toDataNode(temp)->splitNode();
            }
            else {
                temp->insert_element(key);
            }
        }
    }
    else {
        // Find the leaf
        Node *temp = toNode(this->root);
        while (!temp->is_Leaf()) {
            temp = toNode(toIndexNode(temp)->get_child_at_index(toIndexNode(temp)->lower_bound_on_child(key)));
        }
        // We got the leaf -> "temp"
        if (temp->is_full()) {
            temp->insert_element(key);
            
            // Iteratively go to parent if parent also overflows
            while (temp->is_overflow()) {
                if (temp->is_DataNode())
                    toDataNode(temp)->splitNode();
                else
                    toIndexNode(temp)->splitNode();
                if (temp == toNode(this->root))     this->root = temp->get_parent();
                temp = temp->get_parent();
            }
        }
        else
            temp->insert_element(key);
    }
    return NO_ERROR;
}

// Search element at the leaf of BPlusTree
// Returns DataNode if found, else returns NULL
Node* BPlusTree::search_element(int key) {
    if (this->root == NULL)     return NULL;
    Node *temp = toNode(this->root);
    while(!temp->is_Leaf()) {
        // Search element in current node
        if(temp->search_element(key))
            return temp;
        temp = toNode(toIndexNode(temp)->get_child_at_index(toIndexNode(temp)->lower_bound_on_child(key)));
    }
    if (toDataNode(temp)->search_element(key))
        return temp;
    else
        return NULL;
}

/* *******************************************************************************
 * Member declarations of a Node
 * *******************************************************************************/

int Node::get_size() {
    return this->key.size();
}

int Node::get_depth() {
    return this->depth;
}

bool Node::is_full() {
    return this->get_size() == this->max_size;
}

bool Node::is_overflow() {
    return this->get_size() > this->max_size;
}

bool Node::is_Leaf() {
    return this->get_depth() == 0;
}

bool Node::is_DataNode() {
    return this->is_Leaf();
}

bool Node::is_IndexNode() {
    return !this->is_Leaf();
}

void Node::increase_depth() {
    this->depth++;
}

int Node::get_key_at_index(int index) {
    return this->key[index];
}

class Node* Node::get_parent() {
    return this->parent;
}

// Returns position where key is inserted
int Node::insert_element (int key) {
    // Insert key into list of keys in sorted order
    // Get the required location
    vector<int>::iterator it = lower_bound(this->key.begin(), this->key.end(), key);
    int distance = it - this->key.begin();
    // Insert into the vector
    this->key.insert(it, key);

    return distance;
}
// Search element in this node
// Returns true, if found
bool Node::search_element(int key) {
    return binary_search(this->key.begin(), this->key.end(), key);
}

/* *******************************************************************************
 * Member declarations of a DataNode
 * *******************************************************************************/
DataNode::DataNode(int numDataPointers, int depth, int id) {
    this->max_size = numDataPointers;
    this->min_size = (numDataPointers + 1)/2;
    this->depth = depth;
    this->parent = NULL;
    this->id = id;
}

int DataNode::get_numDataPointers() {
    return this->max_size;
}

class DataNode* DataNode::get_new_DataNode(int numDataPointers) {
    return new DataNode(numDataPointers);
}

int DataNode::get_separator(int a, int b) {
    return (a+b)/2;
}

// Split the current node and send the middle element to the parent node
void DataNode::splitNode() {
    // Get the parent node
    class Node* parent = this->parent;

    /*********** Creating a right node **************/

    // Create a node for right node
    class DataNode *new_node = this->get_new_DataNode(this->get_numDataPointers());
    // Get keys for the right node
    new_node->key.insert(new_node->key.end(), this->key.begin()+ceil(this->key.size()/2.0), this->key.end());
    // Set parent of the node
    new_node->parent = parent;
    // Set depth of the new node
    new_node->depth = this->depth;
    // Set ID of node
    new_node->id = this->id + 1;
    
    /*********** Creating a left node **************/

    // key needed to send to parent
    int key = this->get_separator(this->key[ceil(this->key.size()/2.0) - 1], this->key[ceil(this->key.size()/2.0)]);

    // Keep this node as a left node, so delete right_keys
    this->key.erase(this->key.begin()+ceil(this->key.size()/2.0), this->key.end());

    /*********** Send the middle key to the parent **************/

    // If parent does not exists (Splitting the root case)
    if (parent == NULL) {

        // Create a new node for root
        parent = this->get_new_IndexNode(this->get_numDataPointers());
        // Set left child of parent[0] = this
        toIndexNode(parent)->set_child_at_index(this);
        // set id of parent
        parent->id = this->id + 2;
        parent->depth = this->depth + 1;
    }
    
    int position = parent->insert_element(key);

    // Correctly inserted
    if (position != -1) {
        // Left child is already set at its required child position
        // Insert right child at parent[position+1]
        toIndexNode(parent)->set_child_at_index(new_node, position + 1);
    }
    else {
        cout << "Error splitting node ...\n";
        exit(-1);
    }

    // Set the parent of both the child
    this->parent = parent;
    new_node->parent = parent;
}

// Print Node ID, Node keys and ID of children
void DataNode::printDataNode() {
    cout << "ID : " << this->id << " Depth : " << this->get_depth() << "\tElements : ";
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->key[i] << " ";
    }
    cout << endl;
}

/* *******************************************************************************
 * Member declarations of a IndexNode
 * *******************************************************************************/
IndexNode::IndexNode(int numIndexPointers, int depth, int id) {
    this->max_size = numIndexPointers;
    this->min_size = (numIndexPointers + 1)/2;
    this->depth = depth;
    this->parent = NULL;
    this->id = id;
}

int IndexNode::get_child_size() {
    return this->child.size();
}

int IndexNode::get_numIndexPointers() {
    return this->max_size;
}

int IndexNode::lower_bound_on_child (int key) {
    vector<int>::iterator it = lower_bound(this->key.begin(), this->key.end(), key);
    return it - this->key.begin();
}

void* IndexNode::get_child_at_index(int index) {
    return this->child[index];
}

// Returns true if the child is DataNode
bool IndexNode::isChildData() {
    // Check the child[0]
    return toNode(this->child[0])->is_DataNode();
}

// Print Node ID, Node keys and ID of children
void IndexNode::printIndexNode() {
    cout << "ID : " << this->id << " Depth : " << this->get_depth() << "\tElements : ";
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->key[i] << " ";
    }
    cout << endl;
    cout << "Child ID : ";
    // If child is dataNode
    for(int i = 0; i < this->child.size(); i++) {
        if (this->isChildData()) {
            DataNode* child = toDataNode(this->child[i]);
            cout << child->id << " ";
        }
        else {
            IndexNode* child = toIndexNode(this->child[i]);
            cout << child->id << " ";
        }
    }
    cout << endl;
}

class IndexNode* Node::get_new_IndexNode(int numIndexPointers) {
    return new IndexNode(numIndexPointers);
}

// Split the current node and send the middle element to the parent node
void IndexNode::splitNode() {
    // Get the parent node
    class Node* parent = this->parent;

    /*********** Creating a right node **************/

    // Create a node for right node
    class IndexNode *new_node = this->get_new_IndexNode(this->get_numIndexPointers());
    // Get keys for the right node
    new_node->key.insert(new_node->key.end(), this->key.begin()+ceil(this->key.size()/2.0) + 1, this->key.end());
    // Get child for right node
    new_node->child.insert(new_node->child.end(), this->child.begin()+ceil(this->child.size()/2.0), this->child.end());
    // Set parent of the node
    new_node->parent = parent;
    // Set depth of the new node
    new_node->depth = this->depth;
    // Set ID of node
    new_node->id = this->id + 1;
    
    /*********** Creating a left node **************/

    // key needed to send to parent
    int key = this->key[ceil(this->key.size()/2.0)];

    // Keep this node as a left node, so delete right_keys and child from the current node
    this->key.erase(this->key.begin()+ceil(this->key.size()/2.0), this->key.end());
    this->child.erase(this->child.begin()+ceil(this->child.size()/2.0), this->child.end());

    /*********** Send the middle key to the parent **************/

    // If parent does not exists (Splitting the root case)
    if (parent == NULL) {

        // Create a new node for root
        parent = this->get_new_IndexNode(this->get_numIndexPointers());
        // Set left child of parent[0] = this
        toIndexNode(parent)->child.push_back(this);
        // set id of parent
        parent->id = this->id + 2;
        parent->depth = this->depth + 1;
    }
    
    int position = parent->insert_element(key);

    // Correctly inserted
    if (position != -1) {
        // Left child is already set at its required child position
        // Insert right child at parent[position+1]
        toIndexNode(parent)->child.insert(toIndexNode(parent)->child.begin() + position + 1, new_node);
    }
    else {
        cout << "Error splitting node ...\n";
        exit(-1);
    }

    // Set the parent of both the child
    this->parent = parent;
    new_node->parent = parent;
}

void IndexNode::set_child_at_index(void *child, int position) {
    if (position == -1) {
        // Insert at the end
        this->child.push_back(child);
    }
    else {
        this->child.insert(this->child.begin() + position, child);
    }
}

/* *******************************************************************************
 * Member declarations ends
 * *******************************************************************************/

/*
 *  To Do :
 *          Handle depth
 *          Handle ID correctly
 *          Handle left right pointer of DataNode
 * */

/* Main for testing purpose */
// int main(int argc, char const *argv[])
// {
//     class BPlusTree root;
//     root.insert_element(10, false);
//     root.insert_element(20, false);
//     root.insert_element(30, false);
//     root.insert_element(40, false);
//     root.insert_element(50, false);
//     root.printBTree();
//     cout << endl;
//     root.insert_element(60, false);
//     root.insert_element(70, false);
//     root.insert_element(80, false);
//     root.insert_element(90, false);
//     root.insert_element(100, false);
//     root.printBTree();

//     // Search
//     // if (root.search_element(-23) != NULL)
//     //     cout << "FOUND\n";
//     return 0;
// }