#include<bits/stdc++.h>
#define PII pair<int,int>
#define endl '\n'
using namespace std;

const int N=100005;
int a,b,x,y;

vector<pair<PII,PII>>ans;
bool exist(int x,int y){
    if(x==0&&y==0)return true;
    if(x==0&&y==b)return true;
    if(x==a&&y==0)return true;
    if(x==a&&y==b)return true;
    return false;
}

void solve(int round){
    cin>>a>>b>>x>>y;
    if(a==0&&x!=0||b==0&&y!=0){
        cout<<-1<<endl;
        return;
    }

    int mna=a==0?1:a/(a&-a);
    int mnb=b==0?1:b/(b&-b);
    if(x%mna!=0||y%mnb!=0){
        cout<<-1<<endl;
        return;
    }
    x/=mna; a/=mna;
    y/=mnb; b/=mnb;
    //choose a corner everytime
    //push the node
    while(!exist(x,y)){
        int sx=x<=a/2?0:a;
        int sy=y<=b/2?0:b;
        ans.push_back({
            {sx,sy},
            {2*x-sx,2*y-sy},
        });
        x=2*x-sx;
        y=2*y-sy;
    }
    cout<<ans.size()<<endl;
    reverse(ans.begin(),ans.end());
    for(auto dots:ans){
        cout<<dots.first.first*mna<<' ';
        cout<<dots.first.second*mnb<<' ';
        cout<<dots.second.first*mna<<' ';
        cout<<dots.second.second*mnb<<endl;
    }
    return;
}

signed main(void){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int T=1; //cin>>T;
    for(int i=1;i<=T;++i)solve(i);
    return 0;
}
