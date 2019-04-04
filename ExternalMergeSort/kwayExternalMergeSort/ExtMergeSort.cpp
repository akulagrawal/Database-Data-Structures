#include "common.h"
#include "ExtMergeSort.h"
#include <bits/stdc++.h>

#define f first
#define s second

//creates initial runs of 1 page size
void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory){
	
	int frame = -1;
	// load each page to main memory frame and sort
	for(int i = 0; i < inputFile.totalPages; i++){
		frame = memory.loadPage(inputFile, i);
		this->sortFrame(memory, frame);
		memory.writeFrame(inputFile, frame, i);
		memory.freeFrame(frame);
	}
	
	runSize = 1;
	totalPass = 1;
	totalRuns = inputFile.totalPages;
	cout << "First Pass Performed" << endl;
	inputFile.writeDiskFile(); //print file to cout
}

//sorts each frame
void ExtMergeSort :: sortFrame(MainMemory &memory, int f){
	sort(memory.data[f].arr.begin(), memory.data[f].arr.begin()+memory.getValidEntries(f));
}

//Performs merging of 2 runs
void ExtMergeSort :: merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int mid, int rightEnd){

	int finalRunSize = rightEnd - leftStart + 1;
	DiskFile tempFile(finalRunSize);

	bool flag = false;
	int currPage = 0;
	int l = leftStart;
	int r = mid + 1;

	int leftFrame = memory.loadPage(inputFile, l);
	int rightFrame = memory.loadPage(inputFile, r);
	int resFrame = memory.getEmptyFrame();	//frame to store result
	if(leftFrame == -1 || rightFrame == -1 || resFrame == -1){
		cout << "Can't proceed further due to unavailability of memory or invalid Page access" << endl;
		exit(1); 
	}

	int leftIndex = 0;
	int rightIndex = 0;
	int resIndex = 0;

	while(l <= mid && r <= rightEnd){
		if(leftIndex < memory.getValidEntries(leftFrame) && rightIndex < memory.getValidEntries(rightFrame)){	
			int x = memory.getVal(leftFrame, leftIndex);
			int y = memory.getVal(rightFrame, rightIndex); 
			if( x < y){
				memory.setVal(resFrame, resIndex, x);
				flag = true;
				leftIndex++;
			}
			else{
				flag = true;
				memory.setVal(resFrame, resIndex, y);
				rightIndex++;
			}
			resIndex++;
			if(resIndex == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, resFrame, currPage);
				flag = false;
				memory.freeFrame(resFrame);
				resFrame = memory.getEmptyFrame();
				currPage++; 
				resIndex = 0;
			}
		}

		if(leftIndex == memory.getValidEntries(leftFrame)){
			memory.freeFrame(leftFrame);
			l++;
			if(l <= mid){
				leftFrame = memory.loadPage(inputFile, l);
				leftIndex = 0;
			}	
		}

		if(rightIndex == memory.getValidEntries(rightFrame)){
			memory.freeFrame(rightFrame);
			r++;
			if(r <= rightEnd){
				rightFrame = memory.loadPage(inputFile, r);
				rightIndex = 0;
			}
		}
	}
	while(l <= mid){
		int x = memory.getVal(leftFrame, leftIndex);
		memory.setVal(resFrame, resIndex, x);
		flag = true;
		leftIndex++;
		resIndex++;
		if(resIndex == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
		if(leftIndex == memory.getValidEntries(leftFrame)){
			memory.freeFrame(leftFrame);
			l++;
			if(l <= mid){
				leftFrame = memory.loadPage(inputFile, l);
				leftIndex = 0;
			}	

		}
	}
	while(r <= rightEnd){
		int x = memory.getVal(rightFrame, rightIndex);
		memory.setVal(resFrame, resIndex, x);
		flag = true;
		rightIndex++;
		resIndex++;
		if(resIndex == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
		if(rightIndex == memory.getValidEntries(rightFrame)){
			r++;
			memory.freeFrame(rightFrame);
			if(r <= rightEnd){
				rightFrame = memory.loadPage(inputFile, r);
				rightIndex = 0;
			}	

		}
	}
	if(flag)
		memory.writeFrame(tempFile, resFrame, currPage);
	memory.freeFrame(resFrame);
	memory.freeFrame(leftFrame);
	memory.freeFrame(rightFrame);
	inputFile.DiskFileCopy(tempFile, leftStart, rightEnd);

}

//Performs 2 way merge sort on inputFile using memory
void ExtMergeSort :: twoWaySort(DiskFile &inputFile, MainMemory &memory){
	
	if(memory.totalFrames < 3)
	{
		cout << "Error: Two way merge sort requires atleast 3 frames" << endl; 
		exit(1);
	}
	
	this->firstPass(inputFile, memory);

	int leftStart;
	
	for(this->runSize = 1; this->runSize < inputFile.totalPages; this->runSize *= 2){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += 2*this->runSize){
			int mid = leftStart + this->runSize-1;
			int rightEnd = min(leftStart + 2*this->runSize-1, inputFile.totalPages-1);
			cout << "calling merge for < " << leftStart <<", " << mid << ", " << rightEnd << " >" << endl;
			//this->merge(inputFile, memory, leftStart, mid, rightEnd);
			vector<int> v;
			v.push_back(leftStart);
			v.push_back(mid);
			v.push_back(mid+1);
			v.push_back(rightEnd); 
			this->mergek(inputFile, memory, v);
			//inputFile.writeDiskFile();
		}
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}

void ExtMergeSort :: KWaySort(DiskFile &inputFile, MainMemory &memory, int k){
	
	if(memory.totalFrames < k+1)
	{
		cout << "Error: "<<k<<" way merge sort requires atleast "<<k+1<<" frames but only "<<memory.totalFrames<<" are free"<<endl; 
		exit(1);
	}
	
	this->firstPass(inputFile, memory);

	int leftStart;
	
	for(this->runSize = 1; this->runSize < inputFile.totalPages; this->runSize *= k){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += k*this->runSize){
			int rightEnd = min(leftStart + k*this->runSize-1, inputFile.totalPages-1);
			cout << "calling merge for < " << leftStart <<", " << rightEnd << " >" << endl;
			vector<int> v;
			for(int mid = leftStart;mid <= rightEnd;mid += this->runSize){
				v.push_back(mid);
				v.push_back(min(mid+this->runSize-1, rightEnd));
			}
			//for(int i=0;i<v.size();i++)
			//	cout<<v[i]<<" ";
			//cout<<endl;
			this->mergek(inputFile, memory, v);
			//inputFile.writeDiskFile();
		}
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}

struct compare
{
    bool operator()(const pair<int, int>& l, const pair<int, int>& r)
    {
        return l.f > r.f;
    }
};


void ExtMergeSort :: mergek(DiskFile &inputFile, MainMemory &memory, vector<int> v){

	int k = v.size()/2;
	int finalRunSize = v[v.size()-1] - v[0] + 1;
	int leftStart = v[0];
	int rightEnd = v[v.size()-1];
	DiskFile tempFile(finalRunSize);

	bool flag = false;
	int currPage = 0;
	vector<int> currIndex(k, 0);
	int resIndex = 0;

	priority_queue<pair<int, int>, vector<pair<int, int> >, compare > pq;
	vector<int> loadFrame;
	int i, tempFrame, resFrame;
	for(i=0;i<k;i++)
	{
		tempFrame = memory.loadPage(inputFile, v[2*i]);
		if(tempFrame == -1){
			cout << "Can't proceed further due to unavailability of memory or invalid Page access" << endl;
			exit(1); 
		}
		loadFrame.push_back(tempFrame);
		int x = memory.getVal(tempFrame, 0);
		//cout<<x<<":"<<i<<" ";
		pq.push(make_pair(x, i));
	}
	//cout<<endl;
	tempFrame = memory.getEmptyFrame();	//frame to store result
	if(resFrame == -1){
		cout << "Can't proceed further due to unavailability of memory or invalid Page access" << endl;
		exit(1); 
	}
	resFrame = tempFrame;
	while(!pq.empty()){
		int x = pq.top().f;
		int y = pq.top().s;
		//cout<<x<<":"<<y<<endl;
		memory.setVal(resFrame, resIndex, x);
		resIndex++;
		currIndex[y]++;
		pq.pop();
		flag = true;
		if(currIndex[y] == memory.getValidEntries(loadFrame[y])){
			memory.freeFrame(loadFrame[y]);
			v[2*y]++;
			if(v[2*y] <= v[2*y+1]){
				currIndex[y] = 0;
				loadFrame[y] = memory.loadPage(inputFile, v[2*y]);
				int z = memory.getVal(loadFrame[y], 0);
				pq.push(make_pair(z, y));
			}
		}
		else{
			int z = memory.getVal(loadFrame[y], currIndex[y]);
			pq.push(make_pair(z, y));
		}
		
		if(resIndex == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
	}
	if(flag)
		memory.writeFrame(tempFile, resFrame, currPage);
	memory.freeFrame(resFrame);
	inputFile.DiskFileCopy(tempFile, leftStart, rightEnd);

}