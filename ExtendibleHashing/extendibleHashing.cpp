#include<bits/stdc++.h>
using namespace std;
#define f first
#define s second
#define ll long long
#define mp make_pair
#define MAX 1000006
#define mod 1000000007
#define pb push_back
#define INF 1e18
#define pii pair<int,int>

#define BUCKET_SIZE 2

vector<pair<list<pair<int,string> >, int>* > v;
int globalDepth;

int main()
{
    //freopen ("input1.in","r",stdin);
    //freopen ("output22.txt","w",stdout);

    globalDepth = 1;
    string str;
    int key, new_key;
    for(int i=0;i<2;i++)
    {
    	v.pb(new pair<list<pair<int,string> >, int>);
    	(*v[i]).s = 1;
    }
    while(1){
    	cin>>key>>str;
    	while(1)
    	{
    		new_key = key%(1<<globalDepth);
    		if((*v[new_key]).f.size() < BUCKET_SIZE)
    		{
    			(*v[new_key]).f.pb(mp(key, str));
    			break;
    		}
    		else
    		{
    			if((*v[new_key]).s == globalDepth)
    			{
    				for(int i=0;i<(1<<globalDepth);i++)
    					v.pb(v[i]);
                    globalDepth++;
    			}
    			new_key = key%(1<<(*v[new_key]).s);
    			list<pair<int,string> > l;
    			int new_mod = new_key + (1<<(*v[new_key]).s);
    			(*v[new_key]).s++;
    			v[new_mod] = new pair<list<pair<int,string> >, int>;
    			(*v[new_mod]).s = (*v[new_key]).s;
    			for(int i=new_mod;i<(1<<globalDepth);i+=(1<<(*v[new_key]).s))
    				v[i] = v[new_mod];
    			list <pair<int,string> > :: iterator it;
    			for(it = (*v[new_key]).f.begin(); it != (*v[new_key]).f.end();) 
    			{
    				if((*it).f%(1<<(*v[new_key]).s) == new_mod)
    				{
    					(*v[new_mod]).f.pb(*it);
    					it = (*v[new_key]).f.erase(it);
    				}
    				else
    					it++;
    			}
    		}
    	}
    	cout<<globalDepth<<endl;
    	for(int i=0;i<v.size();i++)
    	{
    		cout<<i<<"- ";
    		for(list <pair<int,string> > :: iterator it = (*v[i]).f.begin();it != (*v[i]).f.end(); it++)
    			cout<<(*it).f<<":"<<(*it).s<<" ";
    		cout<<"| "<<(*v[i]).s<<endl;
    	}
    }




    return 0;
}
