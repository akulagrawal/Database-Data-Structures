#pragma once
#include<bits/stdc++.h>

using namespace std; 

class Node{
	public:
		Node(int totalKeys);

    	int getNumKeys();
    	void setNumKeys(int val);

	    int getKeyAtIndex(int i);
    	void setKeyAtIndex(int key, int i);

	private:
		int numKeys;
		int *keys;
};

class IndexNode: public Node{
	public:
		IndexNode(int numIndexPointers);
		void *getIndexPointerAt(int i);
		void setIndexPointerAt(void *p,int i);
	private:
		void **indexPointers;
};

class DataNode: public Node{
	public:
		DataNode(int numDataPointers);
		void *getDataPointerAt(int i);
		void setDataPointerAt(void *p, int i);
		DataNode* getDataNodeLeft();
		void setDataNodeLeft(DataNode *p);
		DataNode* getDataNodeRight();
		void setDataNodeRight(DataNode *p);
	private:
		void **dataPointers;
		DataNode *left, *right;
};

