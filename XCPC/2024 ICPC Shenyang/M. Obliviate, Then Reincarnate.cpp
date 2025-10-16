#include<bits/stdc++.h>
#define endl '\n'
#define ll long long
#define PII pair<int,int>
using namespace std;

const int N=500005;

int n,m,q;

int mo(int x){return (x%n+n)%n;}

vector<PII>to[N];
bitset<N>in_stack,loop;
int st[N],tp;
int dfn[N],low[N],cnt,sc[N],sz[N],hd[N],scc;

void tarjan(int u){
    dfn[u]=++cnt; low[u]=dfn[u];
    st[++tp]=u; in_stack[u]=true;
    for(auto[v,w]:to[u]){
        if(!dfn[v]){
            tarjan(v);
            low[u]=min(low[u],low[v]);
        }else if(in_stack[v]){
            low[u]=min(low[u],dfn[v]);
        }
    }
    if(dfn[u]==low[u]){
        ++scc;
        hd[scc]=u;
        do{
            sc[st[tp]]=scc;
            ++sz[scc];
            in_stack[st[tp]]=false;
        }while(st[tp--]!=u);
    }
}

bitset<N>vis;
ll dis[N];
bool check_scc(int scnum){//check if the scc is zero loop
    queue<int>q;
    q.push(hd[scnum]); vis[hd[scnum]]=true;
    while(!q.empty()){
        int u=q.front(); q.pop();
        for(auto[v,w]:to[u]){
            if(sc[v]!=scnum)continue;
            if(!vis[v]){
                vis[v]=true;
                dis[v]=dis[u]+w;
                q.push(v);
            }else if(dis[v]!=dis[u]+w){
                return false;
            }
        }
    }
    return true;
}

vector<int>topo_edge[N];
int rd[N];
void topo(){
    queue<int>q;
    for(int i=1;i<=scc;++i){
        if(rd[i]==0)q.push(i);
    }
    while(!q.empty()){
        int u=q.front(); q.pop();
        for(int v:topo_edge[u]){
            loop[v]=loop[v]|loop[u];
            if(--rd[v]==0){
                q.push(v);
            }
        }
    }
}

void solve(int turn){
    cin>>n>>m>>q;
    for(int _=1;_<=m;++_){
        int a,b; cin>>a>>b;
        int u=mo(a); int v=mo(a+b);
        to[u].push_back({v,b});
    }

    for(int i=0;i<n;++i)if(!dfn[i])
        tarjan(i);

    for(int i=1;i<=scc;++i)
        loop[i]=!check_scc(i);

    for(int i=0;i<n;++i)for(auto[v,w]:to[i])if(sc[i]!=sc[v]){
        topo_edge[sc[v]].push_back(sc[i]);
        ++rd[sc[i]];
    }

    topo();

    for(int _=1;_<=q;++_){
        int x; cin>>x;
        if(loop[sc[mo(x)]]){
            cout<<"Yes"<<endl;
        }else{
            cout<<"No"<<endl;
        }
    }
    return;
}

signed main(void){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int T=1; //cin>>T;
    for(int i=1;i<=T;++i)solve(i);
    return 0;
}
