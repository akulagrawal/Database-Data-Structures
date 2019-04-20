#include <bits/stdc++.h>

using namespace std;


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
	bool isCommitOrder();
	bool check(int u, int vis[]);
	bool isPresent(int u, int v);
	bool isMultiCommitOrder();


private:
	vector<pair<int, pair<char,int> > > transaction;
	// it stores all the trasaction details
    unordered_map<int,int> commit;
    // it stores the position of commit operation of a trasaction
    vector<pair<int,vector<int> > > chain;
    // to store all the possible chains
    vector< pair<int,pair<int,int> > > firstReadArray;
    // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid
    vector< vector<int> > precedenceGraph;
		vector< vector<int> > reverseGraph;
};



void Transactions::readInput(){
	/*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'

	*/
	int Tid, DataItem=0;
    char Opr;
    while(cin>>Tid){
        cin>>Opr;
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();
        }
        // Not covering if input format is wrong.
        transaction.push_back({Tid,{Opr,DataItem}});
    }
}


void Transactions::PossibleChains(){
	/*
	    --> this method find all the possible chains and store it in d.s. 'chain'
	    --> each chain represents collection of read operations on a dataitem that is written by a transaction before those read operations

	*/
	vector<int> temp;
	pair<int,int> firstRead;
	for(int i=0; i<transaction.size(); i++){
        if(transaction[i].second.first!='W')
            continue;

        int DataItem=transaction[i].second.second;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.second!=DataItem)
                continue;
            if(transaction[j].second.first=='R'){
            	if(temp.size()==0)
            		firstRead={transaction[j].first,j};
            	temp.push_back(transaction[j].first);
            }
            if(transaction[j].second.first=='W')
                break;
        }
        if(temp.size()){
            chain.push_back({transaction[i].first,temp});
            firstReadArray.push_back({transaction[i].first,{firstRead}});
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
	for(int i=0;i<transaction.size();i++)
		maxm=max(maxm, transaction[i].first);
	precedenceGraph.resize(maxm+1);
  reverseGraph.resize(maxm+1);
	for(int i=0;i<transaction.size();i++){
		if(transaction[i].second.first != 'C'){
			for(int j=i+1;j<transaction.size();j++){
				if(transaction[j].second.first == 'C')
					continue;
				if((transaction[i].second.first == 'R') && (transaction[j].second.first == 'R'))
					continue;
				if(transaction[i].second.second != transaction[j].second.second)
					continue;
				int u = transaction[i].first, v = transaction[j].first;
				if(u==v)
					continue;
				int k;
				for(k=0;k<precedenceGraph[u].size();k++)
					if(precedenceGraph[u][k] == v)
						break;
				if(k>=precedenceGraph[u].size()){
					precedenceGraph[u].push_back(v);
          reverseGraph[v].push_back(u);
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

bool Transactions::check(int u, int vis[]){
  for(int i=0;i<reverseGraph[u].size();i++){
    if(check(reverseGraph[u][i], vis) == false)
      return false;
    if(vis[reverseGraph[u][i]] == 0)
      return false;
  }
  return true;
}

bool Transactions::isCommitOrder(){
  int vis[reverseGraph.size()];
  memset(vis, 0, sizeof(vis));
  for(int i=0;i<transaction.size();i++){
    if(transaction[i].second.first == 'C'){
      cout<<transaction[i].first<<" ";
      if(check(transaction[i].first, vis) == false){
        return false;
      }
      vis[transaction[i].first] = 1;
    }
  }
  cout<<endl;
  return true;
}

bool Transactions::isPresent(int u, int v){
  for(int i=0;i<precedenceGraph[u].size();i++){
    if(precedenceGraph[u][i] == v)
      return true;
    if(isPresent(precedenceGraph[u][i], v))
      return true;
  }
  return false;
}

bool Transactions::isMultiCommitOrder(){
  vector<int> com;
  for(int i=0;i<transaction.size();i++){
    if(transaction[i].second.first == 'C')
      com.push_back(transaction[i].first);
  }
  for(int i=0;i<com.size();i++){
    for(int j=i+1;j<com.size();j++){
      //cout<<com[i]<<" "<<com[j]<<endl;
      if(isPresent(com[j], com[i]))
        return false;
    }
  }
  return true;
}




int main(){

	Transactions obj;

	obj.readInput();   // to read input file

	obj.PossibleChains(); //to find all possible chains

	/*Each chain represents collection of read operation on a dataitem that is written by a transaction before those read operations*/

	//obj.isRecoverable();  // to check whether schedule is recoverable or not

	//obj.isCascadeless(); // to check whether schedule is cascadeless or not

	obj.makeprecedenceGraph();

	//obj.displayGraph();

	if(obj.isConflictSerializable()){
		cout<<"Conflict serializable: Yes\n";
		cout<<"Commit ordering: ";
		if(obj.isMultiCommitOrder())
			cout<<"Yes\n";
		else
			cout<<"No\n";
	}
	else{
		cout<<"Conflict serializable: No\n";
		cout<<"Commit ordering: Not applicable\n";
	}

	return 0;

}
