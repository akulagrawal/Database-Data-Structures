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
	int globalDepth,bucketSize,overflow;
	vector<Bucket*> dir;
public:
	void createHashTable(int globalDepth,int bucketSize);
	void insertElement(int val);
	void insertElementNew(int val);
	bool searchElement(int val);
	bool deleteElement(int val);
	int getBucketIndex(int val);
	void doubleHashTable();
	void splitBucket(int bucketIndex);
	int twoPower(int p);
	void printHashTable();
};

void ExtendibleHashing::createHashTable(int globalDepth,int bucketSize){
	int buckets = twoPower(globalDepth);
	this->globalDepth = globalDepth;
	this->dir.resize(buckets);
	this->bucketSize=bucketSize;
	for(int i=0;i<buckets;i++){
		this->dir[i] = new Bucket();
		this->dir[i]->localDepth = globalDepth;
	}
	this->overflow = 0;
}

int ExtendibleHashing::getBucketIndex(int val){
	int lsbOfOnes = twoPower(this->globalDepth)-1;
	return val & lsbOfOnes;
}

void ExtendibleHashing::insertElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	if(this->dir[bucketIndex]->bucket.size() == this->bucketSize){
		if(this->globalDepth == this->dir[bucketIndex]->localDepth){
			this->doubleHashTable();
			this->splitBucket(bucketIndex);
		}
		else{
			this->splitBucket(bucketIndex);
		}
		this->insertElement(val);
	}
	else{
		this->dir[bucketIndex]->bucket.push_back(val);
	}
}

void ExtendibleHashing::insertElementNew(int val){
	int bucketIndex = this->getBucketIndex(val);
	if(this->dir[bucketIndex]->bucket.size() >= this->bucketSize){
		//Bucket full
		if(this->globalDepth == this->dir[bucketIndex]->localDepth){
			//Case 2. New method
			if(this->dir[bucketIndex]->bucket.size() > this->bucketSize){
				//Case 2.1. Add in overflow
				this->dir[bucketIndex]->bucket.push_back(val);
			}
			else{
				if(this->overflow == this->twoPower(this->globalDepth) - 1){
					//Case 2.2.1. Double once
					this->doubleHashTable();
					for(int i=0;i<twoPower(this->globalDepth - 1);i++){
						int oldSize = this->dir[i]->bucket.size();
						this->splitBucket(i);
						int newSize = this->dir[i]->bucket.size();
						if((oldSize > this->bucketSize) && (newSize <= this->bucketSize))
							this->overflow--;
					}
					bucketIndex = this->getBucketIndex(val);
					if(this->dir[bucketIndex]->bucket.size() == this->bucketSize)
						this->overflow++;
					this->dir[bucketIndex]->bucket.push_back(val);
				}
				else{
					//Case 2.2.2. Add in overflow
					this->dir[bucketIndex]->bucket.push_back(val);
					this->overflow++;
				}
			}
		}
		else{
			//Case 1. Old method
			int oldSize = this->dir[bucketIndex]->bucket.size();
			this->splitBucket(bucketIndex);
			int newSize = this->dir[bucketIndex]->bucket.size();
			if((oldSize > this->bucketSize) && (newSize <= this->bucketSize))
				this->overflow--;
			this->insertElementNew(val);
		}
	}
	else{
		// Normal Case. Bucket not full
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
	int size = this->dir.size();
	this->globalDepth++;
	this->dir.resize(2*size);
	for(int i=size;i<2*size;i++)
		this->dir[i]=this->dir[i-size];
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
	for(auto itr=firstBucket.begin();itr!=firstBucket.end();){
		if(*itr & localDepthPower){
			secondBucket.push_back(*itr);
			itr = firstBucket.erase(itr);
		}
		else itr++;
	}
	/*pointing to proper buckets after spliting*/
	localDepthPower = 2*localDepthPower;
	for(int i=secondBucketIndex+localDepthPower; i < this->dir.size(); i=i+localDepthPower)
		this->dir[i] = this->dir[secondBucketIndex];

	if(this->dir[secondBucketIndex]->bucket.size() > this->bucketSize)
			this->overflow++;
}

int ExtendibleHashing::twoPower(int p){
	return 1<<p;
}

void ExtendibleHashing::printHashTable(){
	cout<<"\n ***** Hash Table ***** \n"<<endl;
	cout<<"globalDepth :: " << this->globalDepth<<endl;
	unordered_set<Bucket*> uset;
	cout<<"Bucket ::| Elements |"<<endl;
	for(int i=0; i<this->dir.size(); i++){
		if(uset.find(this->dir[i])!=uset.end())
			continue;
		cout<<"Bucket "<<i<<" :: | ";
		uset.insert(this->dir[i]);
		for(auto l:this->dir[i]->bucket)
			cout<<l<<" ";
		cout<<"|" <<endl;
	}
	cout<<endl;
}

int main(){

	int choice;
	ExtendibleHashing *d=NULL;
	do{
		cout<<"1. Create new ExtendibleHashTable"<<endl;
		cout<<"2. Add an element to the HashTable"<<endl;
		cout<<"3. Search an element to the HashTable"<<endl;
		cout<<"4. Delete an element from the HashTable"<<endl;
		cout<<"5. Print HashTable"<<endl;

		cout<<"Any other choice to exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		cout<<"You entered "<<choice<<endl;
		switch(choice){
			case 1:
				if(d!=NULL)
					delete d;
				d= new ExtendibleHashing();
				int globalDepth;
				cout<<"Enter global depth: ";
				cin>>globalDepth;
				int bucketSize;
				cout<<"Enter number of entries in bucket: ";
				cin>>bucketSize;
				d->createHashTable(globalDepth,bucketSize);
				break;
			case 2:
				int add;
				cout<<"Enter element to add: ";
				cin>>add;
				d->insertElementNew(add);
				break;
			case 3:
				int search;
				cout<<"Enter element to search: ";
				cin>>search;
				if(d->searchElement(search))
					cout<<"Element is found"<<endl;
				else
					cout<<"Element is not found"<<endl;
				break;
			case 4:
				int del;
				cout<<"Enter element to delete: ";
				cin>>del;
				if(d->deleteElement(del))
					cout<<"Element is deleted"<<endl;
				else
					cout<<"Element is not found"<<endl;
				break;
			case 5:
				d->printHashTable();
				break;
			default:
				return 0;
		}
	}while(true);
}
