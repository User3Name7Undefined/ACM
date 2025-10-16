#include<bits/stdc++.h>
#define int long long
#define endl '\n'

using namespace std;

const int N=5e3+5;

int n;
bitset<N>e[N];

bitset<N>sv;
bitset<N>se[N];

bitset<N>tv;
bitset<N>te[N];

void solve(int round){
    cin>>n;
    for(int i=1;i<=n;++i){for(int j=1;j<=n;++j){
        char ch; cin>>ch;
        e[i][j]=(ch=='1');
    }}
    int a=0,mxout=-1;
    for(int i=1;i<=n;++i){
        int iout=e[i].count();
        if(iout>mxout){
            mxout=iout;
            a=i;
        }
    }
    if(mxout==n-1){
        cout<<"NOT FOUND"<<endl;
        return;
    }
    cout<<a<<' ';

    for(int i=1;i<=n;++i){
        if(e[i][a])sv[i]=true;
        // else if(e[a][i])tv[i]=true;
    }

    for(int i=1;i<=n;++i){for(int j=1;j<=n;++j){
        if(sv[i]&&sv[j])se[i][j]=e[i][j];
        // if(tv[i]&&tv[j])te[i][j]=e[i][j];
    }}

    int b=0; mxout=-1;
    int tots=sv.count();
    for(int i=1;i<=n;++i){
        if(!sv[i])continue;
        int iout=se[i].count();
        if(iout>mxout){
            mxout=iout;
            b=i;
        }
    }
    cout<<b<<' ';
    if(mxout!=tots-1){
        for(int i=1;i<=n;++i){if(!sv[i])continue;
            if(e[i][b])tv[i]=true;
        }

        for(int i=1;i<=n;++i){for(int j=1;j<=n;++j){
            if(tv[i]&&tv[j])te[i][j]=e[i][j];
        }}

        int c=0; mxout=-1;
        int tott=tv.count();
        for(int i=1;i<=n;++i){
            if(!tv[i])continue;
            int iout=te[i].count();
            if(iout>mxout){
                mxout=iout;
                c=i;
            }
        }
        cout<<c<<endl;
        return;
    }else{
        for(int i=1;i<=n;++i){
            if(e[a][i])tv[i]=true;
        }

        for(int i=1;i<=n;++i){
            sv[i]=false;
            if(!tv[i])continue;
            if(e[i][b])sv[i]=true;
        }

        for(int i=1;i<=n;++i){if(!sv[i])continue;
            se[i].reset();
            for(int j=1;j<=n;++j){if(!sv[j])continue;
                se[i][j]=e[i][j];
        }}

        int c=0; mxout=-1;
        int tots=sv.count();
        for(int i=1;i<=n;++i){
            if(!sv[i])continue;
            int iout=se[i].count();
            if(iout>mxout){
                mxout=iout;
                c=i;
            }
        }
        cout<<c<<endl;
        return;
    }
}

signed main(void){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int T=1; //cin>>T;
    for(int i=1;i<=T;++i)solve(i);
    return 0;
}
