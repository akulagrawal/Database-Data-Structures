#include "b+tree.h"

int main(){
	BPlusTree* tree = NULL;
 	int key,choice;
 	int numIndexPointers, numDataPointers;
  	//node* searchResult;
  	int searchResultIndex;
  do {
    cout << "1. Create new tree" << endl;
    cout << "2. Add new key to the tree" << endl;
    cout << "3. Search a key in the tree" << endl;
    cout << "4. Postorder traversal of the tree" << endl;
    cout << "Any other choice to exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cout << "You entered " << choice << endl;

    switch(choice){
      case 1:
        if(tree != NULL){
          delete tree;
        }
        cout<<"Enter number of Index pointers: ";
        cin>>numIndexPointers;
        cout<<"Enter number of Data pointers: ";
        cin>>numDataPointers;
        if(numIndexPointers%2==0 || numDataPointers%2){
        	cout<<"Note:: Index pointers must be in odd number and Data pointers must be in even number"<<endl;
        	exit(0);
        }
        tree = new BPlusTree(numIndexPointers, numDataPointers);
        break;
      
      case 2:
        if(tree == NULL){
          cout << "Create a new tree first" << endl;
          break;
        }
        cout << "Enter key to add ";
        cin >> key;
        tree->insert(tree, key);
        break;
      case 3:
        if(tree == NULL){
          cout << "Create a new tree first" << endl;
          break;
        }
        cout << "Enter key to search: ";
        cin >> key;
        {
        	DataNode *searchResult = tree->search(tree->root, key, tree->depth);
	        if(searchResult == NULL){
	           cout << "Key " << key << " not found." << endl;
	        }
	        else{
	          cout << "Key " << key << " found" << endl;
	        }
	    }
        break;
      case 4:
        if(tree == NULL){
          cout << "Create a new tree first" << endl;
          break;
        }
        tree->LevelOrderTraversal(tree->root, tree->depth);
        break;

      default:
        return 0;
    }
  }
  while(true);
	return 0;
}