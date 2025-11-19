#include "Agent.h" 
#include <algorithm> 
#include <cstdlib> 
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Assuming color constants (RESET, BOLD_RED, CYAN, etc.), CHECK_MARK, CROSS_MARK
// and LoanApplication class are defined in Agent.h

// --- Function Prototypes for new details readers (Needed inside Agent.cpp) ---
string Car_file_reader_details(string file, string user_input);
string Scooter_file_reader_details(string file, string user_input);

// --- Helper Functions (Existing) ---

// convert string to lowercase
string to_lower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
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
    for (char const &c : s) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

// helper: convert string to double safely (FIXED: Handles commas used in provided data)
double to_double(string s) {
    s.erase(remove(s.begin(), s.end(), ','), s.end());
    
    double val = 0;
    try {
        val = stod(s);
    } catch (...) {
        return 0;
    }
    return val;
}

// Function to print a horizontal border line
void print_border(int width, const string& color) {
    cout << color << string(width, '-') << RESET << endl;
}

// Helper function to extract fields for a selected line
vector<string> get_fields_by_line_number(const string& file, int lineNumber) {
    ifstream out(file);
    vector<string> fields;
    if (!out.is_open()) return fields;

    string temp;
    string header;
    getline(out, header); 
    int count = 0;
    
    while (getline(out, temp)) {
        count++;
        if (count == lineNumber) {
            stringstream ss(temp);
            string field;
            while(getline(ss, field, '#')) {
                fields.push_back(field);
            }
            break;
        }
    }
    return fields;
}

// --- NEW/MODIFIED File Readers ---

// CALCULATOR/DETAIL VIEWS FOR CAR
// Car Details: Make#Model#Engine#Used#Year#Installments#Price#Down Payment
string Car_file_reader_details(string file, string user_input) {
    int lineNumber = stoi(user_input);
    vector<string> fields = get_fields_by_line_number(file, lineNumber);

    if (fields.size() < 8) {
        cout << BOLD_RED << " Data corruption or invalid selection for Car." << RESET << endl;
        return "Data corruption.\n";
    }

    // Indices: 5=Installments, 6=Price, 7=Down Payment
    string Installments = fields[5];
    string Price = fields[6];
    string DownPayment = fields[7];
    
    double priceVal = to_double(Price);
    double downVal = to_double(DownPayment);
    int instVal = 0;
    try { instVal = stoi(Installments); } catch (...) {}
    
    string monthlyStr = " Invalid data for installment calculation.\n";
    if (instVal > 0 && priceVal >= downVal) {
        double monthly = (priceVal - downVal) / instVal;
        monthlyStr = to_string((long long)monthly);
    } 

    int box_width = 70;
    print_border(box_width, YELLOW);
    cout << BOLD_CYAN << setw(box_width/2 + 20) << " Selected Car Loan Details " << RESET << endl;
    print_border(box_width, YELLOW);
    cout << CYAN << "Make/Model: " << BOLD << fields[0] << " " << fields[1] << RESET << endl;
    cout << CYAN << "Engine/Year: " << BOLD << fields[2] << "cc (" << fields[4] << ")" << RESET << endl;
    cout << CYAN << "Used Status: " << BOLD << fields[3] << RESET << endl;
    cout << CYAN << "Installments: " << BOLD << Installments << RESET << endl;
    cout << CYAN << "Price : " << BOLD << Price << " PKR" << RESET << endl;
    cout << CYAN << "Down Payment : " << BOLD << DownPayment << " PKR" << RESET << endl;
    cout << GREEN << "Monthly Installment : " << BOLD << monthlyStr << " PKR" << RESET << endl;
    print_border(box_width, YELLOW);
    return "";
}

// CALCULATOR/DETAIL VIEWS FOR SCOOTER
// Scooter Details: Make#Model#Distance#Time#Speed#Installments#Price#Down Payment
string Scooter_file_reader_details(string file, string user_input) {
    int lineNumber = stoi(user_input);
    vector<string> fields = get_fields_by_line_number(file, lineNumber);

    if (fields.size() < 8) {
        cout << BOLD_RED << " Data corruption or invalid selection for Scooter." << RESET << endl;
        return "Data corruption.\n";
    }

    string Installments = fields[5];
    string Price = fields[6];
    string DownPayment = fields[7];
    
    double priceVal = to_double(Price);
    double downVal = to_double(DownPayment);
    int instVal = 0;
    try { instVal = stoi(Installments); } catch (...) {}
    
    string monthlyStr = " Invalid data for installment calculation.\n";
    if (instVal > 0 && priceVal >= downVal) {
        double monthly = (priceVal - downVal) / instVal;
        monthlyStr = to_string((long long)monthly);
    } 

    int box_width = 70;
    print_border(box_width, YELLOW);
    cout << BOLD_CYAN << setw(box_width/2 + 20) << " Selected Scooter Loan Details " << RESET << endl;
    print_border(box_width, YELLOW);
    cout << CYAN << "Make/Model: " << BOLD << fields[0] << " " << fields[1] << RESET << endl;
    cout << CYAN << "Distance Per Charge: " << BOLD << fields[2] << " KM" << RESET << endl;
    cout << CYAN << "Charging Time: " << BOLD << fields[3] << " Hrs" << RESET << endl;
    cout << CYAN << "Installments: " << BOLD << Installments << RESET << endl;
    cout << CYAN << "Price : " << BOLD << Price << " PKR" << RESET << endl;
    cout << CYAN << "Down Payment : " << BOLD << DownPayment << " PKR" << RESET << endl;
    cout << GREEN << "Monthly Installment : " << BOLD << monthlyStr << " PKR" << RESET << endl;
    print_border(box_width, YELLOW);
    return "";
}

// Universal reader/formatter for Car/Scooter options (show_all=true)
string generic_loan_file_reader_options(string file, const vector<string>& headers, const vector<int>& col_widths) {
    ifstream out(file);
    string temp;
    string header_line;
    getline(out, header_line);
    int line_count = 0;
    
    int total_width = 0;
    // Calculate total width based on all provided widths
    for(int w : col_widths) total_width += w;

    print_border(total_width, YELLOW);
    
    // Use col_widths[0] for '#'
    cout << BOLD_YELLOW << setw(col_widths[0]) << left << "#"; 
    
    // Use col_widths[i+1] for the header names
    for (size_t i = 0; i < headers.size(); ++i) {
        cout << setw(col_widths[i+1]) << headers[i]; 
    }
    cout << RESET << endl;
    print_border(total_width, YELLOW);
    
    while (getline(out, temp)) {
        line_count++;
        stringstream ss(temp);
        string field;
        vector<string> fields;
        while(getline(ss, field, '#')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= headers.size()) {
            cout << CYAN << setw(col_widths[0]) << left << to_string(line_count);
            for (size_t i = 0; i < fields.size(); ++i) {
                // Use col_widths[i+1] for the data fields
                cout << setw(col_widths[i+1]) << fields[i];
            }
            cout << RESET << endl;
        }
    }
    print_border(total_width, YELLOW);
    return ""; 
}

// Specific reader for Scooter.txt (Routes to display options or details)
string Scooter_file_reader(string user_input, bool show_all) {
    vector<string> headers = {"Make", "Model", "Dist(KM)", "Time(Hrs)", "Speed(KM/H)", "Installments", "Price(PKR)", "Down Payment(PKR)"};
    // Col widths must have 9 elements (1 for # + 8 for headers)
    vector<int> col_widths = {6, 15, 12, 12, 12, 14, 15, 18, 18}; 
    if (show_all) {
        return generic_loan_file_reader_options("Scooter.txt", headers, col_widths);
    } else if (isNumber(user_input)) {
        return Scooter_file_reader_details("Scooter.txt", user_input);
    }
    return "";
}

// Specific reader for Car.txt (Routes to display options or details)
string Car_file_reader(string user_input, bool show_all) {
    vector<string> headers = {"Make", "Model", "Engine(cc)", "Used", "Year", "Installments", "Price(PKR)", "Down Payment(PKR)"};
    // Col widths must have 9 elements (1 for # + 8 for headers)
    vector<int> col_widths = {6, 12, 12, 10, 8, 14, 15, 18, 18};
    if (show_all) {
        return generic_loan_file_reader_options("Car.txt", headers, col_widths);
    } else if (isNumber(user_input)) {
        return Car_file_reader_details("Car.txt", user_input);
    }
    return "";
}

// reads Home.txt and prints selected details + calculates monthly installment with styling
string Home_file_reader(string file, string user_input, bool show_all) {
    ifstream out(file);
    if (!out.is_open()) {
        cout << BOLD_RED << " Could not open Home file." << RESET << endl;
        return " Could not open Home file.\n";
    }
    string temp;
    string header;
    getline(out, header); 
    string formattedResponse = "";
    int line_count = 0;
    
    int col_width = 20;

    if (show_all) {
        print_border(6 * col_width + 5, YELLOW);
        cout << BOLD_YELLOW << setw(col_width) << left << "#"
             << setw(col_width) << "Area"
             << setw(col_width) << "Size"
             << setw(col_width) << "Installments"
             << setw(col_width) << "Price(PKR)"
             << setw(col_width) << "Down Payment(PKR)" << RESET << endl;
        print_border(6 * col_width + 5, YELLOW);
        
        while (getline(out, temp)) {
            line_count++;
            stringstream ss(temp);
            string field;
            vector<string> fields;
            while(getline(ss, field, '#')) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 5) {
                cout << CYAN << setw(col_width) << left << to_string(line_count)
                     << setw(col_width) << fields[0] 
                     << setw(col_width) << fields[1] 
                     << setw(col_width) << fields[2] 
                     << setw(col_width) << fields[3] 
                     << setw(col_width) << fields[4] 
                     << RESET << endl;
            }
        }
        print_border(6 * col_width + 5, YELLOW);
        return ""; 
    }
    
    // Selected option view logic 
    if (isNumber(user_input)) {
        int lineNumber = stoi(user_input);
        vector<string> fields = get_fields_by_line_number(file, lineNumber);
        
        if (fields.size() < 5) {
            cout << BOLD_RED << " Invalid area selection or data corruption." << RESET << endl;
            return "Invalid area selection.\n";
        }

        string Area = fields[0];
        string Size = fields[1];
        string Installments = fields[2];
        string Price = fields[3];
        string DownPayment = fields[4];
        
        double priceVal = to_double(Price);
        double downVal = to_double(DownPayment);
        int instVal = 0;
        try { instVal = stoi(Installments); } catch (...) {}
        
        string monthlyStr = " Invalid data for installment calculation.\n";
        
        if (instVal > 0 && priceVal >= downVal) {
            double monthly = (priceVal - downVal) / instVal;
            monthlyStr = to_string((long long)monthly);
        } 

        int box_width = 60;
        print_border(box_width, YELLOW);
        cout << BOLD_CYAN << setw(box_width/2 + 15) << " Selected Home Loan Details " << RESET << endl;
        print_border(box_width, YELLOW);
        cout << CYAN << "Area: " << BOLD << Area << RESET << endl;
        cout << CYAN << "Size: " << BOLD << Size << RESET << endl;
        cout << CYAN << "Installments: " << BOLD << Installments << RESET << endl;
        cout << CYAN << "Price : " << BOLD << Price << " PKR" << RESET << endl;
        cout << CYAN << "Down Payment : " << BOLD << DownPayment << " PKR" << RESET << endl;
        cout << GREEN << "Monthly Installment : " << BOLD << monthlyStr << " PKR" << RESET << endl;
        print_border(box_width, YELLOW);
        
        return formattedResponse;
    } else {
        return "Invalid input for Home_file_reader.\n";
    }
}

// reads Utterances.txt or other file
string file_reader(string file, string user_input, bool show_all) {
    if (file == "Utterances.txt") {
        string temp;
        bool response_initiated = false;
        string default_response = "";
        
        ifstream out(file);
        if (!out.is_open()) {
            cout << BOLD_RED << " Could not open Utterances file." << RESET << endl;
            return " Could not open Utterances file.\n";
        }
        while (getline(out, temp)) {
            bool temp_res = false;
            string parsed = string_parser(temp, temp_res, user_input);
            
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
    } else if (file == "Home.txt") {
        return Home_file_reader(file, user_input, show_all);
    } else if (file == "Car.txt") {
        return Car_file_reader(user_input, show_all);
    } else if (file == "Scooter.txt") {
        return Scooter_file_reader(user_input, show_all);
    }
    // Handle other files or default
    return "Unknown file type.";
}


// --- LoanApplication Class Methods (Unchanged) ---

string LoanApplication::to_string_row() const {
    string row = "";
    row += applicationId + "#"; 
    row += loanCategory + "#"; 
    row += status + "#";       
    
    row += fullName + "#";
    row += fatherName + "#";
    row += postalAddress + "#";
    row += contactNumber + "#";
    row += emailAddress + "#";
    row += cnicNumber + "#";
    row += cnicExpiryDate + "#";
    row += employmentStatus + "#";
    row += maritalStatus + "#";
    row += gender + "#";
    row += numDependents + "#";
    row += annualIncome + "#";
    row += monthlyAvgElectricityBill + "#";
    row += currentElectricityBill + "#";
    
    row += loanActive + "#";
    row += totalLoanAmount + "#";
    row += amountReturned + "#";
    row += loanDue + "#";
    row += bankName + "#";
    row += existingLoanCategory + "#";

    row += ref1Name + "#";
    row += ref1Cnic + "#";
    row += ref1CnicIssueDate + "#";
    row += ref1Phone + "#";
    row += ref1Email + "#";
    row += ref2Name + "#";
    row += ref2Cnic + "#";
    row += ref2CnicIssueDate + "#";
    row += ref2Phone + "#";
    row += ref2Email + "#";

    row += cnicFrontPath + "#";
    row += cnicBackPath + "#";
    row += electricityBillPath + "#";
    row += salarySlipPath; 
    
    return row;
}

void LoanApplication::from_string_row(const string& row) {
    stringstream ss(row);
    string field;
    vector<string> fields;
    while(getline(ss, field, '#')) {
        fields.push_back(field);
    }
    
    if (fields.size() < 37) {
        return;
    }

    applicationId = fields[0];
    loanCategory = fields[1];
    status = fields[2];
    fullName = fields[3];
    fatherName = fields[4];
    postalAddress = fields[5];
    contactNumber = fields[6];
    emailAddress = fields[7];
    cnicNumber = fields[8];
    cnicExpiryDate = fields[9];
    employmentStatus = fields[10];
    maritalStatus = fields[11];
    gender = fields[12];
    numDependents = fields[13];
    annualIncome = fields[14];
    monthlyAvgElectricityBill = fields[15];
    currentElectricityBill = fields[16];
    loanActive = fields[17];
    totalLoanAmount = fields[18];
    amountReturned = fields[19];
    loanDue = fields[20];
    bankName = fields[21];
    existingLoanCategory = fields[22];
    ref1Name = fields[23];
    ref1Cnic = fields[24];
    ref1CnicIssueDate = fields[25];
    ref1Phone = fields[26];
    ref1Email = fields[27];
    ref2Name = fields[28];
    ref2Cnic = fields[29];
    ref2CnicIssueDate = fields[30];
    ref2Phone = fields[31];
    ref2Email = fields[32];
    cnicFrontPath = fields[33];
    cnicBackPath = fields[34];
    electricityBillPath = fields[35];
    salarySlipPath = fields[36];
}

// --- Validation and ID Management (Unchanged) ---

bool is13DigitCnic(const string &s) {
    if (s.length() != 13) return false;
    for (char const &c : s) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

bool is_valid_date(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    if (!isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3]) || !isdigit(date[4]) || 
        !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9])) return false;
    return true;
}

bool is_valid_loan_status(const string& status) {
    string lower_status = to_lower(status);
    return lower_status == "active" || lower_status == "not active";
}

bool is_unique_id(const string& id) {
    ifstream appFile("applications.txt");
    if (!appFile.is_open()) {
        return true;
    }

    string line;
    LoanApplication tempApp;
    while (getline(appFile, line)) {
        tempApp.from_string_row(line);
        if (tempApp.applicationId == id) {
            appFile.close();
            return false;
        }
    }
    appFile.close();
    return true;
}

string generate_unique_id() {
    int id_counter = 1; 
    string id;
    
    while (id_counter <= 9999) {
        id = to_string(id_counter);
        while (id.length() < 4) {
            id = "0" + id;
        }
        
        if (is_unique_id(id)) {
            return id;
        }
        id_counter++;
    }
    
    return "FULL"; 
}

// --- Data Persistence (Unchanged) ---

void save_application_to_file(const LoanApplication& app) {
    ofstream appFile("applications.txt", ios::app);
    if (!appFile.is_open()) {
        cout << BOLD_RED << "Error: Could not open applications.txt for saving." << RESET << endl;
        return;
    }

    appFile.seekp(0, ios::end);
    if (appFile.tellp() == 0) {
        cout << BOLD_YELLOW << "Creating applications.txt for the first time..." << RESET << endl;
    }
    
    appFile << app.to_string_row() << endl; 
    appFile.close();
}


// --- Main Application Collection (Unchanged) ---

string collect_application_data(const string& category) {
    
    string appId = generate_unique_id();
    if (appId == "FULL") {
        cout << BOLD_RED << "\n" << CROSS_MARK << " Error: Maximum application limit reached (9999). Cannot submit new application." << RESET << endl;
        return "";
    }
    
    LoanApplication app;
    app.loanCategory = category;
    app.applicationId = appId;
    string userInput;
    
    cout << BOLD_GREEN << "\n--- Starting Loan Application (ID: " << app.applicationId << ", Category: " << app.loanCategory << ") ---" << RESET << endl;
    print_border(80, GREEN);

    // --- 1. Personal Details ---
    cout << BOLD_YELLOW << "\n[1/4] Personal Information" << RESET << endl;
    print_border(40, YELLOW);
    
    cout << CYAN << "Full Name: " << RESET; getline(cin, app.fullName);
    cout << CYAN << "Father's Name: " << RESET; getline(cin, app.fatherName);
    cout << CYAN << "Postal Address: " << RESET; getline(cin, app.postalAddress);

    // Contact Number (Validation)
    while (true) {
        cout << CYAN << "Contact Number: " << RESET; getline(cin, app.contactNumber);
        if (isNumber(app.contactNumber)) {
            cout << CHECK_MARK << " Contact number accepted." << endl;
            break;
        }
        cout << CROSS_MARK << BOLD_RED << " Invalid input! Please enter a numeric contact number." << RESET << endl;
    }
    
    cout << CYAN << "Email Address: " << RESET; getline(cin, app.emailAddress);

    // CNIC Number (13-digit Validation)
    while (true) {
        cout << CYAN << "CNIC Number (13 digits, no dashes): " << RESET; getline(cin, app.cnicNumber);
        if (is13DigitCnic(app.cnicNumber)) {
            cout << CHECK_MARK << " CNIC number validated." << endl;
            break;
        }
        cout << CROSS_MARK << BOLD_RED << " Invalid input! CNIC must be exactly 13 numeric digits without dashes." << RESET << endl;
    }

    // CNIC Expiry Date (Validation)
    while (true) {
        cout << CYAN << "CNIC Expiry Date (DD-MM-YYYY): " << RESET; getline(cin, app.cnicExpiryDate);
        if (is_valid_date(app.cnicExpiryDate)) {
             cout << CHECK_MARK << " Expiry date accepted." << endl;
            break;
        }
        cout << CROSS_MARK << BOLD_RED << " Invalid format! Please use DD-MM-YYYY." << RESET << endl;
    }

    // Employment Status (MENU SELECTION)
    while (true) {
        cout << CYAN << "\nSelect Employment Status:" << RESET << endl;
        cout << "1. Self-employed\n2. Salaried\n3. Retired\n4. Unemployed" << endl;
        cout << BOLD_CYAN << "Enter option (1-4): " << RESET; getline(cin, userInput);

        if (userInput == "1") { app.employmentStatus = "Self-employed"; break; }
        if (userInput == "2") { app.employmentStatus = "Salaried"; break; }
        if (userInput == "3") { app.employmentStatus = "Retired"; break; }
        if (userInput == "4") { app.employmentStatus = "Unemployed"; break; }
        
        cout << CROSS_MARK << BOLD_RED << " Invalid option! Please enter 1, 2, 3, or 4." << RESET << endl;
    }
    cout << CHECK_MARK << " Employment status recorded: " << app.employmentStatus << endl;

    // Marital Status (MENU SELECTION)
    while (true) {
        cout << CYAN << "\nSelect Marital Status:" << RESET << endl;
        cout << "1. Single\n2. Married\n3. Divorced\n4. Widowed" << endl;
        cout << BOLD_CYAN << "Enter option (1-4): " << RESET; getline(cin, userInput);

        if (userInput == "1") { app.maritalStatus = "Single"; break; }
        if (userInput == "2") { app.maritalStatus = "Married"; break; }
        if (userInput == "3") { app.maritalStatus = "Divorced"; break; }
        if (userInput == "4") { app.maritalStatus = "Widowed"; break; }
        
        cout << CROSS_MARK << BOLD_RED << " Invalid option! Please enter 1, 2, 3, or 4." << RESET << endl;
    }
    cout << CHECK_MARK << " Marital status recorded: " << app.maritalStatus << endl;

    // Gender (MENU SELECTION)
    while (true) {
        cout << CYAN << "\nSelect Gender:" << RESET << endl;
        cout << "1. Male\n2. Female\n3. Other" << endl;
        cout << BOLD_CYAN << "Enter option (1-3): " << RESET; getline(cin, userInput);

        if (userInput == "1") { app.gender = "Male"; break; }
        if (userInput == "2") { app.gender = "Female"; break; }
        if (userInput == "3") { app.gender = "Other"; break; }
        
        cout << CROSS_MARK << BOLD_RED << " Invalid option! Please enter 1, 2, or 3." << RESET << endl;
    }
    cout << CHECK_MARK << " Gender recorded: " << app.gender << endl;


    // Number of Dependents (Validation)
    while (true) {
        cout << CYAN << "Number of dependents: " << RESET; getline(cin, app.numDependents);
        if (isNumber(app.numDependents)) {
             cout << CHECK_MARK << " Dependents count accepted." << endl;
            break;
        }
        cout << CROSS_MARK << BOLD_RED << " Invalid input! Must be numeric." << RESET << endl;
    }

    // Income and Bills (Validation)
    while (true) {
        cout << CYAN << "Annual income in PKR (numeric, no commas): " << RESET; getline(cin, app.annualIncome);
        if (isNumber(app.annualIncome)) break;
        cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric without commas." << RESET << endl;
    }
    while (true) {
        cout << CYAN << "Monthly avg electricity bill for last 12 months (PKR, numeric): " << RESET; getline(cin, app.monthlyAvgElectricityBill);
        if (isNumber(app.monthlyAvgElectricityBill)) break;
        cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric without commas." << RESET << endl;
    }
    while (true) {
        cout << CYAN << "Exact amount of current electricity bill (PKR, numeric): " << RESET; getline(cin, app.currentElectricityBill);
        if (isNumber(app.currentElectricityBill)) break;
        cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric without commas." << RESET << endl;
    }
    
    // --- 2. Existing Loan Details (MODIFIED FOR MENU SELECTION) ---
    cout << BOLD_YELLOW << "\n[2/4] Existing Loan Information" << RESET << endl;
    
    string hasLoan;
    // Loop for YES/NO (X exit added)
    while (true) {
        cout << BOLD_CYAN << "Do you have any existing loans (Y/N/X)? " << RESET; getline(cin, hasLoan);
        string lower_hasLoan = to_lower(hasLoan);
        if (lower_hasLoan == "x") { return ""; } // Exit application
        if (lower_hasLoan == "y" || lower_hasLoan == "n") break;
        cout << CROSS_MARK << BOLD_RED << " Invalid input! Please enter Y (Yes), N (No), or X (Exit)." << RESET << endl;
    }

    if (to_lower(hasLoan) == "y") {
        string loanActiveOption;
        // Loan period Active or not (MENU SELECTION)
        while (true) {
            cout << CYAN << "\nLoan period Status:" << RESET << endl;
            cout << "1. Active\n2. Not Active" << endl;
            cout << BOLD_CYAN << "Enter option (1 or 2): " << RESET; getline(cin, loanActiveOption);
            
            if (loanActiveOption == "1") { app.loanActive = "Active"; break; }
            if (loanActiveOption == "2") { app.loanActive = "Not Active"; break; }
            
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Enter 1 or 2." << RESET << endl;
        }

        while (true) {
            cout << CYAN << "Total Loan Amount: " << RESET; getline(cin, app.totalLoanAmount);
            if (isNumber(app.totalLoanAmount)) break;
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric." << RESET << endl;
        }

        while (true) {
            cout << CYAN << "Amount Returned: " << RESET; getline(cin, app.amountReturned);
            if (isNumber(app.amountReturned)) break;
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric." << RESET << endl;
        }

        while (true) {
            cout << CYAN << "Loan Amount Still Due: " << RESET; getline(cin, app.loanDue);
            if (isNumber(app.loanDue)) break;
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric." << RESET << endl;
        }

        cout << CYAN << "Bank Name: " << RESET; getline(cin, app.bankName);
        cout << CYAN << "Existing Loan Category (Car, Home, Bike, Personal): " << RESET; getline(cin, app.existingLoanCategory);
    } else {
        app.loanActive = "N/A";
        app.totalLoanAmount = "0";
        app.amountReturned = "0";
        app.loanDue = "0";
        app.bankName = "N/A";
        app.existingLoanCategory = "N/A";
    }

    // --- 3. Details of References (Unchanged) ---
    cout << BOLD_YELLOW << "\n[3/4] Reference Information" << RESET << endl;

    // Referee 1
    cout << CYAN << "\n-- Referee 1 --" << RESET << endl;
    cout << CYAN << "Name of Referee 1: " << RESET; getline(cin, app.ref1Name);
    
    while (true) {
        cout << CYAN << "CNIC of Referee 1 (13 digits, no dashes): " << RESET; getline(cin, app.ref1Cnic);
        if (is13DigitCnic(app.ref1Cnic)) {
            cout << CHECK_MARK << " Referee CNIC validated." << endl;
            break;
        }
        cout << CROSS_MARK << BOLD_RED << " Invalid input! CNIC must be exactly 13 numeric digits without dashes." << RESET << endl;
    }
    
    while (true) {
        cout << CYAN << "Issue date of CNIC of Referee 1 (DD-MM-YYYY): " << RESET; getline(cin, app.ref1CnicIssueDate);
        if (is_valid_date(app.ref1CnicIssueDate)) break;
        cout << CROSS_MARK << BOLD_RED << " Invalid format! Please use DD-MM-YYYY." << RESET << endl;
    }
    cout << CYAN << "Phone number of Referee 1: " << RESET; getline(cin, app.ref1Phone);
    cout << CYAN << "Email address of Referee 1: " << RESET; getline(cin, app.ref1Email);

    // Referee 2
    cout << CYAN << "\n-- Referee 2 --" << RESET << endl;
    cout << CYAN << "Name of Referee 2: " << RESET; getline(cin, app.ref2Name);
    
    while (true) {
        cout << CYAN << "CNIC of Referee 2 (13 digits, no dashes): " << RESET; getline(cin, app.ref2Cnic);
        if (is13DigitCnic(app.ref2Cnic)) {
            cout << CHECK_MARK << " Referee CNIC validated." << endl;
            break;
        }
        cout << CROSS_MARK << BOLD_RED << " Invalid input! CNIC must be exactly 13 numeric digits without dashes." << RESET << endl;
    }
    
    while (true) {
        cout << CYAN << "Issue date of CNIC of Referee 2 (DD-MM-YYYY): " << RESET; getline(cin, app.ref2CnicIssueDate);
        if (is_valid_date(app.ref2CnicIssueDate)) break;
        cout << CROSS_MARK << BOLD_RED << " Invalid format! Please use DD-MM-YYYY." << RESET << endl;
    }
    cout << CYAN << "Phone number of Referee 2: " << RESET; getline(cin, app.ref2Phone);
    cout << CYAN << "Email address of Referee 2: " << RESET; getline(cin, app.ref2Email);
    
    // --- 4. Image Paths (Mocking the storage of paths, ADDING IMAGE PROMPTS) ---
    cout << BOLD_YELLOW << "\n[4/4] Required Document Paths" << RESET << endl;
    cout << MAGENTA << "The following image paths are recorded and saved to ./data/" << app.applicationId << " folder:" << RESET << endl;

    // Documents required:
    cout << CYAN << " - Picture of applicant's CNIC Front:" << RESET;
    app.cnicFrontPath = "./data/" + app.applicationId + "/CNIC_Front.jpg";
    cout << CHECK_MARK << app.cnicFrontPath << endl;
    
    
    cout << CYAN << " - Picture of applicant's CNIC Back:" << RESET;
    app.cnicBackPath = "./data/" + app.applicationId + "/CNIC_Back.jpg";
    cout << CHECK_MARK << app.cnicBackPath << endl;
    
    
    cout << CYAN << " - Picture of applicant's recent electricity bill:" << RESET;
    app.electricityBillPath = "./data/" + app.applicationId + "/Electricity_Bill.jpg";
    cout << CHECK_MARK << app.electricityBillPath << endl;
    
    
    cout << CYAN << " - Picture of salary slip/Bank statement page 1:" << RESET;
    app.salarySlipPath = "./data/" + app.applicationId + "/Salary_Slip.jpg";
    cout << CHECK_MARK << app.salarySlipPath << endl;
    
    
    // --- 5. Confirmation and Submission (Exit option added) ---
    print_border(80, GREEN);
    cout << BOLD_YELLOW << "\nConfirmation" << RESET << endl;
    
    string confirm;
    while (true) {
        cout << BOLD_CYAN << "Do you confirm and want to submit this application (Y/N/X)? " << RESET;
        getline(cin, confirm);
        
        string lower_confirm = to_lower(confirm);

        if (lower_confirm == "x") {
            cout << BOLD_RED << "\nSubmission cancelled by user." << RESET << endl;
            return ""; 
        }

        if (lower_confirm == "y") {
            save_application_to_file(app);
            cout << BOLD_GREEN << "\n" << CHECK_MARK << " Application ID " << app.applicationId << " successfully submitted! Status: Submitted." << RESET << endl;
            return app.applicationId;
        } else if (lower_confirm == "n") {
            cout << BOLD_RED << "\nApplication submission cancelled." << RESET << endl;
            return "";
        } else {
            cout << CROSS_MARK << BOLD_RED << " Invalid choice. Please enter Y (Yes), N (No), or X (Exit)." << RESET << endl;
        }
    }
}

// --- Lender Interface Functions (FIXED viewDetails to show all paths) ---

void viewApplications() {
    ifstream appFile("applications.txt");
    if (!appFile.is_open()) {
        cout << BOLD_RED << "\nNo applications file found or could not open applications.txt." << RESET << endl;
        return;
    }

    string line;
    int col_width = 18;
    cout << BOLD_MAGENTA << "\n--- Loan Applications List ---" << RESET << endl;
    print_border(5 * col_width + 4, MAGENTA);
    cout << BOLD_MAGENTA << setw(col_width) << left << "ID"
         << setw(col_width) << "Status"
         << setw(col_width) << "Category"
         << setw(col_width) << "Full Name"
         << setw(col_width) << "CNIC" << RESET << endl;
    print_border(5 * col_width + 4, MAGENTA);

    while (getline(appFile, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        while (getline(ss, field, '#')) {
            fields.push_back(field);
        }

        if (fields.size() >= 37) { 
            cout << MAGENTA << setw(col_width) << left << fields[0] 
                 << setw(col_width) << fields[2] 
                 << setw(col_width) << fields[1] 
                 << setw(col_width) << fields[3] 
                 << setw(col_width) << fields[8] 
                 << RESET << endl;
        }
    }
    print_border(5 * col_width + 4, MAGENTA);
    appFile.close();
}

void viewDetails(const string& appId) {
    ifstream appFile("applications.txt");
    if (!appFile.is_open()) {
        cout << BOLD_RED << "\nCould not open applications.txt." << RESET << endl;
        return;
    }

    string line;
    LoanApplication app;
    bool found = false;

    while (getline(appFile, line)) {
        app.from_string_row(line);
        if (app.applicationId == appId) {
            found = true;
            break;
        }
    }
    appFile.close();

    if (found) {
        int width = 70;
        cout << BOLD_CYAN << "\n-- Details for Application ID: " << appId << " (" << app.status << ") --" << RESET << endl;
        print_border(width, CYAN);
        
        cout << BOLD_YELLOW << "General Info:" << RESET << endl;
        cout << CYAN << setw(25) << left << "Loan Category:" << MAGENTA << app.loanCategory << RESET << endl;
        cout << CYAN << setw(25) << left << "Full Name:" << MAGENTA << app.fullName << RESET << endl;
        cout << CYAN << setw(25) << left << "CNIC Number:" << MAGENTA << app.cnicNumber << RESET << endl;
        cout << CYAN << setw(25) << left << "Contact Number:" << MAGENTA << app.contactNumber << RESET << endl;
        cout << CYAN << setw(25) << left << "Annual Income (PKR):" << MAGENTA << app.annualIncome << RESET << endl;
        
        print_border(width, CYAN);
        cout << BOLD_YELLOW << "Employment & Marital:" << RESET << endl;
        cout << CYAN << setw(25) << left << "Employment Status:" << MAGENTA << app.employmentStatus << RESET << endl;
        cout << CYAN << setw(25) << left << "Marital Status:" << MAGENTA << app.maritalStatus << RESET << endl;
        
        print_border(width, CYAN);
        cout << BOLD_YELLOW << "Existing Loan Status:" << RESET << endl;
        cout << CYAN << setw(25) << left << "Loan Active:" << MAGENTA << app.loanActive << RESET << endl;
        cout << CYAN << setw(25) << left << "Loan Due (PKR):" << MAGENTA << app.loanDue << RESET << endl;

        print_border(width, CYAN);
        cout << BOLD_YELLOW << "Reference 1:" << RESET << endl;
        cout << CYAN << setw(25) << left << "Name:" << MAGENTA << app.ref1Name << RESET << endl;
        cout << CYAN << setw(25) << left << "Phone:" << MAGENTA << app.ref1Phone << RESET << endl;
        
        // FIX: Displaying all document paths
        print_border(width, CYAN);
        cout << BOLD_YELLOW << "Image Paths (data/):" << RESET << endl;
        cout << CYAN << setw(30) << left << "CNIC Front:" << MAGENTA << app.cnicFrontPath << RESET << endl; 
        cout << CYAN << setw(30) << left << "CNIC Back:" << MAGENTA << app.cnicBackPath << RESET << endl;
        cout << CYAN << setw(30) << left << "Electricity Bill:" << MAGENTA << app.electricityBillPath << RESET << endl;
        cout << CYAN << setw(30) << left << "Salary Slip/Bank Statement:" << MAGENTA << app.salarySlipPath << RESET << endl;
        print_border(width, CYAN);
        
    } else {
        cout << BOLD_RED << "Application ID " << appId << " not found." << RESET << endl;
    }
}

void generateMonthlyPlan(const string& appId, const string& category, const string& startingMonth) {
    cout << BOLD_GREEN << "\n--- Monthly Payment Plan Generation ---" << RESET << endl;
    cout << GREEN << "Plan generated for Application ID: " << appId << " (" << category << ")" << RESET << endl;
    cout << GREEN << "Starting Month: " << startingMonth << RESET << endl;
    cout << "A full plan would show month-wise payments, total paid, and remaining balance." << endl;
    print_border(60, GREEN);
}

void updateApplicationStatus(const string& appId, const string& newStatus) {
    ifstream appFileIn("applications.txt");
    if (!appFileIn.is_open()) {
        cout << BOLD_RED << "\nCould not open applications.txt." << RESET << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool found = false;
    LoanApplication app;

    while (getline(appFileIn, line)) {
        app.from_string_row(line);
        if (app.applicationId == appId) {
            found = true;
            app.status = newStatus;
            lines.push_back(app.to_string_row());
        } else {
            lines.push_back(line);
        }
    }
    appFileIn.close();

    if (!found) {
        cout << BOLD_RED << "Application ID " << appId << " not found." << RESET << endl;
        return;
    }

    ofstream appFileOut("applications.txt");
    if (!appFileOut.is_open()) {
        cout << BOLD_RED << "Error: Could not save applications file." << RESET << endl;
        return;
    }
    for (const string& l : lines) {
        appFileOut << l << endl;
    }
    appFileOut.close();

    cout << BOLD_GREEN << "\n" << CHECK_MARK << " Status of Application ID " << appId << " successfully updated to: " << newStatus << RESET << endl;
    
    if (newStatus == "Approved") {
        string startingMonth;
        cout << BOLD_CYAN << "Enter the starting month for the payment plan (e.g., Nov-2025): " << RESET;
        getline(cin, startingMonth);
        generateMonthlyPlan(appId, app.loanCategory, startingMonth);
    }
}

void runLenderInterface() {
    cout << BOLD_MAGENTA << "\n=======================================================" << RESET << endl;
    cout << BOLD_MAGENTA << "             Lender / Admin Console Interface            " << RESET << endl;
    cout << BOLD_MAGENTA << "=======================================================" << RESET << endl;
    string userInput;
    while (true) {
        cout << BOLD_CYAN << "\nLender Menu:\n"
             << MAGENTA << "V - View all applications\n"
             << MAGENTA << "D - View details of an application\n"
             << MAGENTA << "A - Approve an application\n"
             << MAGENTA << "R - Reject an application\n"
             << MAGENTA << "X - Exit" << RESET << BOLD_CYAN << "\n> " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);

        if (lower_input == "x") {
            cout << GREEN << "Exiting Lender Interface." << RESET << endl;
            return;
        } else if (lower_input == "v") {
            viewApplications();
        } else if (lower_input == "d") {
            string appId;
            cout << CYAN << "Enter Application ID to view details (e.g., 0001): " << RESET;
            getline(cin, appId);
            viewDetails(appId);
        } else if (lower_input == "a") {
            string appId;
            cout << CYAN << "Enter Application ID to APPROVE: " << RESET;
            getline(cin, appId);
            updateApplicationStatus(appId, "Approved");
        } else if (lower_input == "r") {
            string appId;
            cout << CYAN << "Enter Application ID to REJECT: " << RESET;
            getline(cin, appId);
            updateApplicationStatus(appId, "Rejected");
        } else {
            cout << BOLD_RED << "Invalid option." << RESET << endl;
        }
    }
}


// --- Main Chatbot Function (FIXED STREAMING ERROR) ---
void runChatbot() {
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
    bool greeted = false; 
    bool applied = false; 
    
    // Greeting Loop
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
    
    // Apply Loop
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
    
    // Category Selection Loop
    string selectedCategory = "";
    while (selectedCategory.empty()) {
        // FIX: Removed streaming error by calling print_border separately.
        print_border(30, CYAN); 
        cout << BOLD_CYAN << "SELECT LOAN CATEGORY" << endl
             << CYAN << "H - Home Loan\nC - Car Loan\nS - Scooter Loan\nP - Personal Loan\nX - Exit" << RESET << BOLD_CYAN << "\n> " << RESET;
        
        getline(cin, userInput);
        string lower_input = to_lower(userInput);
        
        if (lower_input == "x") {
            cout << GREEN << "Thanks for coming" << RESET << endl;
            return;
        }
        
        if (lower_input == "h" || lower_input == "c" || lower_input == "s" || lower_input == "p") {
            selectedCategory = (lower_input == "h") ? "Home" : 
                               (lower_input == "c") ? "Car" : 
                               (lower_input == "s") ? "Scooter" : "Personal";
            
            cout << BOLD_YELLOW << "\nAvailable " << selectedCategory << " Loan Options:" << RESET << endl;

            string fileName = selectedCategory + ".txt";
            bool optionsAvailable = (lower_input != "p");

            if (!optionsAvailable) {
                 cout << BOLD_RED << "No specific options list for Personal Loan. Proceeding to application..." << RESET << endl;
            } else {
                // Show options table for H, C, or S
                file_reader(fileName, "", true); 
            }
            
            string optionNumber; 
            bool optionSelected = false;

            // Option Selection Loop (Only runs if options were displayed)
            while (optionsAvailable && !optionSelected) {
                cout << BOLD_CYAN << "\nEnter the OPTION NUMBER you want to select (e.g., 1, 2, ...) or R to re-view options, or X to exit: " << RESET;
                getline(cin, optionNumber);
                string lower_option = to_lower(optionNumber);
                
                if (lower_option == "x") {
                    cout << GREEN << "Application cancelled. Returning to main menu." << RESET << endl;
                    return; 
                }
                
                if (lower_option == "r") {
                     file_reader(fileName, "", true);
                     continue;
                }
                
                if (isNumber(optionNumber)) {
                    // Display detailed selection and installment calculation (Break out of the loop)
                    file_reader(fileName, optionNumber, false); 
                    optionSelected = true;
                } else {
                    cout << BOLD_RED << "Invalid input! Please enter a valid OPTION NUMBER, R, or X." << RESET << endl;
                }
            }
            
            // Proceed to data collection
            collect_application_data(selectedCategory);
            
            // After application attempt, return to main menu (main function loop)
            return; 

        } else {
            cout << BOLD_RED << " Invalid input! Please select H, C, S, P, or X." << RESET << endl;
        }
    }
}

// --- Main Function (Unchanged) ---
#ifndef UNIT_TEST
int main() {
    string mode;
    while(true) {
        cout << BOLD_YELLOW << "\n--- System Mode Selection ---" << RESET << endl;
        cout << BOLD_CYAN << "Enter 'B' for Bot Interface (Applicant) or 'L' for Lender Interface, or 'X' to exit: " << RESET;
        getline(cin, mode);
        
        string lower_mode = to_lower(mode);
        
        if (lower_mode == "x") {
            cout << GREEN << "System exiting. Goodbye!" << RESET << endl;
            break;
        } else if (lower_mode == "b") {
            runChatbot();
        } else if (lower_mode == "l") {
            runLenderInterface();
        } else {
            cout << BOLD_RED << "Invalid mode selection. Please enter B, L, or X." << RESET << endl;
        }
    }

    system("PAUSE"); 
    return 0;
}
#endif