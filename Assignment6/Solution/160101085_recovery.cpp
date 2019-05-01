#include <bits/stdc++.h>

using namespace std;

struct transaction{
	int id;
	char comm;
	string obj;
	int val;
};

struct logs{
	int id;
	int oldVal;
	int newVal;
	string obj;
	char comm;
	vector<int> active;
};


/*this class contains declarations of all the data structures that are used to perform the given tasks*/

class Transactions{


public:
	void readInput();   // to read the given input file
	void PossibleChains();  // to find all possible chains
	void isRecoverable();  // to check whether schedule is recoverable or not
	void isCascadeless();  // to check whether schedule is cascadeless or not
	void makeprecedenceGraph();
	bool isConflictSerializable();
	void displayGraph();
	bool isCycle(int u, int color[]);
	void writelogs();
	void makelogs();
	void recover();


private:
	vector<transaction> tList;
	// it stores all the trasaction details
    unordered_map<int,int> commit;
    // it stores the position of commit operation of a trasaction
    vector<pair<int,vector<int> > > chain;
    // to store all the possible chains
    vector< pair<int,pair<int,int> > > firstReadArray;
    // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid
    vector< vector<int> > precedenceGraph;
	unordered_map<string,int> object;
	vector<string> objectList;
	vector<int> active;
	vector<logs> logsList;
	unordered_map<int,int> isAbort;
};



void Transactions::readInput(){
	/*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'

	*/
	int Tid;
    string DataItem;
    char Opr;
    int n;
    string s;
    int val;
    cin>>n;
    while(n--){
        cin>>s>>val;
				objectList.push_back(s);
        object[s] = val;
    }
    transaction temp;
    while(cin>>Opr){
        cin>>Tid;
        temp.id = Tid;
        temp.comm = Opr;
        if((Opr=='R')||(Opr=='W')){
            cin>>DataItem;
            temp.obj = DataItem;
            if(Opr=='W'){
                cin>>val;
                temp.val = val;
            }
        }
        // Not covering if input format is wrong.
        tList.push_back(temp);
    }
}


void Transactions::PossibleChains(){
	/*
	    --> this method find all the possible chains and store it in d.s. 'chain'
	    --> each chain represents collection of read operations on a dataitem that is written by a transaction before those read operations

	*/
	vector<int> temp;
	pair<int,int> firstRead;
	for(int i=0; i<tList.size(); i++){
        if(tList[i].comm!='W')
            continue;

        string DataItem=tList[i].obj;

        for(int j=i+1; j<tList.size(); j++){
            if(tList[j].obj!=DataItem)
                continue;
            if(tList[j].comm=='R'){
            	if(temp.size()==0)
            		firstRead={tList[j].id,j};
            	temp.push_back(tList[j].id);
            }
            if(tList[j].comm=='W')
                break;
        }
        if(temp.size()){
            chain.push_back({tList[i].id,temp});
            firstReadArray.push_back({tList[i].id,{firstRead}});
        }
        temp.clear();
    }
}


void Transactions::isRecoverable(){
	/*
         --> this method checks whether the given schedule is recoverable or not
	*/
	for(int i=0; i<chain.size(); i++){
        int x = commit[chain[i].first];
        for(int j=0; j<chain[i].second.size(); j++){
            if(x>=commit[chain[i].second[j]]){
            	cout<<"No"<<endl;
            	cout<<"Transaction "<<chain[i].second[j]<<" is reading DataItem written by Transaction "<<chain[i].first<<endl;
            	cout<<"And Transaction "<<chain[i].first<<" commits after Transaction "<<chain[i].second[j]<<endl;
            	return;
            }
        }
    }
    cout<<"Yes"<<endl;
}


void Transactions::isCascadeless(){
	/*
         --> this method checks whether the given schedule is cascadeless or not
	*/
	for(int i=0; i<firstReadArray.size(); i++){
        int x = commit[firstReadArray[i].first];
        if(x>=firstReadArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadArray[i].first<<" commits after reading by Transaction "<<firstReadArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}

void Transactions::makeprecedenceGraph(){
	int maxm = -1;
	for(int i=0;i<tList.size();i++)
		maxm=max(maxm, tList[i].id);
	precedenceGraph.resize(maxm+1);
	for(int i=0;i<tList.size();i++){
		if(tList[i].comm != 'C'){
			for(int j=i+1;j<tList.size();j++){
				if(tList[j].comm == 'C')
					continue;
				if((tList[i].comm == 'R') && (tList[j].comm == 'R'))
					continue;
				if(tList[i].obj != tList[j].obj)
					continue;
				int u = tList[i].id, v = tList[j].id;
				if(u==v)
					continue;
				int k;
				for(k=0;k<precedenceGraph[u].size();k++)
					if(precedenceGraph[u][k] == v)
						break;
				if(k>=precedenceGraph[u].size()){
					precedenceGraph[u].push_back(v);
        }
			}
		}
	}
}

void Transactions::displayGraph(){
	for(int i=0;i<precedenceGraph.size();i++)
	{
		cout<<i<<": ";
		for(int j=0;j<precedenceGraph[i].size();j++)
		{
			cout<<precedenceGraph[i][j]<<" ";
		}
		cout<<endl;
	}
}

bool Transactions::isCycle(int u, int color[]) {
	//cout<<u<<endl;
    color[u] = 1;
    for (int i = 0; i < precedenceGraph[u].size(); i++){
        int v = precedenceGraph[u][i];
        if (color[v] == 1){
        	//cout<<u<<" and "<<v<<" have conflict\n";
        	return true;
        }
        if ((color[v] == 0) && isCycle(v, color))
        	return true;
    }
    color[u] = 2;
    return false;
}

bool Transactions::isConflictSerializable(){
    int color[precedenceGraph.size()];
    memset(color, 0, sizeof(color));
    for (int i = 0; i < precedenceGraph.size(); i++)
    	if (color[i] == 0)
    		if (isCycle(i, color)){
    			return false;
    		}
    return true;
}

void Transactions::writelogs(){
	ofstream myfile;
	myfile.open ("transaction.log", ios::out);
	char Opr;
	for(int i=0;i<tList.size();i++){
		Opr = tList[i].comm;
		if(Opr == 'R')
			continue;
		if(Opr == 'S')
			active.push_back(tList[i].id);
		else if(Opr == 'C')
			active.erase(remove(active.begin(), active.end(), tList[i].id), active.end());
		myfile<<tList[i].id<<",";
		if(Opr == 'W'){
			myfile<<tList[i].obj<<","<<object[tList[i].obj]<<","<<tList[i].val;
			object[tList[i].obj] = tList[i].val;
		}
		else
			myfile<<tList[i].comm;
		myfile<<endl;
		myfile<<"Checkpoint";
		for(int i=0;i<active.size();i++)
			myfile<<","<<active[i];
		myfile<<endl;
	}
	myfile.close();
}

void Transactions::makelogs(){
	logs temp;
	logsList.clear();
	for(int i=0;i<tList.size();i++){
		temp.id = tList[i].id;
		temp.oldVal = object[tList[i].obj];
		temp.newVal = tList[i].val;
		temp.comm = tList[i].comm;
		temp.obj = tList[i].obj;
		if(temp.comm == 'R')
			continue;
		if(temp.comm == 'S')
			active.push_back(temp.id);
		else if(temp.comm == 'C')
			active.erase(remove(active.begin(), active.end(), tList[i].id), active.end());
		else if(temp.comm == 'W'){
			object[tList[i].obj] = tList[i].val;
		}
		logsList.push_back(temp);
	}
	for(int i=0;i<logsList.size();i++){
		cout<<logsList[i].comm<<endl<<logsList[i].id<<endl;
		if(logsList[i].comm == 'W')
			cout<<logsList[i].obj<<endl<<logsList[i].oldVal<<endl<<logsList[i].newVal<<endl;
	}

}


void Transactions::recover(){
	int n = logsList.size();
	for(int i=n-1;i>=0;i--){
		for(int j=0;j<active.size();j++){
			if(active[j]==logsList[i].id){
				if(logsList[i].comm=='W'){
					object[logsList[i].obj] = logsList[i].oldVal;
				}
				else if(logsList[i].comm=='S'){
					isAbort[logsList[i].id] = 1;
					active.erase(remove(active.begin(), active.end(), logsList[i].id), active.end());
				}
				break;
			}
		}
		if(!active.size())
			break;
	}
	for(int i=0;i<n;i++){
		if(!isAbort[logsList[i].id]){
			if(logsList[i].comm=='W'){
				object[logsList[i].obj] = logsList[i].newVal;
			}
		}
	}

	for(int i=0;i<objectList.size();i++)
		cout<<objectList[i]<<" = "<<object[objectList[i]]<<endl;
}


int main(){

	Transactions obj;

	obj.readInput();   // to read input file
	obj.makelogs();
	obj.recover();

	/*obj.PossibleChains(); //to find all possible chains

	/*Each chain represents collection of read operation on a dataitem that is written by a transaction before those read operations*/

	//obj.isRecoverable();  // to check whether schedule is recoverable or not

	//obj.isCascadeless(); // to check whether schedule is cascadeless or not

	/*obj.makeprecedenceGraph();

	//obj.displayGraph();

	if(obj.isConflictSerializable()){
		cout<<"Conflict serializable: Yes\n";
	}
	else{
		cout<<"Conflict serializable: No\n";
	}*/

	return 0;

}
