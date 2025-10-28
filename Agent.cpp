

#include"Agent.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

bool compare_responses(string user_greet, string response, string user_input) {
    bool res = false;
    if (user_greet == user_input) {
        cout << response << endl;
        res = true;
    }
    return res;
}


string string_parser(string line, bool& res, string user_input) {
    string User_greet;
    string Agent_Response;
    bool in_response = false;

    for (int i = 0; line[i] != '\0'; i++) {
        if (!in_response) {
            if (line[i] != '#' && line[i] != '*')
                User_greet.push_back(line[i]);
            else
                in_response = true;
        }
        else {
            if (line[i] != '#' && line[i] != '*')
                Agent_Response.push_back(line[i]);
        }
    }

    if (User_greet.empty()) {
        User_greet = "-1";
    }
    res = compare_responses(User_greet, Agent_Response, user_input);
    return Agent_Response;
}


bool isNumber(const string &s) {
    if (s.empty()) return false; 

    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

string Home_file_reader(string file, string user_input) {
    ifstream out(file);
    if (!out.is_open()) {
        return "Error: Could not open Home file.\n";
    }

    string temp;
    string header;
    getline(out, header); // skip header
    string formattedResponse = "";

    if (isNumber(user_input)) {
        int count = 0;
        while (count < stoi(user_input) && getline(out, temp)) {
            count++;
        }
    }

    if (temp.empty()) return "Invalid area selection.\n";

    string field = "";
    int col = 0;
    for (int i = 0; i < temp.size(); i++) {
        if (temp[i] == '#') {
            switch (col) {
            case 0: formattedResponse += "Area  " + field + "\n"; break;
            case 1: formattedResponse += "Size  " + field + "\n"; break;
            case 2: formattedResponse += "Installments  " + field + "\n"; break;
            case 3: formattedResponse += "Price  " + field + "\n"; break;
            }
            field.clear();
            col++;
        }
        else {
            field.push_back(temp[i]);
        }
    }
    formattedResponse += "Down Payment  " + field + "\n";
    return formattedResponse;
}


string file_reader(string file, string user_input) {
    string temp;
    bool response_initiated = false;
    string default_response = ""; // store * line separately

    if (file == "Utterances.txt") {
        ifstream out(file);
        if (!out.is_open()) {
            return "Error: Could not open Utterances file.\n";
        }

        while (getline(out, temp)) {
            bool temp_res = false;
            string parsed = string_parser(temp, temp_res, user_input);

            // if this line is * line then  store this line for later use
            // this function is change to pass over the * if it occured first before it find the user greet
            if (!temp.empty() && temp[0] == '*') {
                default_response = parsed;
                continue;   
                // skip using it now to check the next 
            }

            if (temp_res) {
                response_initiated = true;
                break;
            }
        }
        // here i implement the logic  if no exact match was found print the * line
        if (!response_initiated && !default_response.empty()) {
            cout << default_response << endl;
        }
        return "";
    }
    else {
        string formatted_response = Home_file_reader(file, user_input);
        cout << formatted_response;
        return formatted_response;
    }
}

void runChatbot() {


    
    cout << "Welcome .... I am here to help you" << endl;
    string userInput;
    string last_input = "";

   while(true){
        getline(cin, userInput);

        if (userInput == "X") {
            cout << "Thanks for coming" << endl;
            break;
        }

        if (last_input == "H" && isNumber(userInput)) {
            file_reader("Home.txt", userInput);

        }
        else {
            file_reader("Utterances.txt", userInput);
        }

        last_input = userInput;
    }
}

#ifndef UNIT_TEST
int main() {
    runChatbot();
    return 0;
}
#endif
