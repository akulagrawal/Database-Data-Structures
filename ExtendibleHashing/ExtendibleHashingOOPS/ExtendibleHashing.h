#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<unordered_set>

using namespace std;

class Bucket{
public:
	int localDepth;
	list<int> bucket;
};
class ExtendibleHashing{
	int globalDepth,bucketSize;
	vector<Bucket*> dir;
public:
	void createHashTable(int globalDepth,int bucketSize);
	void insertElement(int val);
	bool searchElement(int val);
	bool deleteElement(int val);
	int getBucketIndex(int val);
	void doubleHashTable();
	void splitBucket(int bucketIndex);
	int twoPower(int p);
	void printHashTable();
};