#include<bits/stdc++.h>
#define ull unsigned long long
#define ui128 ull
#define int long long
#define endl '\n'

using namespace std;

const int N=200000;
const ull MOD=(ull)1e9+7;

ull qpo(ull x,ull k){
    ull res=1ull;
    while(k){
        if(k&1ull)res=(ui128)res*x%MOD;
        x=(ui128)x*x%MOD;
        k>>=1ull;
    }
    return res;
}

const ull b1=2,invb1=qpo(b1,MOD-2ull);
const ull b2=3,invb2=qpo(b2,MOD-2ull);

int n,q;
int a[N+5];
ull pop1[N+5],pon1[N+5];
//ull pop2[N+5],pon2[N+5];

struct hh{
    ull hp1,hn1;
    //ull hp2,hn2;
    hh operator+(const hh& other)const{
        return{
            (hp1+other.hp1)%MOD,
            //(hp2+other.hp2)%MOD,
            (hn1+other.hn1)%MOD,
            //(hn2+other.hn2)%MOD,
        };
    }
    void update(int v){
        if(v<=N){
            hp1=(ui128)hp1*pop1[v]%MOD;
            //hp2=(ui128)hp2*pop2[v]%MOD;
            hn1=(ui128)hn1*pon1[v]%MOD;
            //hn2=(ui128)hn2*pon2[v]%MOD;
        }else{
            hp1=(ui128)hp1*qpo(b1,v)%MOD;
            //hp2=(ui128)hp1*qpo(b2,v);
            hn1=(ui128)hn1*qpo(invb1,v)%MOD;
            //hn2=(ui128)hn1*qpo(invb2,v);
        }
    }
};

struct Node{
    int ls,rs,L,R;
    int flag,sum;
    hh hpn;
}tree[N*2+5];

#define lsu tree[u].ls
#define rsu tree[u].rs
#define Lu tree[u].L
#define Ru tree[u].R
#define mid ((Lu+Ru)/2)

void init(){
    pop1[0]=pon1[0]=1ull;
    //pop2[0]=pon2[0]=1ull;
    for(int i=1;i<=N;++i){
        pop1[i]=(ui128)pop1[i-1]*b1%MOD;
        //pop2[i]=(ui128)pop2[i-1]*b2%MOD;
        pon1[i]=(ui128)pon1[i-1]*invb1%MOD;
        //pon2[i]=(ui128)pon2[i-1]*invb2%MOD;
    }
}

int cnt=0;
void apply(int u,int v){
    tree[u].sum+=v*(Ru-Lu+1);
    tree[u].flag+=v;
    tree[u].hpn.update(v);
}
void push_up(int u){
    tree[u].sum=tree[lsu].sum+tree[rsu].sum;
    tree[u].hpn=tree[lsu].hpn+tree[rsu].hpn;
}
void push_down(int u){
    if(tree[u].flag==0)return;
    apply(lsu,tree[u].flag);
    apply(rsu,tree[u].flag);
    tree[u].flag=0;
}
int build(int L,int R){
    int u=++cnt;
    if(L==R){
        tree[u]={
            0,0,L,R,
            0,a[L],
            pop1[a[L]],//pop2[a[L]]
            pon1[a[L]],//pon2[a[L]]
        };
        return u;
    }
    Lu=L; Ru=R;
    lsu=build(L,mid);
    rsu=build(mid+1,R);
    push_up(u);
    return u;
}
void modify(int l,int r,int v,int u=1){
    if(l<=Lu&&Ru<=r){
        apply(u,v);
        return;
    }
    push_down(u);
    if(l<=mid)modify(l,r,v,lsu);
    if(r>mid)modify(l,r,v,rsu);
    push_up(u);
}
int query_sum(int l,int r,int u=1){
    if(l<=Lu&&Ru<=r){
        return tree[u].sum;
    }
    push_down(u);
    int ans=0;
    if(l<=mid)ans+=query_sum(l,r,lsu);
    if(r>mid)ans+=query_sum(l,r,rsu);
    return ans;
}
hh query_hash(int l,int r,int u=1){
    if(l<=Lu&&Ru<=r){
        return tree[u].hpn;
    }
    push_down(u);
    hh ans={0ull,0ull};
    if(l<=mid)ans=ans+query_hash(l,r,lsu);
    if(r>mid)ans=ans+query_hash(l,r,rsu);
    return ans;
}

void solve(int round){
    cin>>n>>q;
    for(int i=1;i<=n;++i)cin>>a[i];
    build(1,n);
    while(q--){
        int op; cin>>op;
        if(op==1){
            int l,r,v; cin>>l>>r>>v;
            modify(l,r,v);
        }else{
            int l,r; cin>>l>>r;
            int sum=query_sum(l,r);
            if(sum*2%(r-l+1)!=0){
                cout<<"No"<<endl;
                continue;
            }
            int move=sum*2/(r-l+1);
            auto[p1,n1]=query_hash(l,r);
            if(
                (ui128)n1*qpo(b1,move)%MOD==p1
                //(ui128)n2*qpo(b2,move)%MOD==p2
            ){
                cout<<"Yes"<<endl;
            }else{
                cout<<"No"<<endl;
            }
        }
    }
    return;
}

signed main(void){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    init();
    int T=1; //cin>>T;
    for(int i=1;i<=T;++i)solve(i);
    return 0;
}
