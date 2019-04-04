#include "Page.h"

//fills page with contents from vector v
void Page :: fillPage(vector<int> &v){
	
	/*if(v.size() < DISK_PAGE_SIZE){
		cout << "vector should contain atleast as many entries as a page" << endl;
		exit(1);
	}*/
	int i = 0;
	bool flag = false;
	while(i < v.size() && i < DISK_PAGE_SIZE){
		this->arr[i] = v[i];
		if(v[i] == -1){
			flag = true;
			this->validEntries = i;
			i++;
			break;
		}
		i++;
	}
	while(i < DISK_PAGE_SIZE){
		this->arr[i] = -1;
		i++;
	}
	if(!flag)
		this->validEntries = DISK_PAGE_SIZE;
}

//writes page to cout
void Page :: writePage(){
	//cout<<"valid: "<<this->validEntries<<"; ";

	for(int i = 0; i < DISK_PAGE_SIZE; i++){
		if(this->arr[i] == -1)
			break;
		cout << this->arr[i] << " ";
	}
	cout << endl;
}