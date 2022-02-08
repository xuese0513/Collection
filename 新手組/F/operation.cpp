# include <bits/stdc++.h>
using namespace std;

int main(){
    int a, b;
    char c;
    while(cin>>a>>b>>c){
        if(c == '+')
            cout<<(a+b)<<'\n';
        else if(c == '-')
            cout<<(a-b)<<'\n';
        else if(c == '*')
            cout<<(a*b)<<'\n';
        else if(c == '/'){
            if(b == 0)
                cout<<"INF\n";
            else
                cout<<(a/b)<<'\n';
        }
        else if(c == '%'){
            if(b == 0)
                cout<<"INF\n";
            else
                cout<<((a+b)%b)<<'\n';
        }
    }
}
