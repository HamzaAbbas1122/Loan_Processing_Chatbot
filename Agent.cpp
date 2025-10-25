

#include"Agent.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

bool compare_responses(string user_greet,string response,string user_input){
    cout<<" user greet in comapre: "<<user_greet<<endl;

   bool res=false;
    if(user_greet==user_input){
        cout<<"displaying response"<<endl;
        cout<<response<<endl;
        res=true;
    }
    else{
        res=false;
    }
    return res;

}





string string_parser(string line,bool& res,string user_input){
    
    string User_greet;
    string Agent_Response;
    bool in_response=false;
    cout<<"inline: "<<line<<endl;
    for(int i=0;line[i]!='\0';i++){

    if(in_response!=true){
        cout<<" line {i}: "<<line[i]<<endl;
            if(line[i]!='#'&&line[i]!='*'){
            User_greet.push_back(line[i]);
            }
            else{
                in_response=true;
         }
    }
    else{
             if(line[i]!='#'&&line[i]!='*')
            Agent_Response.push_back(line[i]);
        
    }
    
}
cout<<"user: "<<User_greet<<endl;
    cout<<"agent: "<<Agent_Response<<endl;

if(User_greet==""){
    User_greet="-1";
}
   res= compare_responses(User_greet,Agent_Response,user_input);
   return Agent_Response;
}



bool isNumber(const string &s) {
    for (char c : s)
        if (!isdigit(c))
            return false;
    return !s.empty();
}

string file_reader(string file,string user_input){
    ifstream out(file);
    if(out.is_open()){
        cout<<"file is opedne"<<endl;
    }

    string temp;
    bool response_initiated=false;
    string response;

    if(file=="Utterances.txt")
    while(getline(out,temp)){
        cout<<"temp: "<<temp<<endl;
        response=string_parser(temp,response_initiated,user_input);
        if(response_initiated==true){
            cout<<"iam breaking"<<endl;
            break;
        }
    }
    else{
        int count= 0;
         getline(out,temp);
         for(int i=0;temp[i]!='\0';i++){
            if(temp[i]=='#'){
                temp[i]=':';
            }
         }
         cout<<temp<<endl;
         if (isNumber(user_input)) {
    while (count < stoi(user_input) - 1) {
        getline(out, temp);
        count++;
    }
}

         getline(out,temp);
         for(int i=0;temp[i]!='\0';i++){
            if(temp[i]=='#'){
                temp[i]=':';
            }
         }
         response=temp;


    }
    return response;
}



void runChatbot() {
    cout << "Welcome .... I am here to help you" << endl;
    string usergreet;
    cin >> usergreet;
    file_reader("Utterances.txt", usergreet);
    string ch;
    cin >> ch;
    if (ch == "A") {
        file_reader("Utterances.txt", ch);
        string ch1;
        cin >> ch1;
        if (ch1 == "H") {
            file_reader("Utterances.txt", ch1);
            string ch;
            cin >> ch;
            file_reader("Home.txt", ch);
        }
    }
    if (ch == "X") {
        cout << "Thanks for coming" << endl;
    }
}


#ifndef UNIT_TEST
int main(){
    
runChatbot();
return 0;

}
#endif