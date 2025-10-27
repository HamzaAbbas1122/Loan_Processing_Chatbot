

#include"Agent.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

bool compare_responses(string user_greet, string response, string user_input){
    cout<<" User greets in compare: "<<user_greet<<endl;

   bool res=false;
    if(user_greet==user_input || user_greet=="-1"){
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
    if (s.empty()) return false; 

    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}



string file_reader(string file, string user_input){
    ifstream out(file);
    if(out.is_open()){
        cout<<"file is opened"<<endl;
    }

    string temp;
    bool response_initiated=false;
    string response;
    string default_response = ""; // store * line separately

    if(file=="Utterances.txt"){
        while(getline(out,temp)){
            cout<<"temp: "<<temp<<endl;
            bool temp_res = false;
            string parsed = string_parser(temp,temp_res,user_input);

            // if this line is * line then  store this lin for later use
            // this function is chnage to pass over the * if it occured first before it find the user greet
        
            if (temp.size() > 0 && temp[0] == '*') {
                default_response = parsed;
                continue; 
                // skip using it now to check the next 
            }

            if(temp_res==true){
                cout<<"i am breaking "<<endl;
                response_initiated = true;
                break;
            }
        }

        // here i implement the logic  if no exact match was found print the * line
        if(!response_initiated && default_response != ""){
            cout<<default_response<<endl;
        }
        return response;
    }
   else {
    int count = 0;
    string header;
    getline(out, header); 
    // skip the first line (column names)
    string formattedResponse = "";

    if (isNumber(user_input)) {
        while (count < stoi(user_input)) {
            getline(out, temp);
            count++;
        }
    }

    string field = "";
    int col = 0;
    for (int i = 0; i < temp.size(); i++) {
        if (temp[i] == '#') {
            switch (col) {
                case 0: formattedResponse += "Area  " + field + "\n";
                 break;
                case 1: formattedResponse += "Size  " + field + "\n";
                 break;
                case 2: formattedResponse += "Installments  " + field + "\n";
                 break;
                case 3: formattedResponse += "Price  " + field + "\n";
                 break;
            }
            field = "";
            col++;
        } else {
            field.push_back(temp[i]);
        }
    }
    formattedResponse += "Down Payment  " + field + "\n";

    cout << formattedResponse; /// printing here and also run it it will be need in the test or git action for automation testing 
    return formattedResponse; 
}


}


void runChatbot() {
     cout << "Welcome .... I am here to help you" << endl;
      string userInput;

      string last_input = "";


    while (true) {

        getline(cin, userInput);

        if (userInput == "X") {

            cout << "Thanks for coming" << endl;

            break;
        }

        if (last_input == "H" && isNumber(userInput)) {
             file_reader("Home.txt", userInput);
        } else {
             file_reader("Utterances.txt", userInput);
        }
        last_input = userInput;
    }
}

#ifndef UNIT_TEST
int main(){
    
runChatbot();
return 0;

}
#endif