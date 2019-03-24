#include<bits/stdc++.h>
using namespace std;
#define f first
#define s second
#define ll long long
#define mp make_pair
#define MAglobalDepth 1000006
#define mod 1000000007
#define pb push_back
#define INF 1e18
#define pii pair<int,int>

#define BUCKET_SIZE 2

vector<list<pair<int,string> > > v;
int globalDepth = 1;

int main()
{
    //freopen ("input1.in","r",stdin);
    //freopen ("output22.tglobalDeptht","w",stdout);

    string str;
    int key;
    list<pair<int,string> > l;
    v.pb(l);
    v.pb(l);
    while(1){
    	cin>>key>>str;
    	int new_key, old_key;
        if(v[new_key].size() >= BUCKET_SIZE)
        {
            v.pb(l);
            if((v.size()-1)/(1<<globalDepth))
                globalDepth++;
            globalDepth--;
            new_key = v.size() - 1;
            old_key = new_key - (1<<globalDepth);
            globalDepth++;
            for(auto itr = v[old_key].begin();itr != v[old_key].end();)
            {
                if(((*itr).f%(1<<globalDepth)) == new_key)
                {
                    v[new_key].pb((*itr));
                    itr = v[old_key].erase(itr);
                }
                else
                    itr++;
            }
        }
        new_key = key%(1 << globalDepth);
        v[new_key].pb(mp(key, str));

        for(int i=0;i<v.size();i++)
        {
            cout<<i<<"- ";
            for(auto it = v[i].begin();it != v[i].end(); it++)
                cout<<(*it).f<<":"<<(*it).s<<" ";
            cout<<endl;
        }
    }




    return 0;
}
