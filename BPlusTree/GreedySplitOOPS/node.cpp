#include "node.h"


int Node::getNumKeys(){
  return numKeys;
}

void Node::setNumKeys(int val){
	numKeys = val;
}

int Node::getKeyAtIndex(int i){
  return keys[i];
}

void Node::setKeyAtIndex(int key, int i){
	keys[i] = key;
}

Node::Node(int totalKeys){
	numKeys = 0;
	keys = new int[totalKeys];
}

IndexNode::IndexNode(int numIndexPointers)
	: Node(numIndexPointers-1)
{
	indexPointers = new void*[numIndexPointers];
}

void* IndexNode::getIndexPointerAt(int i){
	return indexPointers[i];
}
void IndexNode::setIndexPointerAt(void* p, int i){
	indexPointers[i] = p;
}

DataNode::DataNode(int numDataPointers)
	: Node(numDataPointers)
{
	dataPointers = new void*[numDataPointers];
	left = NULL;
	right = NULL;
}

void* DataNode::getDataPointerAt(int i){
	return dataPointers[i];
}
void DataNode::setDataPointerAt(void *p, int i){
	dataPointers[i] = p;
}
DataNode* DataNode::getDataNodeLeft(){
	return left;
}
void DataNode::setDataNodeLeft(DataNode *p){
	left = p;
}
DataNode* DataNode::getDataNodeRight(){
	return right;
}
void DataNode::setDataNodeRight(DataNode *p){
	right = p;
}