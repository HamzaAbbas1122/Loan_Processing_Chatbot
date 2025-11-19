#include "Agent.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // For setw

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
        cout << GREEN << response << RESET << endl;
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

// Function to print a horizontal border line
void print_border(int width, const string& color = YELLOW) {
    cout << color << string(width, '-') << RESET << endl;
}

// reads Home.txt and prints selected details + calculates monthly installment with styling
string Home_file_reader(string file, string user_input, bool show_all = false) {
    ifstream out(file);
    if (!out.is_open()) {
        cout << BOLD_RED << " Could not open Home file." << RESET << endl;
        return " Could not open Home file.\n";
    }
    string temp;
    string header;
    getline(out, header); // skip header
    string formattedResponse = "";
    int line_count = 0;
    if (show_all) {
        // Show all options in a fancy table
        int col_width = 20;
        print_border(6 * col_width + 5);
        cout << BOLD_YELLOW << setw(col_width) << left << "#"
             << setw(col_width) << "Area"
             << setw(col_width) << "Size"
             << setw(col_width) << "Installments"
             << setw(col_width) << "Price($)"
             << setw(col_width) << "Down Payment($)" << RESET << endl;
        print_border(6 * col_width + 5);
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
                    case 4: DownPayment = field; break;
                    }
                    field.clear();
                    col++;
                } else {
                    field.push_back(temp[i]);
                }
            }
            DownPayment = field;
            cout << CYAN << setw(col_width) << left << to_string(++line_count)
                 << setw(col_width) << Area
                 << setw(col_width) << Size
                 << setw(col_width) << Installments
                 << setw(col_width) << Price
                 << setw(col_width) << DownPayment << RESET << endl;
            formattedResponse += to_string(line_count) + ". Area: " + Area + "\n";
            formattedResponse += " Size: " + Size + "\n";
            formattedResponse += " Installments: " + Installments + "\n";
            formattedResponse += " Price: " + Price + "\n";
            formattedResponse += " Down Payment: " + DownPayment + "\n\n";
        }
        print_border(6 * col_width + 5);
        return formattedResponse;
    }
    // If user selected a number, show full details in a boxed panel
   // If user selected a number, show full details in a boxed panel
if (isNumber(user_input)) {
    int lineNumber = stoi(user_input);
    int count = 0;
    bool found = false;
    while (getline(out, temp)) {
        count++;
        if (count == lineNumber) {
            found = true;
            break; // Stop at the selected line
        }
    }

    if (!found) {
        cout << BOLD_RED << " Invalid area selection." << RESET << endl;
        return "Invalid area selection.\n";
    }
}

    if (temp.empty()) {
        cout << BOLD_RED << " Invalid area selection." << RESET << endl;
        return "Invalid area selection.\n";
    }
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
    string monthlyStr = " Invalid data for installment calculation.\n";
    if (instVal > 0 && priceVal >= downVal) {
        double monthly = (priceVal - downVal) / instVal;
        monthlyStr = to_string((int)monthly);
        formattedResponse += "Monthly Installment: " + monthlyStr + "\n";
    } else {
        formattedResponse += monthlyStr;
    }
    // Print in a fancy boxed panel
    int box_width = 50;
    print_border(box_width);
    cout << BOLD_CYAN << " Selected Home Loan Details " << RESET << endl;
    print_border(box_width);
    cout << CYAN << "Area: " << BOLD << Area << RESET << endl;
    cout << CYAN << "Size: " << BOLD << Size << RESET << endl;
    cout << CYAN << "Installments: " << BOLD << Installments << RESET << endl;
    cout << CYAN << "Price : " << BOLD << Price << RESET << endl;
    cout << CYAN << "Down Payment : " << BOLD << DownPayment << RESET << endl;
    cout << GREEN << "Monthly Installment : " << BOLD << monthlyStr << RESET << endl;
    print_border(box_width);
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
            cout << BOLD_RED << " Could not open Utterances file." << RESET << endl;
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
            cout << GREEN << default_response << RESET << endl;
        }
        return "";
    } else {
        string formatted_response = Home_file_reader(file, user_input, show_all);
        return formatted_response;
    }
}
void runChatbot() {
    // Fancy welcome banner with ASCII art and emojis
    cout << BOLD_YELLOW << R"(
 _       ____    ___    _   _     ____    ____   _______ 
| |     / __ \  / _ \  | \ | |   |  _ \  / __ \ |__   __|
| |    | |  | || |_| | |  \| |   | |_) || |  | |   | |   
| |    | |  | ||  __ | | . ` |-- |  _ < | |  | |   | |   
| |____| |__| || | | | | |\  |   | |_) || |__| |   | |   
|______|\____/ | | | | |_| \_|   |____/  \____/    |_|   
                                                          
Welcome to Loan-Bot. I am here to help you!
)" << RESET << endl;
    string userInput;
    bool greeted = false; // user greeted?
    bool applied = false; // user pressed A?
    bool homeLoanMode = false; // user pressed H?
    // Wait for Greeting
    while (!greeted) {
        cout << BOLD_CYAN << "\nPlease greet me (Hi, Hello, Salam, AoA) or press X to exit: " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);
        if (lower_input == "x") {
            cout << GREEN << "Thanks for coming" << RESET << endl;
            return;
        }
        else {
            file_reader("Utterances.txt", userInput, false);
            greeted = true;
        }
    }
    // Wait for 'A' (apply for loan)
    while (!applied) {
        cout << BOLD_CYAN << "\nPlease press A to apply for a loan or X to exit: " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);
        if (lower_input == "x") {
            cout << GREEN << "Thanks for coming" << RESET << endl;
            return;
        }
        if (lower_input == "a") {
            file_reader("Utterances.txt", userInput, false);
            applied = true;
        } else {
            cout << BOLD_RED << " Invalid input!" << RESET << endl;
        }
    }
    // Wait for loan category
    while (!homeLoanMode) {
        cout << BOLD_CYAN << "\nPlease select the loan category:\n"
             << CYAN << "H - Home Loan\nC - Car Loan\nS - Scooter Loan\nP - Personal Loan\nX - Exit" << RESET << BOLD_CYAN << "\n> " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);
        if (lower_input == "x") {
            cout << GREEN << "Thanks for coming" << RESET << endl;
            return;
        }
        if (lower_input == "h" || lower_input == "c" || lower_input == "s" || lower_input == "p") {
            file_reader("Utterances.txt", lower_input, false);
            if (lower_input == "h") {
                homeLoanMode = true;
                file_reader("Home.txt", "", true); // show all home options
            } else {
                cout << BOLD_RED << " Currently, only Home Loan details are available." << RESET << endl;
            }
        } else {
            cout << BOLD_RED << " Invalid input! Please select H, C, S, or P." << RESET << endl;
        }
    }
    // Wait for valid area number
    while (true) {
        cout << BOLD_CYAN << "\nEnter the area number you want to view (1, 2, 3, ...) or X to exit: " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);
        if (lower_input == "x") {
            cout << GREEN << "Thanks for coming" << RESET << endl;
            break;
        }
        if (isNumber(userInput)) {
            file_reader("Home.txt", userInput, false); // show selected option
        } else {
            cout << BOLD_RED << " Invalid input! Please enter a valid area number." << RESET << endl;
        }
    }
}
#ifndef UNIT_TEST
int main() {
    runChatbot();
    system("PAUSE");
    return 0;
}
#endif