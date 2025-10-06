#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

const int N=25;

struct Node{
    int x,y,z;
};
Node delta_near[]={
    {1,0,0},
    {-1,0,0},
    {0,1,0},
    {0,-1,0},
    {0,0,1},
    {0,0,-1},
};
static const Node delta_dir[]={
    {-1,0},
    {-1,-1},
    {0,-1},
    {1,-1},
    {1,0},
    {1,1},
    {0,1},
    {-1,1},
};

int n,m;
int cold[N][N];
bool block[N][N][N];
int Hm;
int Hr,Hc,Hx,Hy;
int Hroof;

bool INSIDE(int x,int y,int z=0){
    return 0<=x&&x<n&&0<=y&&y<n&&0<=z&&z<=Hm;
}
int dis(int x1,int y1,int x2,int y2){
    return max(abs(x2-x1),abs(y2-y1));
}

void ICE_BARRAGE(int R,int C,int D,int S){
    
    int iced=0;
    int x=R,y=C;
    do{
        if(!INSIDE(x,y))break;
        if(block[x][y][0])break;
        if(cold[x][y]<4){
            cold[x][y]++;
            iced++;
        }
        x+=delta_dir[D].x; y+=delta_dir[D].y;
    }while(dis(x,y,R,C)<=S);
    cout<<"CIRNO FREEZED "<<iced<<" BLOCK(S)"<<endl;
}

int blocks_tot=0;
void MAKE_ICE_BLOCK(void){
    int get_blocks=0;
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            if(cold[i][j]==4){
                get_blocks++;
                cold[i][j]=0;
            }
        }
    }
    blocks_tot+=get_blocks;
    cout<<"CIRNO MADE "<<get_blocks<<" ICE BLOCK(S),";
    cout<<"NOW SHE HAS "<<blocks_tot<<" ICE BLOCK(S)"<<endl;
}

vector<Node> GET_NEAR(int x,int y,int z){
    vector<Node>ret;
    for(int i=0;i<6;++i){
        int near_x=x+delta_near[i].x;
        int near_y=y+delta_near[i].y;
        int near_z=z+delta_near[i].z;
        if(!INSIDE(near_x,near_y,near_z))continue;
        ret.push_back({near_x,near_y,near_z});
    }
    return ret;
}
void PUT_ICE_BLOCK(int R,int C,int H){
    auto IN_THE_AIR=[&](int x,int y,int z){
        if(z==0)return false;//on the ground
        vector<Node>near=GET_NEAR(x,y,z);
        for(auto [x,y,z]:near){
            if(!INSIDE(x,y,z))continue;
            if(block[x][y][z])return false;
        }
        return true;
    };
    
    if(blocks_tot==0){
        cout<<"CIRNO HAS NO ICE_BLOCK"<<endl;
        return;
    }
    if(block[R][C][H]||IN_THE_AIR(R,C,H)){
        cout<<"BAKA CIRNO,CAN'T PUT HERE"<<endl;
        return;
    }

    blocks_tot--;
    block[R][C][H]=true;
    if(H==0)cold[R][C]=0;

    if(R<Hr||R>Hr+Hx-1||C<Hc||C>Hc+Hy-1){
        cout<<"CIRNO MISSED THE PLACE"<<endl;
        return;
    }
    if(Hr+1<=R&&R<=Hr+Hx-2&&Hc+1<=C&&C<=Hc+Hy-2){
        cout<<"CIRNO PUT AN ICE_BLOCK INSIDE THE HOUSE"<<endl;
        return;
    }
    cout<<"CIRNO SUCCESSFULLY PUT AN ICE_BLOCK,";
    cout<<"NOW SHE HAS "<<blocks_tot<<" ICE_BLOCK(S)"<<endl;
}

pair<vector<Node>,bool>GET_CC(int x,int y,int z){
    static bool flag[N][N][N]={};
    static vector<Node>cc;
    static queue<Node>q;
    bool grounded=false;

    for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<=Hm;++k){
        flag[i][j][k]=false;
    }
    cc.clear();

    q.push({x,y,z}); flag[x][y][z]=true;
    while(!q.empty()){
        auto[x,y,z]=q.front(); q.pop();
        cc.push_back({x,y,z});
        grounded|=(z==0);
        vector<Node>near=GET_NEAR(x,y,z);
        for(auto[x,y,z]:near){
            if(!INSIDE(x,y,z))continue;
            if(!block[x][y][z])continue;
            if(flag[x][y][z])continue;
            flag[x][y][z]=true;
            q.push({x,y,z});
        }
    }

    return {cc,grounded};
}
int UPDATE_BLOCK(){
    static bool flag[N][N][N]={};
    int blocks_broken=0;

    for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<=Hm;++k){
        flag[i][j][k]=false;
    }

    for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<=Hm;++k){
        if(flag[i][j][k])continue;
        if(!block[i][j][k])continue;
        auto[blocks,grounded]=GET_CC(i,j,k);
        for(auto[x,y,z]:blocks){
            flag[x][y][z]=true;
            if(!grounded){
                block[x][y][z]=false;
                blocks_broken++;
            }
        }
    }
    return blocks_broken;
}
void REMOVE_ICE_BLOCK(int R,int C,int H){
    int blocks_broken=0;
    if(!block[R][C][H]){
        cout<<"BAKA CIRNO,THERE IS NO ICE_BLOCK"<<endl;
        return;
    }
    block[R][C][H]=false;
    blocks_tot++;
    blocks_broken=UPDATE_BLOCK();
    if(blocks_broken==0){
        cout<<"CIRNO REMOVED AN ICE_BLOCK"<<endl;
    }else{
        cout<<"CIRNO REMOVED AN ICE_BLOCK,";
        cout<<"AND "<<blocks_broken<<" BLOCK(S) ARE BROKEN"<<endl;
    }
}

bool NUM_CHECK(int H){
    int blocks_needed=0;
    for(int i=Hc;i<=Hr+Hx-1;++i)for(int j=Hr;j<=Hc+Hy-1;++j){
        if(!block[i][j][H])++blocks_needed;
    }
    return blocks_needed<=blocks_tot;
}
bool INSIDE_HOUSE(int x,int y,int z){
    return x>Hr&&x<Hr+Hx-1&&y>Hc&&y<Hc+Hy-1&&z>=0&&z<Hroof;
}
bool OUTSIDE_HOUSE(int x,int y,int z){
    return x<Hr||x>Hr+Hx-1||y<Hc||y>Hc+Hy-1||z>Hroof;
}
bool ON_ROOF(int x,int y,int z){
    return z==Hroof&&x>=Hr&&x<=Hr+Hx-1&&y>=Hc&&y<=Hc+Hy-1;
}
bool ON_WALL(int x,int y,int z){
    return !INSIDE_HOUSE(x,y,z)&&!OUTSIDE_HOUSE(x,y,z)&&!ON_ROOF(x,y,z);
}
bool need_remove=false;
bool REMOVE_OTHER_BLOCKS(){
    static bool flag[N][N][N]={};
    for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<=Hm;++k){
        flag[i][j][k]=false;
    }

    //count blocks outside/outside
    int blocks_inside=0;
    int blocks_outside=0;
    for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<=Hm;++k){
        //remove the blocks and recycle
        if(INSIDE_HOUSE(i,j,k)&&block[i][j][k]){
            block[i][j][k]=false;
            ++blocks_inside;
            ++blocks_tot;
        }else if(OUTSIDE_HOUSE(i,j,k)&&block[i][j][k]){
            block[i][j][k]=false;
            ++blocks_outside;
            ++blocks_tot;
        }
    }
    if(blocks_inside||blocks_outside)need_remove=true;
    cout<<blocks_inside<<" ICE_BLOCK(S) INSIDE THE HOUSE NEED TO BE REMOVED"<<endl;
    cout<<blocks_outside<<" ICE_BLOCK(S) OUTSIDE THE HOUSE NEED TO BE REMOVED"<<endl;

    for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<=Hm;++k){
        if(flag[i][j][k])continue;
        if(!block[i][j][k])continue;
        auto[blocks,grounded]=GET_CC(i,j,k);
        for(auto[x,y,z]:blocks){
            flag[x][y][z]=true;
            if(!grounded){
                block[x][y][z]=false;
                if(ON_WALL(x,y,z)){
                    ++blocks_tot;
                }
            }
        }
    }
    
    //check roof
    if(!block[Hr][Hc][Hroof]){
        return false;
    }else{
        return true;
    }
}
int REPAIR_THE_WALL(int x,int y,bool repair_mode=false){
    auto near=[&](int x1,int y1,int x2,int y2){
        if(!INSIDE(x1,y1)||!INSIDE(x2,y2))return false;
        if(x1==x2&&abs(y1-y2)==1)return true;
        if(y1==y2&&abs(x1-x2)==1)return true;
        return false;
    };

    int blocks_need=0;
    for(int k=0;k<2;++k){//above the door
        //check wall
        auto check_wall=[&](int xx,int yy){
            if(!block[xx][yy][k]){
                ++blocks_need;
                if(repair_mode)block[xx][yy][k]=true;
            }
        };
        for(int i=Hr+1;i<=Hr+Hx-2;++i){//left right
            if(i!=x||Hc!=y)check_wall(i,Hc);
            if(i!=x||Hc+Hy-1!=y)check_wall(i,Hc+Hy-1);
        }
        for(int j=Hc+1;j<=Hc+Hy-2;++j){//up down
            if(Hr!=x||j!=y)check_wall(Hr,j);
            if(Hr+Hx-1!=x||j!=y)check_wall(Hr+Hx-1,j);
        }


        //check pillar
        auto check_pillar=[&](int xx,int yy){
            if(near(x,y,xx,yy)&&!block[xx][yy][k]&&!block[x][y][k]){
                ++blocks_need;
                if(repair_mode)block[xx][yy][k]=true;
            }
        };
        check_pillar(Hr,Hc);
        check_pillar(Hr+Hx-1,Hc);
        check_pillar(Hr,Hc+Hy-1);
        check_pillar(Hr+Hx-1,Hc+Hy-1);
    }
    return blocks_need;
}

struct Door{
    int x,y;
    int blocks_need,blanks,mid;
    Door(int x,int y,int blocks_need,int blanks,int mid):
        x(x),y(y),blocks_need(blocks_need),blanks(blanks),mid(mid){}
    bool operator<(const Door d)const{
        if(blocks_need!=d.blocks_need)return blocks_need<d.blocks_need;
        if(blanks!=d.blanks)return blanks>d.blanks;
        if(mid!=d.mid)return mid>d.mid;
        return false;
    }
};

void out(){
    for(int i=0;i<=Hroof;++i){for(int j=0;j<n;++j){for(int k=0;k<n;++k){
        if(block[j][k][i])cout<<"0";
        else cout<<".";
    }cout<<endl;}cout<<endl;}
    cout<<blocks_tot<<endl;
}

void MAKE_ROOF(void){
    Hroof=0;
    for(int k=Hm-1;k>=0;--k){
        for(int i=Hr;i<=Hr+Hx-1;++i){
            if(Hroof)break;
            if(block[i][Hc][k]||block[i][Hc+Hy-1][k]){
                Hroof=k+1;
            }
        }
        for(int j=Hc;j<=Hc+Hy-1;++j){
            if(Hroof)break;
            if(block[Hr][j][k]||block[Hr+Hx-1][j][k]){
                Hroof=k+1;
            }
        }
    }
    // out();

    if(!NUM_CHECK(Hroof)){
        cout<<"SORRY CIRNO,NOT ENOUGH ICE_BLOCK(S) TO MAKE ROOF"<<endl;
        return;
    }
    if(Hroof<2||Hx<=2||Hy<=2){
        cout<<"SORRY CIRNO,HOUSE IS TOO SMALL"<<endl;
        return;
    }

    //make roof
    for(int i=Hr;i<=Hr+Hx-1;++i)for(int j=Hc;j<=Hc+Hy-1;++j){
        if(!block[i][j][Hroof]){
            --blocks_tot;
            block[i][j][Hroof]=true;
        }
    }
    // out();

    if(!REMOVE_OTHER_BLOCKS()){
        cout<<"SORRY CIRNO,HOUSE IS BROKEN WHEN REMOVING BLOCKS"<<endl;
        return;
    }
    // out();

    int blocks_need=0;
    for(int k=2;k<Hroof;++k){//above the door
        for(int i=Hr+1;i<=Hr+Hx-2;++i){//left right
            if(!block[i][Hc][k])++blocks_need;
            if(!block[i][Hc+Hy-1][k])++blocks_need;
        }
        for(int j=Hc+1;j<=Hc+Hy-2;++j){//up down
            if(!block[Hr][j][k])++blocks_need;
            if(!block[Hr+Hx-1][j][k])++blocks_need;
        }
    }
    //choose the door
    vector<Door>doors;
    auto doors_push=[&](int x,int y,int md){
        doors.push_back(Door(
            x,y,
            blocks_need+REPAIR_THE_WALL(x,y),
            2-block[x][y][0]-block[x][y][1],
            md
        ));
    };
    for(int i=Hr+1;i<=Hr+Hx-2;++i){//left right
        int mid=abs((i-Hr)-(Hr+Hx-1-i))<=1;
        doors_push(i,Hc,mid);
        doors_push(i,Hc+Hy-1,mid);
    }
    for(int j=Hc+1;j<=Hc+Hy-2;++j){//up down
        int mid=abs((j-Hc)-(Hc+Hy-1-j))<=1;
        doors_push(Hr,j,mid);
        doors_push(Hr+Hx-1,j,mid);
    }
    sort(doors.begin(),doors.end());
    Door door=doors.front();
    
    if(door.blocks_need>blocks_tot){
        cout<<"SORRY CIRNO,NOT ENOUGH ICE_BLOCKS TO FIX THE WALL"<<endl;
        return;
    }
    cout<<"GOOD JOB CIRNO,SUCCESSFULLY BUILT THE HOUSE"<<endl;

    if(door.blanks<2){
        cout<<"HOUSE HAS NO DOOR"<<endl;
    }else{
        cout<<"DOOR IS OK"<<endl;
    }

    if(door.blocks_need==0){
        cout<<"WALL IS OK"<<endl;
    }else{
        cout<<"WALL NEED TO BE FIXED"<<endl;
    }

    // out();
    //truly repair the wall
    for(int k=2;k<Hroof;++k){//above the door
        for(int i=Hr+1;i<=Hr+Hx-2;++i){//left right
            block[i][Hc][k]=true;
            block[i][Hc+Hy-1][k]=true;
        }
        for(int j=Hc+1;j<=Hc+Hy-2;++j){//up down
            block[Hr][j][k]=true;
            block[Hr+Hx-1][j][k]=true;
        }
    }
    blocks_need+=REPAIR_THE_WALL(door.x,door.y,true);
    blocks_tot-=blocks_need;
    // out();

    bool pillar_need_repair=false;
    for(int h=0;h<=Hroof;++h){
        auto repair=[&](int x,int y,int z){
            if(!block[x][y][z]){
                pillar_need_repair=true;
                blocks_tot--;
            }
        };
        repair(Hr,Hc,h);
        repair(Hr+Hx-1,Hc,h);
        repair(Hr,Hc+Hy-1,h);
        repair(Hr+Hx-1,Hc+Hy-1,h);
    }
    blocks_tot=max(0,blocks_tot);

    if(pillar_need_repair){
        cout<<"CORNER NEED TO BE FIXED"<<endl;
    }else{
        cout<<"CORNER IS OK"<<endl;
    }

    //blocks on door can be recycled???wtf
    cout<<"CIRNO FINALLY HAS "<<blocks_tot+2-door.blanks<<" ICE_BLOCK(S)"<<endl;

    if(blocks_need==0&&door.blanks==2&&!pillar_need_repair&&door.mid&&!need_remove){
        cout<<"CIRNO IS PERFECT!"<<endl;
    }
}

signed main(void){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    cin>>n;
    cin>>Hm;
    cin>>Hr>>Hc>>Hx>>Hy;
    cin>>m;
    while(m--){
        string op;
        cin>>op;
        if(op=="ICE_BARRAGE"){
            int R,C,D,S;
            cin>>R>>C>>D>>S;
            ICE_BARRAGE(R,C,D,S);
        }else if(op=="MAKE_ICE_BLOCK"){
            MAKE_ICE_BLOCK();
        }else if(op=="PUT_ICE_BLOCK"){
            int R,C,H;
            cin>>R>>C>>H;
            PUT_ICE_BLOCK(R,C,H);
        }else if(op=="REMOVE_ICE_BLOCK"){
            int R,C,H;
            cin>>R>>C>>H;
            REMOVE_ICE_BLOCK(R,C,H);
        }else if(op=="MAKE_ROOF"){
            MAKE_ROOF();
        }
    }
    return 0;
}
