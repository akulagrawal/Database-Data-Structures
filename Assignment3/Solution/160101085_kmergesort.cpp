#include <bits/stdc++.h>

#define f first
#define s second

using namespace std;

int DISK_PAGE_SIZE;
int MEM_FRAME_SIZE;


class Frame{
public:
	vector<int> arr;
	int validEntries;
	//initializes an empty page with invalid entries i.e. -1
	Frame()
	{
		arr.resize(MEM_FRAME_SIZE, -1);
		/*for(int i  = 0; i < MEM_FRAME_SIZE; i++)
			arr[i] = -1;*/
		validEntries = 0;
	}

	//fill Frame with given vector
	void fillFrame(vector<int> &v);
	void printFrame();
};


//fills frame with data from vector v
void Frame :: fillFrame(vector<int> &v){
	int i = 0;
	bool flag = false;
	while(i < v.size() && i < MEM_FRAME_SIZE){
		this->arr[i] = v[i];
		if(v[i] == -1){
			flag = true;
			this->validEntries = i;
			i++;
			break;
		}
		i++;
	}
	while(i < MEM_FRAME_SIZE){
		this->arr[i] = -1;
		i++;
	}
	if(!flag)
		this->validEntries = MEM_FRAME_SIZE;
}

//Prints all valid entries of a frame
void Frame :: printFrame()
{
	for(int i = 0; i < this->validEntries; i++)
	{
		cout << this->arr[i] << endl;
	}
}


class Page{
public:
	vector<int> arr;
	int validEntries;
	//initializes an empty page with invalid entries i.e. -1
	Page(){
		arr.resize(DISK_PAGE_SIZE, -1);
		validEntries = 0;
	}

	vector<int> writePage();
	void fillPage(vector<int> &v);
};


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
vector<int> Page :: writePage(){
	//cout<<"valid: "<<this->validEntries<<"; ";
	vector<int> v;

	for(int i = 0; i < DISK_PAGE_SIZE; i++){
		if(this->arr[i] == -1)
			break;
		v.push_back(this->arr[i]);
		cout << this->arr[i] << " ";
	}
	cout << endl;
	return v;
}


class DiskFile{

public:
	vector<Page> data;
	int totalPages;
	int size; //in terms of number of entries
	vector<vector<vector<int> > > store;

	DiskFile(){
		totalPages = 0;
		size = 0;
	}

	DiskFile(int s){
		size = 0;
		totalPages = s;
		data.resize(totalPages);
	}


	void readDiskFile(int n);
	vector<vector<int> > writeDiskFile();
	void DiskFileCopy( DiskFile &f, int startPage, int endPage);
};


class MainMemory{

public:
	vector<Frame> data;
	int totalFrames;
	vector<bool> valid;

	MainMemory(int s){
		totalFrames = s;
		data.resize(s);
		valid.resize(s);
		for(int i = 0; i < s; i++)
			valid[i] = false;
		cout << "Mem created" << endl;
	}

	int loadPage(DiskFile &f, int n);
	int getEmptyFrame();
	int getValidEntries(int f);
	int getVal(int f, int i);
	void setVal(int f, int i, int val);
	void writeFrame(DiskFile &inputFile, int f, int p);
	void freeFrame(int f);
};


//loads nth page of disk file f to an empty frame if available
int MainMemory :: loadPage(DiskFile &f, int n){

	if(n >= f.totalPages)
		return -1; // invalid page

	for(int i = 0; i < this->totalFrames; i++){
		if(!this->valid[i]){
			this->data[i].fillFrame(f.data[n].arr);
			this->valid[i] = true;
			return i;
		}
	}
	return -1; //no empty frame is available
}

//returns an empty frame if available
int MainMemory :: getEmptyFrame(){

	vector<int> v(MEM_FRAME_SIZE, -1);

	for(int i = 0; i < this->totalFrames; i++){
		if(!this->valid[i]){
			this->data[i].fillFrame(v);
			this->valid[i] = true;
			return i;
		}
	}
	return -1; //no empty frame is available

}

//returns number of valid entries in frame f
int MainMemory :: getValidEntries(int f){
	if(f >= this->totalFrames || !this->valid[f])
		return -1;
	return this->data[f].validEntries;
}


// returns value stored at location i in frame f
int MainMemory :: getVal(int f, int i){
	if(!this->valid[f] || i >= MEM_FRAME_SIZE)
		return -1;
	return this->data[f].arr[i];
}

//assigns value val to ith location of frame f
void MainMemory :: setVal(int f, int i, int val){
	if(!this->valid[f] || i >= MEM_FRAME_SIZE){
		cout << "accessing invalid address" << endl;
		exit(1);
	}

	this->data[f].arr[i] = val;
	if(this->data[f].validEntries == i)
		this->data[f].validEntries = i+1;

}

//write Frame f to file at page p
void MainMemory :: writeFrame(DiskFile &inputFile, int f, int p){
	inputFile.data[p].validEntries = getValidEntries(f);

	for(int i = 0; i <	MEM_FRAME_SIZE; i++){
		inputFile.data[p].arr[i] = this->data[f].arr[i];
	}
}

//clears frame f
void MainMemory :: freeFrame(int f){
	if(f < totalFrames)
		this->valid[f] = false;
}



//creates DiskFile by reading input file through cin
void DiskFile :: readDiskFile(int n){

	int i = 0;
	this->size  = 0;
	this->totalPages = 0;
	Page t;
	vector<int> temp(DISK_PAGE_SIZE,-1);
	bool flag = true;
	int x;
	for(int j=0;j<n;j++){
		cin>>x;
		//cout<<x<<endl;
		flag = false;
		temp[i++] = x;
		this->size++;

		if(i == DISK_PAGE_SIZE){
			t.fillPage(temp);
			this->data.push_back(t);
			flag = true;
			this->totalPages++;
			i = 0;
		}
	}

	if(!flag){

		while(i != DISK_PAGE_SIZE){
			temp[i++] = -1;
		}
		t.fillPage(temp);
		this->data.push_back(t);
		this->totalPages++;
	}
}

//writes disk file to console
vector<vector<int> > DiskFile :: writeDiskFile(){
	vector<vector<int> > v;
	cout << "Contents of Disk File: " <<endl;
	for(int  i = 0; i < totalPages; i++){
		cout << "Page: " << i << ": ";
		v.push_back(this->data[i].writePage());
	}
	return v;
}

//copies contents of disk file f into destination disk file from startPage to endPage
void DiskFile :: DiskFileCopy(DiskFile &f, int startPage, int endPage){
	int j = 0; //start of source file
	int i = startPage; //start of destination file

	while(i <= endPage && j < f.totalPages && i < this->totalPages){
		this->data[i].validEntries = f.data[j].validEntries;
		for(int k = 0; k < DISK_PAGE_SIZE; k++){
			this->data[i].arr[k] = f.data[j].arr[k];
		}
		i++;
		j++;
	}
}



class ExtMergeSort{
public:
	int runSize; // size of run in terms of number of pages
	int totalPass; // number of passes performed
	int totalRuns;

	ExtMergeSort(){
		runSize = 0;
		totalPass = 0;
		totalRuns = -1;
	}

	void firstPass(DiskFile &inputFile, MainMemory &memory);
	void sortFrame(MainMemory &memory, int f);
	void merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int mid, int rightEnd);
	void twoWaySort(DiskFile &inputFile, MainMemory &memory);
	void mergek(DiskFile &inputFile, MainMemory &memory, vector<int> v, int inBufSize, int outBufSize);
	void KWaySort1(DiskFile &inputFile, MainMemory &memory, int in, int k, int total, int offset);
	void KWaySort(DiskFile &inputFile, MainMemory &memory, int in, int k, int total, int offset);

};




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
	//cout << "First Pass Performed" << endl;
	//inputFile.writeDiskFile(); //print file to cout
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
			this->mergek(inputFile, memory, v, 1, 1);
			//inputFile.writeDiskFile();
		}
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}

map<int, vector<vector<int> > > my;

void ExtMergeSort :: KWaySort1(DiskFile &inputFile, MainMemory &memory, int in, int k, int total, int offset){

	if(memory.totalFrames < k+1)
	{
		cout << "Error: "<<k<<" way merge sort requires atleast "<<k+1<<" frames but only "<<memory.totalFrames<<" are free"<<endl;
		exit(1);
	}

	this->firstPass(inputFile, memory);
	my[totalPass] = inputFile.writeDiskFile();

	int leftStart;

	for(this->runSize = in; this->runSize < total; this->runSize *= k){
		//cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < total; leftStart += k*this->runSize){
			int rightEnd = min(leftStart + k*this->runSize-1, total-1);
			//cout << "calling merge for < " << leftStart <<", " << rightEnd << " >" << endl;
			vector<int> v;
			for(int mid = leftStart;mid <= rightEnd;mid += this->runSize){
				v.push_back(offset+mid);
				v.push_back(offset+min(mid+this->runSize-1, rightEnd));
			}
			this->mergek(inputFile, memory, v, 1, 1);
			//inputFile.writeDiskFile();
			//exit(1);
			//inputFile.writeDiskFile();
		}
	}

	//cout << "Total Passes required: " << totalPass << endl;
}


void ExtMergeSort :: KWaySort(DiskFile &inputFile, MainMemory &memory, int in, int k, int total, int offset){

	if(memory.totalFrames < k+1)
	{
		cout << "Error: "<<k<<" way merge sort requires atleast "<<k+1<<" frames but only "<<memory.totalFrames<<" are free"<<endl;
		exit(1);
	}

	//this->firstPass(inputFile, memory);

	int leftStart;

	for(this->runSize = in; this->runSize < total; this->runSize *= k){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < total; leftStart += k*this->runSize){
			int rightEnd = min(leftStart + k*this->runSize-1, total-1);
			cout << "calling merge for < " << leftStart <<", " << rightEnd << " >" << endl;
			vector<int> v;
			for(int mid = leftStart;mid <= rightEnd;mid += this->runSize){
				v.push_back(offset+mid);
				v.push_back(offset+min(mid+this->runSize-1, rightEnd));
			}
			//for(int i=0;i<v.size();i++)
			//	cout<<v[i]<<" ";
			//cout<<endl;
			this->mergek(inputFile, memory, v, 1, 1);
			//inputFile.writeDiskFile();
			//exit(1);
		}
		totalPass++;
		my[totalPass] = inputFile.writeDiskFile();
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


void ExtMergeSort :: mergek(DiskFile &inputFile, MainMemory &memory, vector<int> v, int inBufSize, int outBufSize){

	int k = v.size()/2;
	int finalRunSize = v[v.size()-1] - v[0] + 1;
	int leftStart = v[0];
	int rightEnd = v[v.size()-1];
	DiskFile tempFile(finalRunSize);
	vector< queue<int> > inBuf;
	queue<int> q, outBuf;
	for(int j=0;j<inBufSize;j++)
		q.push(0);
	for(int j=0;j<outBufSize;j++)
			outBuf.push(0);
	for(int i=0;i<k;i++){
			inBuf.push_back(q);
	}

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
		//cout<<pq.top().f<<" ";
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


int main()
{
	int x;

	//cin>>DISK_PAGE_SIZE;
	//MEM_FRAME_SIZE = DISK_PAGE_SIZE;
	//reads size of main memory in terms of number of frames available
	cin>>DISK_PAGE_SIZE;
	MEM_FRAME_SIZE = DISK_PAGE_SIZE;
	cin >> x;
	//cout<<x<<" ";

	int k;
	cin>>k;
	//cout<<k<<" ";

	//create main memory of x frames
	MainMemory mm(x);
	//cout<<"Safe\n";

	int n;
	cin>>n;
	//cout<<n<<endl;
	//create a file by taking input from cin
	DiskFile f;
	f.readDiskFile(n);
	//f.writeDiskFile();

	ExtMergeSort e;

	//call 2 way externalmerge sort
	//e.twoWaySort(f,mm);
	int in = ((float)(x-k))/(float)k;
	cout<<x<<" "<<k<<" "<<n<<" "<<in<<endl;
	int out = x - (k*in);
	int i;
	for(i=0;i<=f.totalPages;i+=x)
		e.KWaySort1(f,mm,1,in,min(x, f.totalPages-i),i);
	e.KWaySort(f,mm,x,in,f.totalPages,0);
	int temp = 0,y;
	while(cin>>x){
		if(x<0)
			break;
		if(temp%2==0){
			y=x;
			temp = 1;
		}
		else{
			cout<<"Pass "<<y<<", Page "<<x<<": ";
			x--;
			temp = 0;
			if(my[y].size() == 0)
				cout<<"Invalid pass number\n";
			else if(x >= my[y].size())
				cout<<"Invalid page number\n";
			else
			{
				for(int i=0;i<my[y][x].size();i++)
					cout<<my[y][x][i]<<" ";
				cout<<endl;
			}
		}

	}

	//output file by using cout
	//f.writeDiskFile();

}
