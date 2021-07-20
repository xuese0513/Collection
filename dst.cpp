#include <bits/stdc++.h>
#include <windows.h>
#include <direct.h>
using namespace std;

typedef unsigned long long int ull;
struct DST{        //DST = Dynamic Segment Tree
    double cost;
    int edge;
    int part;
    int partn;

    void write(const double &c, const int &e, const int &p, const int &pn){
        cost = c;
        edge = e;
        part = p;
        partn = pn;
    }
};

int w = 4;
int Base;        //Base of DST

map<ull,DST> dp;
int record[50];
long long int amount;

void print_node_info(const ull &i);
void print_dst(ull n);
double build(const ull &n);

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0),cout.tie(0);
    ull n,N,L,x,p,pn;
    double cost;

    //cin>>N;
    N = 1e+8;

    Base = 1;
    size_t timer = clock();


    //DST base
    do{
        Base++;
        dp[Base].write(cost = Base,1,1,1);

        for(x = 2 ; x<Base ; x++){
            p = Base/x;
            pn = (x-Base%x)*p;
            cost = min(cost,w + x + (double)(pn*p + (Base-pn)*(p+1))/(double)Base);
        }
    }while(cost>=Base);


    for(n = 0 ; n<Base ; n++)
        dp[n].write(n,1,1,1);

    build(N);


    cout<<"\n";
    double ave = 0;
    int amrec = 0;

    print_dst(N);


    for(n = 1 ; n<Base ; n++){
        ave += record[n]*n;
        amrec += record[n];
    }
}

void print_node_info(const ull &i){
    cout<<" ["<<right<<setw(20)<<i<<"]"<<" | ";
    cout<<"time-costed = "<<left<<setw(8)<<fixed<<setprecision(4)<<dp[i].cost<<" | ";
    cout<<"edges = "<<left<<setw(2)<<dp[i].edge<<" |\n";
}

void print_dst(ull n){
    cout<<"weight : "<<w<<", Base : "<<Base<<"\n\n";

    while(n>Base){
        print_node_info(n);

        n = n/dp[n].edge + (bool)(n%dp[n].edge);
    }
    print_node_info(n);
}


double build(const ull &n){
    if(!dp[n].edge){
        ull p,pn,k; // part, amount of parts, k-parts
        double cost;
        dp[n].write(n,1,1,1);

        for(k = 2 ; k<Base ; k++){
            p = n/k;
            pn = (k-n%k);
            cost = w + k + (build(p)*pn*p + build(p+1)*(n-pn*p))/n;
            if(cost < dp[n].cost)
                dp[n].write(cost,k,p,pn);
        }

        record[dp[n].edge]++;
    }
    return dp[n].cost;
}
