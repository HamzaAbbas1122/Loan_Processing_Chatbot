#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>
#include <iostream>
#include <set>          // Required for IoU functions
#include <filesystem>   // Required for document upload

using namespace std;

// --- Color and UI Constants ---
// Using inline constants to avoid redefinition errors and match .cpp usage
const string RESET = "\033[0m";
const string BOLD_RED = "\033[1;31m";
const string BOLD_GREEN = "\033[1;32m";
const string BOLD_YELLOW = "\033[1;33m";
const string BOLD_CYAN = "\033[1;36m";
const string BOLD_MAGENTA = "\033[1;35m";
const string GREEN = "\033[0;32m";
const string CYAN = "\033[0;36m";
const string MAGENTA = "\033[0;35m";
const string YELLOW = "\033[0;33m";
const string BOLD = "\033[1m";

// UI Marks — defined as string literals (not using color vars in .cpp)
// In your .cpp, CHECK_MARK and CROSS_MARK are used as macros or inline strings
// So we keep them as raw UTF-8 characters
const string CHECK_MARK = "\u2713";  // ✔
const string CROSS_MARK = "\u2717";  // ✗

// --- LoanApplication Class ---
class LoanApplication {
public:
    // Core Application Tracking
    string applicationId;
    string loanCategory;
    string status;

    // Personal Information (C1)
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

    // Financial Information (C2)
    string annualIncome;
    string monthlyAvgElectricityBill;
    string currentElectricityBill;
    string loanActive;
    string totalLoanAmount;
    string amountReturned;
    string loanDue;
    string bankName;
    string existingLoanCategory;

    // References (C3)
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

    // Documents (C4)
    string cnicFrontPath;
    string cnicBackPath;
    string electricityBillPath;
    string salarySlipPath;

    // Methods
    string to_string_row() const;
    void from_string_row(const string& row);
    bool isC1Complete() const;
    bool isC2Complete() const;
    bool isC3Complete() const;
    bool isC4Complete() const;

    // Constructor
    LoanApplication();
};

// --- Helper Function Prototypes ---
string to_lower(string s);
vector<string> tokenize(const string &s);
double calculate_iou(const string &s1, const string &s2);
string IoU_response(const string &user_input, const string &corpusFile);

bool isNumber(const string &s);
double to_double(string s); // Added — used in .cpp
bool is13DigitCnic(const string &s);
bool is_valid_date(const string &date);

// ID Management
string generate_unique_id();
bool is_unique_id(const string &id);
int get_max_id(); // Added — used in .cpp

// File & Data Helpers
void print_border(int width, const string &color);
vector<string> get_fields_by_line_number(const string &file, int lineNumber);

// Loan Option Readers
string Home_file_reader(string file, string user_input, bool show_all);
string Car_file_reader(string user_input, bool show_all);
string Scooter_file_reader(string user_input, bool show_all);
// Note: Personal_file_reader NOT used in .cpp → removed

// Data Persistence
void save_application_to_file(const LoanApplication &app);
LoanApplication load_application_by_id(const string &appId);

// Checkpoint Collectors
string collect_personal_info(LoanApplication &app);
string collect_financial_info(LoanApplication &app);
string collect_references(LoanApplication &app);
string collect_documents(LoanApplication &app);
string collect_application_data(const string &category, const string &existingId = "", const string &existingCnic = "");

// Lender Interface
void viewApplications();
void viewDetails(const string &appId);
void updateApplicationStatus(const string &appId, const string &newStatus);
void generateMonthlyPlan(const string &appId, const string &category, const string &startingMonth);
void runLenderInterface();

// Main Interfaces
void runChatbot();

#endif // AGENT_H