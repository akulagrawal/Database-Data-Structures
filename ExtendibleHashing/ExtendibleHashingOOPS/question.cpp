#include "ExtendibleHashing.h"


void ExtendibleHashing::createHashTable(int globalDepth,int bucketSize){
	int buckets = twoPower(globalDepth);
	this->globalDepth = globalDepth;
	this->dir.resize(buckets);
	this->bucketSize=bucketSize;
	for(int i=0;i<buckets;i++){
		this->dir[i] = new Bucket();
		this->dir[i]->localDepth = globalDepth;
	}
}

int ExtendibleHashing::getBucketIndex(int val){
	int lsbOfOnes = twoPower(this->globalDepth)-1;
	return val & lsbOfOnes;  
}

void ExtendibleHashing::insertElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	if(this->dir[bucketIndex]->bucket.size() == this->bucketSize){
		if(/* TODO */){
			/* TODO */
		}
		else{
			/* TODO */
		}
		this->insertElement(val);
	}
	else{
		this->dir[bucketIndex]->bucket.push_back(val);
	}
}

bool ExtendibleHashing::searchElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	for(auto itr = this->dir[bucketIndex]->bucket.begin(); itr!=this->dir[bucketIndex]->bucket.end(); itr++)
		if(*itr==val)
			return true;
	return false;
}

bool ExtendibleHashing::deleteElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	for(auto itr = this->dir[bucketIndex]->bucket.begin(); itr!=this->dir[bucketIndex]->bucket.end(); itr++){
		if(*itr==val){
			this->dir[bucketIndex]->bucket.erase(itr);
			return true;
		}
	}
	return false;
}
void ExtendibleHashing::doubleHashTable(){
	/* TODO */
}

void ExtendibleHashing::splitBucket(int bucketIndex){
	int localDepthPower = twoPower(this->dir[bucketIndex]->localDepth);
	int firstBucketIndex = bucketIndex & (localDepthPower-1);
	int secondBucketIndex = firstBucketIndex + localDepthPower;
	/*creating new bucket and updating local depths*/
	this->dir[secondBucketIndex] = new  Bucket();
	this->dir[secondBucketIndex]->localDepth = ++this->dir[firstBucketIndex]->localDepth;
	
	list<int> &firstBucket = this->dir[firstBucketIndex]->bucket;
	list<int> &secondBucket = this->dir[secondBucketIndex]->bucket;
	/*distributing values to two buckets*/
	
	/* TODO */

	/*pointing to proper buckets after splitting*/

	/* TODO */
}

int ExtendibleHashing::twoPower(int p){
	return 1<<p;
}

void ExtendibleHashing::printHashTable(){
	cout<<"\n ***** Hash Table ***** \n"<<endl;
	cout<<"globalDepth :: " << this->globalDepth<<endl;
	unordered_set<Bucket*> uset;
	cout<<"bucketIndex --> localDepth ::| Elements |"<<endl;
	for(int i=0; i<this->dir.size(); i++){
		if(uset.find(this->dir[i])!=uset.end())
			continue;
		cout<< i << " --> "<<this->dir[i]->localDepth<<" :: | ";
		uset.insert(this->dir[i]);
		for(auto l:this->dir[i]->bucket)
			cout<<l<<" ";
		cout<<"|" <<endl;
	}
	cout<<endl;
}
