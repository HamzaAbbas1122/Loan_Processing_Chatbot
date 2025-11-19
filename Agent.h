#pragma once 

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <algorithm>
#include <cstdlib>

using namespace std;

// --- ANSI Color Definitions ---
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";
const string BOLD_RED = BOLD + RED;
const string BOLD_GREEN = BOLD + GREEN;
const string BOLD_YELLOW = BOLD + YELLOW;
const string BOLD_CYAN = BOLD + CYAN;
const string BOLD_MAGENTA = BOLD + MAGENTA;

// --- UI Indicators ---
const string CHECK_MARK = BOLD_GREEN + " \u2713 " + RESET; // Unicode Check Mark
const string CROSS_MARK = BOLD_RED + " \u2717 " + RESET;  // Unicode Cross Mark

// --- LoanApplication Class ---
class LoanApplication {
public:
    // Application Details 
    string applicationId; 
    string loanCategory;  
    string status;        

    // Personal Details 
    string fullName;
    string fatherName;
    string postalAddress;
    string contactNumber;
    string emailAddress;
    string cnicNumber;
    string cnicExpiryDate; 
    string employmentStatus; 
    string maritalStatus;  
    string gender;
    string numDependents;
    string annualIncome;
    string monthlyAvgElectricityBill;
    string currentElectricityBill;

    // Existing Loan Details 
    string loanActive;
    string totalLoanAmount;
    string amountReturned;
    string loanDue;
    string bankName;
    string existingLoanCategory;

    // Reference Details 
    string ref1Name;
    string ref1Cnic;
    string ref1CnicIssueDate;
    string ref1Phone;
    string ref1Email;
    string ref2Name;
    string ref2Cnic;
    string ref2CnicIssueDate;
    string ref2Phone;
    string ref2Email;

    // Image Paths 
    string cnicFrontPath;
    string cnicBackPath;
    string electricityBillPath;
    string salarySlipPath;

    LoanApplication() : applicationId(""), loanCategory(""), status("Submitted") {}
    
    // Core methods
    string to_string_row() const;
    void from_string_row(const string& row); 
};

// --- Agent Functions (Original) ---
string to_lower(string s);
bool compare_responses(string user_greet, string response, string user_input);
string string_parser(string line, bool &res, string user_input);
bool isNumber(const string &s);
double to_double(string s);
void print_border(int width, const string& color = YELLOW);
string Home_file_reader(string file, string user_input, bool show_all = false);
string file_reader(string file, string user_input, bool show_all = false);
void runChatbot();

// --- Increment 2 & Robustness Prototypes ---
// ID Management
string generate_unique_id();
bool is_unique_id(const string& id); 

// Validation Helpers
bool is13DigitCnic(const string &s);
bool is_valid_date(const string& date);
bool is_valid_loan_status(const string& status);

// Application Flow
string collect_application_data(const string& category);
void save_application_to_file(const LoanApplication& app);

// Detail Readers (for Car/Scooter)
string Car_file_reader(string user_input, bool show_all);
string Scooter_file_reader(string user_input, bool show_all);

// --- Lender Interface (Admin) Prototypes ---
void runLenderInterface(); 
void viewApplications();
void viewDetails(const string& appId);
void updateApplicationStatus(const string& appId, const string& newStatus);
void generateMonthlyPlan(const string& appId, const string& category, const string& startingMonth);

// Helper for detail extraction
vector<string> get_fields_by_line_number(const string& file, int lineNumber);