#include<bits/stdc++.h>
#define pii pair<int,int>
#define endl '\n'
using namespace std;

const int N=300005;
const int INF=INT_MAX;

vector<pii>edge[N];
int n,s;

int len[N];
int st,ed;

int dia[N],cnt;
int nxt[N];
bitset<N>on_dia;

void dfs(int u,int fa,bool sign){
    for(auto [v,w]:edge[u]){
        if(v==fa)continue;
        if(sign)nxt[v]=u;
        len[v]=len[u]+w;
        dfs(v,u,sign);
    }
}

int get_len(int u,int fa){
    int ret=0;
    for(auto [v,w]:edge[u]){
        if(v==fa)continue;
        if(on_dia[v])continue;
        ret=max(ret,get_len(v,u)+w);
    }
    return ret;
}
int out_len[N];
int pre[N];
int suf[N];

int pre_max[N];
int suf_max[N];

deque<int>q;

void solve(int turn){
    cin>>n>>s;
    for(int i=1;i<n;++i){
        int u,v,w;
        cin>>u>>v>>w;
        edge[u].push_back({v,w});
        edge[v].push_back({u,w});
    }

    dfs(1,0,false);
    st=1;
    for(int i=1;i<=n;++i){
        if(len[i]>len[st]){
            st=i;
        }
    }
    for(int i=1;i<=n;++i)len[i]=0;
    dfs(st,0,true);
    ed=1;
    for(int i=1;i<=n;++i){
        if(len[i]>len[ed]){
            ed=i;
        }
    }

    int now=ed;
    cnt=0;
    do{
        dia[++cnt]=now;
        on_dia[now]=true;
        now=nxt[now];
    }while(now);

    for(int i=1;i<=cnt;++i){
        out_len[i]=get_len(dia[i],0);
    }
    for(int i=1;i<=cnt;++i){
        for(auto [v,w]:edge[dia[i]]){
            if(i>1&&v==dia[i-1]){
                pre[i]=w;
            }
            if(i<n&&v==dia[i+1]){
                suf[i]=w;
            }
        }
    }
    for(int i=1;i<=n;++i)pre[i]+=pre[i-1];
    for(int i=cnt;i>=1;--i)suf[i]+=suf[i+1];

    for(int i=1;i<=cnt;++i){
        pre_max[i]=max(out_len[i]-pre[i],pre_max[i-1]);
    }
    for(int i=cnt;i>=1;--i){
        suf_max[i]=max(out_len[i]-suf[i],suf_max[i+1]);
    }
    

    int ans=INF;
    int l=1,r=1;
    q.push_back(1);
    while(r<=cnt){
        while(!q.empty()&&out_len[q.back()]<=out_len[r])q.pop_back();
        q.push_back(r);

        while(pre[r]-pre[l]>s){
            ++l;
            if(q.front()<l)q.pop_front();
        }

        ans=min(ans,max(max(pre[l]+pre_max[l-1], suf[r]+suf_max[r+1]),out_len[q.front()]));
        ++r;
    }
    cout<<ans<<endl;

    return;
}

signed main(void){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int T=1; //cin>>T;
    for(int i=1;i<=T;++i)solve(i);
    return 0;
}