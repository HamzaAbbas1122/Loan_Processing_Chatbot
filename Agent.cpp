#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void compare_responses(string user_greet,string response, bool& res){

    if(user_greet==user_greet){
        cout<<response<<endl;
        res=true;
    }
    else{
        res=false;
    }


}





void string_parser(string line,bool& res){
    
    string User_greet;
    string Agent_Response;
    bool in_response=false;

    for(int i=0;i<line.length();i++){
        if(line[i]!='#'||line[i]!='*' && in_response==false){
            User_greet[i]+=line[i];
        }
        if(line[i]=='#'||line[i]=='*'){
            in_response=true;
        }
        if(in_response==true){
            Agent_Response[i]+=line[i];
        }
    }

   
    compare_responses(User_greet,Agent_Response,res);
}


string file_reader(string file){
    ifstream out(file);

    string temp;
    bool response_initiated=false;
    while(getline(out,temp)){
        string_parser(temp,response_initiated);
        if(response_initiated==true){
            break;
        }
    }

}



int main(){
    cout<<"Hello WOrld"<<endl;
}