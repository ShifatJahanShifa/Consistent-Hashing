#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

ll hashOfServer(string s)
{
    hash<string> h;
    return h(s);
}

map<ll,string>mpServer;
map<ll,string>mpKey;
map<string,string>keyToServer;
map<string,vector<string>>serverToKey;

void keyAllocate()
{
    for(auto key:mpKey)
    {
        ll a=key.first;
        bool ok=true;
        for(auto server:mpServer)
        {
            ll b=server.first;
            if(a<=b)
            {
                keyToServer[key.second]=server.second;
                serverToKey[server.second].push_back(key.second);
                ok=false;
                // cout<<a<<" "<<b<<" dd \n";
                // cout<<"key: dd "<<key.first<<" "<<key.second<<"\n";
                // cout<<"server: dd "<<server.first<<" "<<server.second<<"\n";
                break;
            }
        }
        if(ok)
        {
            // cout<<"wdhucfhed\n";
            keyToServer[key.second]=mpServer.begin()->second;
            serverToKey[mpServer.begin()->second].push_back(key.second);
        }
    }
}

// next server
string findNextServer(string removeServer)
{
    string nextServer;
    auto it=mpServer.find(hashOfServer(removeServer));
    it++;
    if(it==mpServer.end())
    {
        nextServer=mpServer.begin()->second;
        cout<<"next server is: "<<nextServer<<"\n";
    }
    else nextServer=it->second;
    return nextServer;
    
}

// prev server
string findPrevServer(string removeServer)
{
    string prevServer;
    auto it=mpServer.find(hashOfServer(removeServer));
    if(it==mpServer.begin())
    {
        prevServer=mpServer.rbegin()->second;
        cout<<"prev server is: "<<prevServer<<"\n";
    }
    else 
    {
        it--;
        prevServer=it->second;
    }
    return prevServer;
}

void reAllocate(string removeServer,string nextServer,string prevServer)
{
    ll b=hashOfServer(removeServer);
    ll c=hashOfServer(nextServer);
    ll a=hashOfServer(prevServer);
    // easy
    if(a<=b)
    {
        for(auto it:mpKey)
        {
            if(it.first>=a and it.first<=b)
            {
                keyToServer[it.second]=nextServer;
                serverToKey[nextServer].push_back(it.second);
            }
        }
        
    }
    else 
    {
        for(auto it:mpKey)
        {
            if(it.first>=a or it.first<=b)
            {
                keyToServer[it.second]=nextServer;
                serverToKey[nextServer].push_back(it.second);
            }
        }
    }
    serverToKey.erase(removeServer);
    mpServer.erase(b);
}

void reAllocate1(string newServer,string nextServer,string prevServer)
{
    ll a=hashOfServer(newServer);
    ll b=hashOfServer(nextServer);
    ll c=hashOfServer(prevServer);

    for(auto it:mpServer)

    // easy
    if(a<=b)
    {
        for(auto it:mpKey)
        {
            if(it.first>=c and it.first<=a)
            {
                keyToServer[it.second]=newServer;
                serverToKey[newServer].push_back(it.second);
            }
        }
        
    }
    else 
    {
        for(auto it:mpKey)
        {
            if(it.first>=c or it.first<=a)
            {
                keyToServer[it.second]=newServer;
                serverToKey[newServer].push_back(it.second);
            }
        }
    }
}


void viewAll()
{
    for(auto it:serverToKey)
    {
        cout<<"server: "<<it.first<<" : ";
        for(auto key:it.second)
        {
            cout<<key<<" , ";
        }
    }
}

int main()
{
    freopen("input.txt","r",stdin);
    cout<<"welcome to the consistent hashing\n";\
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
        mpServer[ans]=serverName;
    }    

    // hashing the keys
    for(int i=0;i<key;i++)
    {
        string keyName;
        cout<<"enter the key name\n";
        cin>>keyName;
        ll ans=hashOfServer(keyName);
        mpKey[ans]=keyName;
    }

    // printing the servers
    for(auto it:mpServer)
    {
        cout<<it.first<<" "<<it.second<<"\n";
    }

    // printing the servers
    for(auto it:mpKey)
    {
        cout<<it.first<<" "<<it.second<<"\n";
    }
   
    keyAllocate();
    // key lookup
    cout<<"enter the key to search\n";
    string keyName;
    cin>>keyName;
    cout<<keyName<<"\n";
    cout<<"key is present in the server: "<<keyToServer[keyName]<<"\n";

    // server lookup
    cout<<"enter the server to search\n";
    string serverName;
    cin>>serverName;
    cout<<"keys present in the server: \n";
    for(auto key:serverToKey[serverName])
    {
        cout<<key<<"\n";
    }

    viewAll();
    // remove server
    cout<<"enter the server to remove\n";
    string removeServer;
    cin>>removeServer;
    string nextServer=findNextServer(removeServer);
    string prevServer=findPrevServer(removeServer);
    reAllocate(removeServer,nextServer,prevServer);
    cout<<"server removed\n";
    

    viewAll();

    // add a new server
    cout<<"enter the server to add\n";
    string newServer;
    cin>>newServer;
    mpServer[hashOfServer(newServer)]=newServer;
    prevServer=findPrevServer(newServer);
    nextServer=findNextServer(newServer);
    reAllocate1(newServer,nextServer,prevServer);

}