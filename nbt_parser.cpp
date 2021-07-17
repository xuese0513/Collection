# include <bits/stdc++.h>
using namespace std;

enum Tag{           //array's TAG(B,I,L,Other) or value's TAG
    Int = 0,
    Byte = 1,
    Short = 2,
    Long = 3,
    Float = 4,
    Double = 5,
    String = 6,
    Object = 7,
    Compound = 8,
    List = 9,
    Byte_Array = 10,
    Int_Array = 11,
    Long_Array = 12
};

void print_tag_char(const Tag &tag){
    switch(tag){
        case Byte:   cout<<'b'; break;
        case Short:  cout<<'s'; break;
        case Long:   cout<<'l'; break;
        case Float:  cout<<'f'; break;
        case Double: cout<<'d'; break;
    }
}

class NBT{
public:
    string name;            //name:value or stored value
    int size;               //amount of value
    Tag tag;                //NBT_TAG
    NBT* root;
    vector<NBT*> value;

    NBT(NBT *r){
        size = -1;
        tag = Int;
        root = r;
    }

    void clear(){
        clear(this);
    }

    void write_as_nbt(){
        print_as_json_format(this,0,0);
    }

    void write_as_json(){
        print_as_json_format(this,0,1);
    }

private:
    void print_as_json_format(NBT *cur, const int &layer, const char &format){  //format 0: nbt, 1: json
        cout<<setw(4*layer)<<" ";

        if(!cur->name.empty()){
            if(format == 0){
                cout<<cur->name;
                print_tag_char(cur->tag);
            }

            else if(format == 1){
                if(cur->tag >= 7)
                    cout<<"\""<<cur->name<<"\"";
                else
                    cout<<cur->name;
            }

            if(cur->tag >=7)
                cout<<": ";
        }

        if(cur->tag == Object){
            cout<<cur->value[0]->name;
            if(format == 0)
                print_tag_char(cur->tag);
        }
        else if(cur->tag >= 8){
            if(cur->tag == Compound)
                cout<<"{\n";
            else if(cur->tag == List)
                cout<<"[\n";

            print_as_json_format(cur->value[0],layer+1,format);
            for(int x = 1 ; x<cur->size ; x++){
                cout<<",\n";
                print_as_json_format(cur->value[x],layer+1,format);
            }

            if(cur->tag == Compound)
                cout<<'\n'<<setw(4*layer)<<" "<<"}";
            else if(cur->tag == List)
                cout<<'\n'<<setw(4*layer)<<" "<<"]";
        }
    }

    void clear(NBT *cur){
        for(int x = 0 ; x<cur->size ; x++){
            clear(cur->value[x]);
            delete cur->value[x];
        }
        //cur->value.clear();
        cur->size = 0;
    }

};

Tag get_value_tag(string &s){
    if(s[0] == '\"' || s[0] == '\'')
        return String;

    char end_ch = s[s.size()-1];
    if('0' <= end_ch && end_ch <= '9')
        return Int;

    s.pop_back();
    return (end_ch == 'b' || end_ch == 'B') ? Byte : (end_ch == 's' || end_ch == 'S') ? Short :
           (end_ch == 'l' || end_ch == 'L') ? Long : (end_ch == 'f' || end_ch == 'F') ? Float : Double;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0),cout.tie(0);

    string input;
    int x,l;
    NBT head(new NBT(nullptr));
    NBT* cur = &head;
    fstream fin;

    fin.open("nbt.txt",ios::in);
    getline(fin,input);
    freopen("output.txt","w",stdout);
    l = input.size();


    for(x = 0 ; x<l ; ++x){

        switch(input[x]){
            case ':':
                // object's tag
                while(input[++x]==' ');
                if(input[x] == '{')             // object's tag is Compound
                    cur->tag = Compound;

                else if (input[x] == '['){      // object's tag is List
                    cur->tag = (input[x+1] == 'B') ? Byte_Array : (input[x+1] == 'I') ? Int_Array : (input[x+1] == 'L') ? Long_Array : List;
                    if(cur->tag != List)
                        while(input[++x]!=';');
                }
                else{
                    cur->tag = Object;         // object's tag is Object
                    x--;
                }

                cur->value.push_back(new NBT(cur));
                cur = cur->value[++cur->size];    // object --> value (size: -1 --> 0)

                break;

            case '{':
                //current object's tag is Compound
                cur->tag = Compound;
                cur->value.push_back(new NBT(cur));
                cur = cur->value[++cur->size];    // object --> value
                break;

            case '[':
                //current object's tag is List
                cur->tag = (input[x+1] == 'B') ? Byte_Array : (input[x+1] == 'I') ? Int_Array : (input[x+1] == 'L') ? Long_Array : List;
                if(cur->tag != List)
                    while(input[++x]!=';');
                cur->value.push_back(new NBT(cur));
                cur = cur->value[++cur->size];    // object --> value
                break;

            case ',':
                if(cur->tag <= 6 && !cur->name.empty())
                    cur->tag = get_value_tag(cur->name);

                cur = cur->root;                    // value --> object
                if(cur->tag == Object){
                    cur->size++;
                    cur = cur->root;                // value(Object) --> object
                }

                cur->value.push_back(new NBT(cur));
                cur = cur->value[++cur->size];    // object --> value

                break;

            case '}':
                if(cur->tag <= 6 && !cur->name.empty())              // has value
                    cur->tag = get_value_tag(cur->name);

                cur = cur->root;                    // value --> object
                if(cur->tag == Object){
                    cur->size++;
                    cur = cur->root;                // value(Object) --> object(Compound)
                }

                // if Compound doesn't have value
                if(!cur->value[0]->tag && cur->value[0]->name.empty())  //Compound doestn't have value
                    cur->value.pop_back();
                else
                    cur->size++;
                break;

            case ']':
                if(cur->root->tag == List && cur->tag <= 6 && !cur->name.empty())
                    cur->tag = get_value_tag(cur->name);

                cur = cur->root;                // value --> object(List)

                if(!cur->value[0]->tag && cur->value[0]->name.empty())  //List doestn't have value
                    cur->value.pop_back();
                else
                    cur->size++;
                break;


            case '\"':  //String
                cur->name.push_back('\"');

                while(!(input[++x]=='\"' && input[x-1] != '\\'))
                    cur->name.push_back(input[x]);

                cur->name.push_back('\"');
            continue;
                break;

            case '\'':  //Quote_String
                cur->name.push_back('\'');

                while(!(input[++x]=='\'' && input[x-1] != '\''))
                    cur->name.push_back(input[x]);

                cur->name.push_back('\'');
                break;

            default:
                if(input[x] != ' ')
                    cur->name.push_back(input[x]);
                break;
        }
    }

    head.write_as_json();
}

