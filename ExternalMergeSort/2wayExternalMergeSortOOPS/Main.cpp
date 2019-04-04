#include "common.h"
#include "MainMemory.h"
#include "DiskFile.h"
#include "ExtMergeSort.h"

using namespace std;

int main()
{
	int x;
	//reads size of main memory in terms of number of frames available
	cin >> x;

	//create main memory of x frames
	MainMemory mm(x);


	//create a file by taking input from cin
	DiskFile f;
	f.readDiskFile();
	f.writeDiskFile();

	ExtMergeSort e;
	
	//call 2 way externalmerge sort
	e.twoWaySort(f,mm);

	//output file by using cout
	f.writeDiskFile(); 

}