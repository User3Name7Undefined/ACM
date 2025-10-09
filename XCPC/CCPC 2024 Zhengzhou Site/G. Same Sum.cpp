#include<bits/stdc++.h>
#define ull unsigned long long
#define ui128 __uint128_t
#define int long long
#define mid ((L+R)/2)
#define ls(u) tree[u].ls
#define rs(u) tree[u].rs
#define endl '\n'
using namespace std;

const int N=200005;

ull pow(const ull mod,ull x,ull k){
    ull ans=1ull,t=x;
    while(k){
        if(k&1)ans=(ui128)ans*t%mod;
        k>>=1; t=(ui128)t*t%mod;
    }
    return ans;
}

class Hash{
public:
    ull MOD,base,base_inv;
    vector<ull> powp,pown;
    int n;

    Hash(ull m,ull b,int n):
    MOD(m),base(b),n(n){
        base_inv=pow(MOD,base,MOD-2ull);
        powp.resize(n+5,1);
        pown.resize(n+5,1);
        for(int i=1;i<=n;++i){
            powp[i]=(ui128)powp[i-1]*base%MOD;
            pown[i]=(ui128)pown[i-1]*base_inv%MOD;
        }
    }
    ull operator()(int x){
        if(x>=0)return powp[x];
        else return pown[-x];
    }
    ull add(ull x1,ull x2){
        return ((ui128)x1+x2)%MOD;
    }
    ull moved(ull x,int step){
        ull multiplier=llabs(step)<=n ? (step>=0?powp[step]:pown[-step]) : pow(MOD,step>=0?base:base_inv,step);
        if(step>=0) return (ui128)x*multiplier%MOD;
        else return (ui128)x*multiplier%MOD;
    }
    void move(ull& x,int step){
        ull multiplier=llabs(step)<=n ? (step>=0?powp[step]:pown[-step]) : pow(MOD,step>=0?base:base_inv,step);
        if(step>=0) x=(ui128)x*multiplier%MOD;
        else x=(ui128)x*multiplier%MOD;
    }
}hash1((ull)1e18+3,131,N)
// ,hash2((ull)1e9+9,257,N)
;

// class phh{
// private:
//     ull h1,h2;
// public:
//     phh(ull h1=0,ull h2=0):h1(h1),h2(h2){}
//     phh(int x):h1(hash1(x)),h2(hash2(x)){}
//     phh operator+(const phh& other){
//         return phh(hash1.add(h1,other.h1),hash2.add(h2,other.h2));
//     }
//     phh moved(int step){
//         return phh(hash1.moved(h1,step),hash2.moved(h2,step));
//     }
//     bool operator==(const phh& other)const{
//         return h1==other.h1&&h2==other.h2;
//     }
//     void move(int step){
//         hash1.move(h1,step);
//         hash2.move(h2,step);
//     }
//     void out(){
//         cout<<h1<<' '<<h2<<endl;
//     }
// };

class phh{
private:
    ull h;
public:
    phh(){h=0;}
    phh(ull h):h(hash1(h)){}
    phh operator+(const phh& other){
        phh ans;
        ans.h=hash1.add(h,other.h);
        return ans;
        // return {hash1.add(h,other.h)};
    }
    phh moved(int step){
        phh ans;
        ans.h=hash1.moved(h,step);
        return ans;
    }
    bool operator==(const phh& other)const{
        return h==other.h;
    }
    void move(int step){
        hash1.move(h,step);
    }
};

class SegTree{
private:
    struct Node{
        int ls,rs,sum,flag;
        phh hashp,hashn;
    };
    int n,cnt,root;
    vector<Node>tree;

    void apply(int u,int L,int R,int v){
        tree[u].sum+=v*(R-L+1);
        tree[u].flag+=v;
        tree[u].hashp.move(v);
        tree[u].hashn.move(-v);
    }
    void push_up(int u){
        tree[u].sum=tree[ls(u)].sum+tree[rs(u)].sum;
        tree[u].hashp=tree[ls(u)].hashp+tree[rs(u)].hashp;
        tree[u].hashn=tree[ls(u)].hashn+tree[rs(u)].hashn;
    }
    void push_down(int u,int L,int R){
        if(!tree[u].flag)return;
        apply(ls(u),L,mid,tree[u].flag);
        apply(rs(u),mid+1,R,tree[u].flag);
        tree[u].flag=0;
    }
    int build(int L,int R,const vector<int>&arr){
        int u=++cnt;
        if(L==R){
            tree[u]={0,0,arr[L],0,phh(arr[L]),phh(-arr[R])};
            return u;
        }
        ls(u)=build(L,mid,arr);
        rs(u)=build(mid+1,R,arr);
        push_up(u);
        return u;
    }
    void modify(int u,int L,int R,int l,int r,int v){
        if(L>=l&&R<=r){
            apply(u,L,R,v);
            return;
        }
        push_down(u,L,R);
        if(l<=mid)modify(ls(u),L,mid,l,r,v);
        if(r>mid)modify(rs(u),mid+1,R,l,r,v);
        push_up(u);
    }
    int query_sum(int u,int L,int R,int l,int r){
        if(L>=l&&R<=r){
            return tree[u].sum;
        }
        push_down(u,L,R);
        int ans=0;
        if(l<=mid)ans+=query_sum(ls(u),L,mid,l,r);
        if(r>mid)ans+=query_sum(rs(u),mid+1,R,l,r);
        return ans;
    }
    phh query_hash(int u,int L,int R,int l,int r,bool sig){
        if(L>=l&&R<=r){
            if(sig)return tree[u].hashp;
            else return tree[u].hashn;
        }
        push_down(u,L,R);
        //phh ans={0,0};
        phh ans=0;
        if(l<=mid)ans=ans+query_hash(ls(u),L,mid,l,r,sig);
        if(r>mid)ans=ans+query_hash(rs(u),mid+1,R,l,r,sig);
        return ans;
    }
    
public:
    SegTree(int n,const vector<int>&arr):n(n){
        cnt=0;
        tree.resize(2*n+5);
        root=build(1,n,arr);
    }
    void modify(int l,int r,int v){
        modify(root,1,n,l,r,v);
    }
    int query_sum(int l,int r){
        return query_sum(root,1,n,l,r);
    }
    phh query_hash(int l,int r,bool sig){
        return query_hash(root,1,n,l,r,sig);
    }
    bool judge(int l,int r){
        int sum=query_sum(l,r);
        if(sum*2%(r-l+1)!=0)return false;
        int step=sum*2/(r-l+1);
        phh aa=query_hash(l,r,false);
        aa=aa.moved(step);
        phh bb=query_hash(l,r,true);
        if(aa==bb){
            return true;
        }else{
            return false;
        }
    }
};

int n,q;
vector<int>arr;

void solve(int round){
    cin>>n>>q;
    arr.resize(n+5,0);
    for(int i=1;i<=n;++i)cin>>arr[i];
    SegTree t(n,arr);
    // auto out=[&](){
    //     for(int i=1;i<=n;++i){
    //         cout<<i<<endl;
    //         t.query_hash(i,i,true).out();
    //         t.query_hash(i,i,false).out();
    //     }
    // };
    // out();
    // cout<<t.query_sum(1,8)<<endl;
    // t.modify(1,4,4);
    // out();
    // ull x=hash1.pown[4];
    // cout<<"x: "<<x<<endl;
    // hash1.move(x,-4);
    // cout<<"move x -4 and x is: "<<x<<endl;
    // cout<<"x should be: "<<hash1.pown[8]<<endl;
    // cout<<t.query_sum(1,8)<<endl;

    while(q--){
        int op; cin>>op;
        if(op==1){
            int l,r,v; cin>>l>>r>>v;
            t.modify(l,r,v);
            // cout<<t.query_sum(1,8)<<endl;
        }else{
            int l,r; cin>>l>>r;
            if(t.judge(l,r)){
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
    int T=1; //cin>>T;
    for(int i=1;i<=T;++i)solve(i);
    return 0;
}
