#include "b+tree.h"

/*Following notations and part of procedures are taken from algorithms of Btree in CLRS*/  

BPlusTree::BPlusTree(int numIndexPointers,int numDataPointers){
	root = new DataNode(numDataPointers);
	this->numIndexPointers = numIndexPointers;
	this->numDataPointers = numDataPointers;
	depth = 0;
}

IndexNode* BPlusTree::toIndexNode(void *p){
	return static_cast<IndexNode*>(p);
}

DataNode* BPlusTree::toDataNode(void *p){
	return static_cast<DataNode*>(p);
}

DataNode* BPlusTree::search(void *node, int key, int depth){
	
	if(depth==0){
		DataNode *x = toDataNode(node);	
		for(int i = 0; i < x->getNumKeys(); i++)
			if(x->getKeyAtIndex(i) == key)
				return x;
		return NULL;	
	}
	else{
		IndexNode *x = toIndexNode(node);
		int i = 0;
		while(i < x->getNumKeys() && key >= x->getKeyAtIndex(i))
			i = i + 1;
		return search(x->getIndexPointerAt(i), key, depth-1);
	}
}


void BPlusTree::insert(BPlusTree *tree, int key){
	if(depth==0){
		DataNode *root = toDataNode(tree->root);
		if(root->getNumKeys() == numDataPointers){
			IndexNode *new_root = new IndexNode(numIndexPointers);
			tree->root = new_root;
			new_root->setIndexPointerAt(root, 0);
			depth = depth + 1;
			splitLeafChild(new_root, 0);
			insertNonFull(new_root, key, depth);
		}
		else{
			insertNonFull(root, key, depth);
		}
	}
	else{
		IndexNode *root = toIndexNode(tree->root);
		if(root->getNumKeys() == numIndexPointers-1){
			IndexNode *new_root = new IndexNode(numIndexPointers);
			tree->root = new_root;
			new_root->setIndexPointerAt(root, 0);
			depth = depth + 1;
			splitNonLeafChild(new_root, 0);
			insertNonFull(new_root, key, depth);
		}
		else{
			insertNonFull(root, key, depth);
		}
	}
}

void BPlusTree::splitLeafChild(IndexNode *x,int i){
	DataNode *z = new DataNode(numDataPointers);
	DataNode *y = toDataNode(x->getIndexPointerAt(i));
	int mid = numDataPointers/2;
	
	z->setNumKeys(mid);
	for(int j = 0; j < mid; j++){
		z->setKeyAtIndex(y->getKeyAtIndex(j + mid), j);
		z->setDataPointerAt(y->getDataPointerAt(j + mid), j);
	}
	y->setNumKeys(mid);

	for(int j = x->getNumKeys(); j > i; j--){
		x->setIndexPointerAt(x->getIndexPointerAt(j), j + 1);
	}
	x->setIndexPointerAt(z, i+1);

	for(int j = x->getNumKeys()-1; j >= i; j--){
		x->setKeyAtIndex(x->getKeyAtIndex(j), j + 1);
	}
	x->setKeyAtIndex(z->getKeyAtIndex(0), i);
	x->setNumKeys(x->getNumKeys()+1);

	z->setDataNodeLeft(y);
	z->setDataNodeRight(y->getDataNodeRight());
	y->setDataNodeRight(z);
}

void BPlusTree::splitNonLeafChild(IndexNode *x,int i){
	IndexNode *z = new IndexNode(numIndexPointers);
	IndexNode *y = toIndexNode(x->getIndexPointerAt(i));
	int mid = numIndexPointers/2;

	int keyToUP = y->getKeyAtIndex(mid);
	z->setNumKeys(mid-1);
	int j;
	for(j = 0; j < mid-1; j++){
		z->setKeyAtIndex(y->getKeyAtIndex(j + mid + 1), j);
		z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
	}
	z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
	y->setNumKeys(mid);

	for(j = x->getNumKeys(); j > i; j--){
		x->setIndexPointerAt(x->getIndexPointerAt(j), j + 1);
	}
	x->setIndexPointerAt(z, i+1);

	for(j = x->getNumKeys()-1; j >= i; j--){
		x->setKeyAtIndex(x->getKeyAtIndex(j), j + 1);
	}
	x->setKeyAtIndex(keyToUP, i);
	x->setNumKeys(x->getNumKeys()+1);
}

void BPlusTree::insertNonFull(void *node, int key, int depth){
	if(depth==0){
		DataNode *x = toDataNode(node);
		int i = x->getNumKeys()-1;
		while(i >= 0 && key < x->getKeyAtIndex(i)){
			x->setKeyAtIndex(x->getKeyAtIndex(i), i+1);
			i = i - 1;
		}
		x->setKeyAtIndex(key, i+1);
		x->setNumKeys(x->getNumKeys()+1);
	}
	else{
		IndexNode *x = toIndexNode(node);
		int i = x->getNumKeys()-1;
		while(i >= 0 && key < x->getKeyAtIndex(i))
			i = i - 1;
		i = i + 1;

		if(depth==1){
			if(toDataNode(x->getIndexPointerAt(i))->getNumKeys() == numDataPointers){
				splitLeafChild(x, i);
				if(key > x->getKeyAtIndex(i))
					i = i + 1;
			} 
		}
		else{
			if(toIndexNode(x->getIndexPointerAt(i))->getNumKeys() == numIndexPointers-1){
				splitNonLeafChild(x, i);
				if(key > x->getKeyAtIndex(i))
					i = i + 1;
			}
		}
		insertNonFull(x->getIndexPointerAt(i), key, depth-1);
	}
}

void BPlusTree::LevelOrderTraversal(void *root, int depth){
	queue<pair<int,void *> > q;
	queue<int> level;
	int nodeID = 1;
	pair<int, void*> curNode;
	int curLevel=0,prevLevel=-1;
	depth = depth + 1;
	q.push({nodeID, root});
	level.push(0);
	cout<<"\n*** NodeID :: Values ***\n"<<endl;
	while(!q.empty()){
		curNode = q.front(); q.pop();
		if(prevLevel!=level.front()){
			depth = depth-1;
			prevLevel = curLevel++;
		}
		level.pop();
		cout<<curNode.first<< " :: ";
		if(depth==0){
			DataNode *node = toDataNode(curNode.second);
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<endl;
		}
		else{
			IndexNode *node = toIndexNode(curNode.second);
			for(int i=0; i <= node->getNumKeys(); i++){
				q.push({++nodeID, node->getIndexPointerAt(i)});
				level.push(curLevel);
			}
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<endl;
		}
	}
	cout<<endl<<endl;
}