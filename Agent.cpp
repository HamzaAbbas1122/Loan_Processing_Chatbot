#include "Agent.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// convert string to lowercase
string to_lower(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

// compare user input with greetings
bool compare_responses(string user_greet, string response, string user_input) {
    bool res = false;
    if (to_lower(user_greet) == to_lower(user_input)) {
        cout << response << endl;
        res = true;
    }
    return res;
}

// parse user greet and agent response from file line
string string_parser(string line, bool &res, string user_input) {
    string User_greet;
    string Agent_Response;
    bool in_response = false;

    for (int i = 0; line[i] != '\0'; i++) {
        if (!in_response) {
            if (line[i] != '#' && line[i] != '*')
                User_greet.push_back(line[i]);
            else
                in_response = true;
        } else {
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

// check if string is numeric
bool isNumber(const string &s) {
    if (s.empty()) return false;
    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

// helper: convert string to double safely (for price/down payment)
double to_double(string s) {
    double val = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            val = val * 10 + (s[i] - '0');
        }
    }
    return val;
}

// reads Home.txt and prints selected details + calculates monthly installment
string Home_file_reader(string file, string user_input, bool show_all = false) {
    ifstream out(file);
    if (!out.is_open()) {
        return " Could not open Home file.\n";
    }

    string temp;
    string header;
    getline(out, header); // skip header
    string formattedResponse = "";
    int line_count = 0;

    if (show_all) {
        // Show all options without down payment
        while (getline(out, temp)) {
            string field = "";
            int col = 0;
            string Area, Size, Installments, Price, DownPayment;

            for (int i = 0; i < temp.size(); i++) {
                if (temp[i] == '#') {
                    switch (col) {
                    case 0: Area = field; break;
                    case 1: Size = field; break;
                    case 2: Installments = field; break;
                    case 3: Price = field; break;
                    case 4: DownPayment=field; break;
                    }
                    field.clear();
                    col++;
                } else {
                    field.push_back(temp[i]);
                }
            }
            DownPayment = field;

            formattedResponse += to_string(++line_count) + ". Area: " + Area + "\n";
            formattedResponse += "   Size: " + Size + "\n";
            formattedResponse += "   Installments: " + Installments + "\n";
            formattedResponse += "   Price: " + Price + "\n";
            formattedResponse += "   Down Payment: " + DownPayment + "\n\n";
        }
        return formattedResponse;
    }

    // If user selected a number, show full details including down payment
    if (isNumber(user_input)) {
        int count = 0;
        while (count < stoi(user_input) && getline(out, temp)) {
            count++;
        }
    }

    if (temp.empty()) return "Invalid area selection.\n";

    string field = "";
    int col = 0;
    string Area, Size, Installments, Price, DownPayment;

    for (int i = 0; i < temp.size(); i++) {
        if (temp[i] == '#') {
            switch (col) {
            case 0: Area = field; break;
            case 1: Size = field; break;
            case 2: Installments = field; break;
            case 3: Price = field; break;
            }
            field.clear();
            col++;
        } else {
            field.push_back(temp[i]);
        }
    }
    DownPayment = field;

    formattedResponse += "Area: " + Area + "\n";
    formattedResponse += "Size: " + Size + "\n";
    formattedResponse += "Installments: " + Installments + "\n";
    formattedResponse += "Price: " + Price + "\n";
    formattedResponse += "Down Payment: " + DownPayment + "\n";

    // Monthly Installment Calculation
    double priceVal = to_double(Price);
    double downVal = to_double(DownPayment);
    int instVal = stoi(Installments);

    if (instVal > 0 && priceVal >= downVal) {
        double monthly = (priceVal - downVal) / instVal;
        formattedResponse += "Monthly Installment: " + to_string((int)monthly) + "\n";
    } else {
        formattedResponse += " Invalid data for installment calculation.\n";
    }

    return formattedResponse;
}

// reads Utterances.txt or other file
string file_reader(string file, string user_input, bool show_all = false) {
    string temp;
    bool response_initiated = false;
    string default_response = "";

    if (file == "Utterances.txt") {
        ifstream out(file);
        if (!out.is_open()) {
            return " Could not open Utterances file.\n";
        }

        while (getline(out, temp)) {
            bool temp_res = false;
            string parsed = string_parser(temp, temp_res, user_input);

            // store default (*) response
            if (!temp.empty() && temp[0] == '*') {
                default_response = parsed;
                continue;
            }

            if (temp_res) {
                response_initiated = true;
                break;
            }
        }

        if (!response_initiated && !default_response.empty()) {
            cout << default_response << endl;
        }
        return "";
    } else {
        string formatted_response = Home_file_reader(file, user_input, show_all);
        cout << formatted_response;
        return formatted_response;
    }
}

// main chatbot loop
void runChatbot() {
    cout << "Welcome .... I am here to help you" << endl;
    string userInput;
    string last_input = "";

    while (true) {
        getline(cin, userInput);
        string lower_input = to_lower(userInput);

        if (lower_input == "x") {
            cout << "Thanks for coming" << endl;
            break;
        }

        if (to_lower(last_input) == "h" && isNumber(userInput)) {
            file_reader("Home.txt", userInput,false); // show full details
        } else if (lower_input == "h") {
            file_reader("Utterances.txt", lower_input, false);
            file_reader("Home.txt", "", true); // show all options without down payment
        } else {
            file_reader("Utterances.txt", userInput,false);
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
