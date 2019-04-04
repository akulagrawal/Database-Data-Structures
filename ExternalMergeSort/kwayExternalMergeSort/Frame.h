#pragma once
#include "common.h"

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