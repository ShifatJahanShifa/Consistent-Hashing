#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

ll hashOfServer(string s)
{
    hash<string> h;
    return h(s);
}

map<ll,string>mpServer;
map<ll,pair<string,int>>hashRing;
map<ll,string>mpKey;
map<string,string>keyToServer;
map<string,vector<string>>serverToKey;

void keyAllocate()
{
    vector<string>keys;
    for(auto key:hashRing)
    {
        int id=key.second.second;
        if(id==1)
        {
            string temp=key.second.first;
            for(auto it:keys)
            {
               serverToKey[temp].push_back(it);
               keyToServer[it]=temp;
            }
            keys.erase(keys.begin(),keys.end());
        }
        else 
        {
            keys.push_back(key.second.first);
        }
    }
    if(keys.size())
    {
        for(auto key:hashRing)
        {
            int id=key.second.second;
            if(id==1)
            {
                string temp=key.second.first;
                for(auto it:keys)
                {
                    serverToKey[temp].push_back(it);
                    keyToServer[it]=temp;
                }
                break;
            }
        }
    }
}

void reAllocate(string removeServer)
{
    ll a=hashOfServer(removeServer);
    auto it=hashRing.find(a);
    vector<string>keys;
    bool ok=false;

    // anti-clockwise
    int cnt=0;
    if(hashRing.begin()!=it)
    it--;
    for(; it!=hashRing.begin();it--)
    {
        if(it->second.second==2)
        {
            keys.push_back(it->second.first);
        }
        else 
        {
            ok=true;
            break;
        }
        cnt++;
    }
    if(!ok)
    {
        if(it->second.second==2)
        {
            keys.push_back(it->second.first);
        }
        else if(cnt==0){} 
        else 
        {
            ok=true;
        }
    }
    if(!ok)
    {
        it=hashRing.end();
        it--;
        for(; it!=hashRing.begin();it--)
        {
            if(it->second.second==2)
            {
                keys.push_back(it->second.first);
            }
            else 
            {
                ok=true;
                break;
            }
        }
    }

    // clockwise
    auto it1=hashRing.find(a);
    it1++;
    string serverName;
    bool ok1=false;
    for( ; it1!=hashRing.end();it1++)
    {
        if(it1->second.second==1)
        {
            serverName=(it1->second.first);
            ok1=true;
            break;
        }
    }
    if(!ok1)
    {
        it1=hashRing.begin();
        for( ; it1!=hashRing.end();it1++)
        {
            if(it1->second.second==1)
            {
                serverName=(it1->second.first);
                ok1=true;
                break;
            }
        }
    }

    // now reallocate the keys
    for(auto key:keys)
    {
        keyToServer[key]=serverName;
        serverToKey[serverName].push_back(key);
    }
    serverToKey.erase(removeServer);
    hashRing.erase(a);
}

void reAllocate1(string newServer)
{
    ll a=hashOfServer(newServer);
    vector<string>keys;
    auto it=hashRing.find(a);
    bool ok=false;
    // anti-clockwise
    // int cnt=0;
    // if(hashRing.begin()!=it)
    it--;
    for(; it!=hashRing.begin();it--)
    {
        if(it->second.second==2)
        {
            keys.push_back(it->second.first);
        }
        else 
        {
            ok=true;
            break;
        }
        // cnt++;
    }
    if(!ok)
    {
        if(it->second.second==2)
        {
            keys.push_back(it->second.first);
        }
        // else if(cnt==0){} 
        else 
        {
            ok=true;
        }
    }
    if(!ok)
    {
        it=hashRing.end();
        it--;
        for(; it!=hashRing.begin();it--)
        {
            if(it->second.second==2)
            {
                keys.push_back(it->second.first);
            }
            else 
            {
                ok=true;
                break;
            }
        }
    }

    // clockwise
    auto it1=hashRing.find(a);
    it1++;
    string serverName;
    bool ok1=false;
    for( ; it1!=hashRing.end();it1++)
    {
        if(it1->second.second==1)
        {
            serverName=(it1->second.first);
            ok1=true;
            break;
        }
    }
    if(!ok1)
    {
        it1=hashRing.begin();
        for( ; it1!=hashRing.end();it1++)
        {
            if(it1->second.second==1)
            {
                serverName=(it1->second.first);
                ok1=true;
                break;
            }
        }
    }
    // now reallocate the keys
    for(auto key:keys)
    {
        keyToServer[key]=newServer;
        serverToKey[newServer].push_back(key);
        serverToKey[serverName].erase(find(serverToKey[serverName].begin(),serverToKey[serverName].end(),key));
    }
}


void viewAll()
{
    cout<<"------hash-ring----------\n";
    for(auto it:hashRing)
    {
        cout<<it.first<<" "<<it.second.first<<"\n";
    }
    for(auto it:serverToKey)
    {
        if(it.second.size())
        cout<<"server: "<<it.first<<" : ";
        for(auto key:it.second)
        {
            cout<<key<<" , ";
        }
    }
    puts("");
}

void reAllocate2(string newKey)
{
    ll v=hashOfServer(newKey);
    auto it=hashRing.find(v);
    it++;
    bool ok=false;
    string serverName="";
    // clockwise
    for( ; it!=hashRing.end();it++)
    {
        if(it->second.second==1)
        {
            ok=true;
            serverName=it->second.first;
            break;
        }
    }
    if(!ok)
    {
        it=hashRing.begin();
        for( ; it!=hashRing.end();it++)
        {
            if(it->second.second==1)
            {
                ok=true;
                serverName=it->second.first;
                break;
            }
        }
    }
    if(ok)
    {
        serverToKey[serverName].push_back(newKey);
        keyToServer[newKey]=serverName;
    }
}

int main()
{
    freopen("input.txt","r",stdin);
    cout<<"welcome to the consistent hashing\n";
    int server=0,key=0;
    cout<<"enter the number of servers\n";
    cin>>server;
    cout<<"enter the number of keys\n";
    cin>>key;

    // hasing the servers
    for(int i=0;i<server;i++)
    {
        string serverName;
        cout<<"enter the server name\n";
        cin>>serverName;
        ll ans=hashOfServer(serverName);
        hashRing[ans]=make_pair(serverName,1);
    }    

    // hashing the keys
    for(int i=0;i<key;i++)
    {
        string keyName;
        cout<<"enter the key name\n";
        cin>>keyName;
        ll ans=hashOfServer(keyName);
        hashRing[ans]=make_pair(keyName,2);
    }

   
    keyAllocate();
    viewAll();
    // key lookup
    cout<<"\nenter the key to search: ";
    string keyName;
    cin>>keyName;
    cout<<keyName<<"\n";
    cout<<"key is present in the server: "<<keyToServer[keyName]<<"\n";

    // server lookup
    cout<<"enter the server to search: ";
    string serverName;
    cin>>serverName;
    cout<<serverName<<"\nkeys present in the server: \n";
    for(auto key:serverToKey[serverName])
    {
        cout<<key<<"\n";
    }

    // remove server
    cout<<"enter the server to remove: ";
    string removeServer;
    cin>>removeServer;
    reAllocate(removeServer);
    cout<<removeServer<<"\nserver removed\n";
    viewAll();
    
    // key lookup
    cout<<"enter the key to search: ";
    // string keyName;
    cin>>keyName;
    cout<<keyName<<"\n";
    cout<<"key is present in the server: "<<keyToServer[keyName]<<"\n";

    // server lookup
    cout<<"enter the server to search: ";
    // string serverName;
    cin>>serverName;
    cout<<serverName<<"\nkeys present in the server: "<<serverName<<"\n";
    for(auto key:serverToKey[serverName])
    {
        cout<<key<<"\n";
    }

    // add a new server
    cout<<"enter the server to add: ";
    string newServer;
    cin>>newServer;
    hashRing[hashOfServer(newServer)]=make_pair(newServer,1);
    reAllocate1(newServer);

    cout<<newServer<<"\nserver added\n";
    viewAll();
    
    // key lookup
    cout<<"enter the key to search:";
    // string keyName;
    cin>>keyName;
    cout<<keyName<<"\n";
    cout<<"key is present in the server: "<<keyToServer[keyName]<<"\n";

    // server lookup
    cout<<"enter the server to search:";
    // string serverName;
    cin>>serverName;
    cout<<serverName<<"\nkeys present in the server: "<<serverName<<"\n";
    for(auto key:serverToKey[serverName])
    {
        cout<<key<<"\n";
    }

    // add key
    cout<<"enter the key to add:";
    string newKey;
    cin>>newKey;
    hashRing[hashOfServer(newKey)]=make_pair(newKey,2);
    reAllocate2(newKey);
    cout<<newKey<<"\nkey added\n";
    viewAll();
    
    // key lookup
    cout<<"enter the key to search:";
    // string keyName;
    cin>>keyName;
    cout<<keyName<<"\n";
    cout<<"key is present in the server: "<<keyToServer[keyName]<<"\n";

    // server lookup
    cout<<"enter the server to search\n";
    // string serverName;
    cin>>serverName;
    cout<<serverName<<"\nkeys present in the server: "<<serverName<<"\n";
    for(auto key:serverToKey[serverName])
    {
        cout<<key<<"\n";
    }


    // again remove
    cout<<"enter the server to remove: ";
    // string removeServer;
    cin>>removeServer;
    reAllocate(removeServer);
    cout<<removeServer<<"\nserver removed\n";
    viewAll();
    
    // key lookup
    cout<<"enter the key to search: ";
    // string keyName;
    cin>>keyName;
    cout<<keyName<<"\n";
    cout<<"key is present in the server: "<<keyToServer[keyName]<<"\n";

    // server lookup
    cout<<"enter the server to search: ";
    // string serverName;
    cin>>serverName;
    cout<<serverName<<"\nkeys present in the server: "<<serverName<<"\n";
    for(auto key:serverToKey[serverName])
    {
        cout<<key<<"\n";
    }

}