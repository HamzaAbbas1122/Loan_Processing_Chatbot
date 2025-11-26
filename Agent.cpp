#include "Agent.h"
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <set>
#include <filesystem>
#include <iterator>

using namespace std;

// Define custom colors and marks for console output (Using Macros for compatibility)
#define BOLD "\033[1m" // Added BOLD macro definition
#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_MAGENTA "\033[1;35m"
#define GREEN "\033[0;32m"
#define CYAN "\033[0;36m"
#define MAGENTA "\033[0;35m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"
#define CHECK_MARK "\u2713"
#define CROSS_MARK "\u2717"

// --- Helper Functions ---

// convert string to lowercase
string to_lower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Tokenize and process string for IoU: lowercase and split by space
vector<string> tokenize(const string &s)
{
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (ss >> token)
    {
        tokens.push_back(to_lower(token));
    }
    return tokens;
}

// Calculate Intersection over Union (IoU)
double calculate_iou(const string &s1, const string &s2)
{
    if (s1.empty() || s2.empty())
        return 0.0;

    vector<string> tokens1 = tokenize(s1);
    vector<string> tokens2 = tokenize(s2);

    set<string> set1(tokens1.begin(), tokens1.end());
    set<string> set2(tokens2.begin(), tokens2.end());

    // Calculate Intersection: elements in both sets
    set<string> intersection;
    set_intersection(set1.begin(), set1.end(),
                     set2.begin(), set2.end(),
                     inserter(intersection, intersection.begin()));

    // Calculate Union: elements in either set
    set<string> union_set;
    set_union(set1.begin(), set1.end(),
              set2.begin(), set2.end(),
              inserter(union_set, union_set.begin()));

    // IoU = |Intersection| / |Union|
    if (union_set.empty())
        return 0.0;

    return (double)intersection.size() / union_set.size();
}

// Default constructor for LoanApplication
LoanApplication::LoanApplication()
    : applicationId(""), loanCategory(""), status(""),
      fullName(""), fatherName(""), postalAddress(""), contactNumber(""),
      emailAddress(""), cnicNumber(""), cnicExpiryDate(""), employmentStatus(""),
      maritalStatus(""), gender(""), numDependents(""), annualIncome(""),
      monthlyAvgElectricityBill(""), currentElectricityBill(""), loanActive(""),
      totalLoanAmount(""), amountReturned(""), loanDue(""), bankName(""),
      existingLoanCategory(""), ref1Name(""), ref1Cnic(""), ref1CnicIssueDate(""),
      ref1Phone(""), ref1Email(""), ref2Name(""), ref2Cnic(""), ref2CnicIssueDate(""),
      ref2Phone(""), ref2Email(""), cnicFrontPath(""), cnicBackPath(""),
      electricityBillPath(""), salarySlipPath("") {
    // Constructor body (can be empty)
}
// General Conversation Mode Response Generator
string IoU_response(const string &user_input, const string &corpusFile)
{
    ifstream corpus(corpusFile);
    if (!corpus.is_open())
    {
        // FIX: Use robust string construction for error message
        std::string error_msg = BOLD_RED;
        error_msg += "Error: Could not load conversation data file.";
        error_msg += RESET;
        return error_msg;
    }

    string line;
    // FIX: Use robust string construction for default response
    string best_response = BOLD_YELLOW;
    best_response += "I'm sorry, I don't have a relevant response for that.";
    best_response += RESET;
    double max_iou = 0.0;

    // Loop through the file, reading line by line
    string current_human1_text = "";

    while (getline(corpus, line))
    {

        // Find Human 1:
        size_t human1_pos = line.find("Human 1:");
        if (human1_pos != string::npos)
        {
            current_human1_text = line.substr(human1_pos + 8); // Extract text after "Human 1:"

            // Try to read the immediate next line for Human 2's response
            string next_line;
            if (getline(corpus, next_line))
            {
                size_t human2_pos = next_line.find("Human 2:");
                if (human2_pos != string::npos)
                {
                    string human2_text = next_line.substr(human2_pos + 8); // Extract text after "Human 2:"

                    // Calculate IoU
                    double iou = calculate_iou(user_input, current_human1_text);

                    if (iou > max_iou)
                    {
                        max_iou = iou;
                        best_response = human2_text;
                    }
                }
            }
        }
    }

    // Set a minimum IoU threshold (Changed from 0.2 to 0.15 for better conversational flow)
    if (max_iou < 0.12)
    {
        // FIX: Use robust string construction here as well
        return BOLD_YELLOW + std::string("I'm sorry, I don't have a clear response for that (IoU: ") +
               to_string(max_iou) + ")." + RESET;
    }

    // Output the bot's response
    return GREEN + best_response + RESET;
}

// check if string is numeric
bool isNumber(const string &s)
{
    if (s.empty())
        return false;
    // Check for negative sign only at the beginning
    size_t start = 0;
    if (s[0] == '-')
    {
        start = 1;
    }
    if (start >= s.length())
        return false; // Only a '-' sign is not a valid number

    for (size_t i = start; i < s.length(); ++i)
    {
        if (!std::isdigit(s[i]))
            return false;
    }
    return true;
}

// helper: convert string to double safely (Handles commas)
double to_double(string s)
{
    s.erase(remove(s.begin(), s.end(), ','), s.end());

    double val = 0;
    try
    {
        val = stod(s);
    }
    catch (...)
    {
        return 0;
    }
    return val;
}

// Function to print a horizontal border line
void print_border(int width, const string &color)
{
    cout << color << string(width, '-') << RESET << endl;
}

// Helper function to extract fields for a selected line
vector<string> get_fields_by_line_number(const string &file, int lineNumber)
{
    ifstream out(file);
    vector<string> fields;
    if (!out.is_open())
        return fields;

    string temp;
    string header;
    getline(out, header);
    int count = 0;

    while (getline(out, temp))
    {
        count++;
        if (count == lineNumber)
        {
            stringstream ss(temp);
            string field;
            while (getline(ss, field, '#'))
            {
                fields.push_back(field);
            }
            break;
        }
    }
    return fields;
}

// --- Validation and ID Management ---

bool is13DigitCnic(const string &s)
{
    if (s.length() != 13)
        return false;
    for (char const &c : s)
    {
        if (std::isdigit(c) == 0)
            return false;
    }
    return true;
}

bool is_valid_date(const string &date)
{
    if (date.length() != 10)
        return false;
    if (date[2] != '-' || date[5] != '-')
        return false;

    string d_str = date.substr(0, 2);
    string m_str = date.substr(3, 2);
    string y_str = date.substr(6, 4);

    if (!isNumber(d_str) || !isNumber(m_str) || !isNumber(y_str))
        return false;

    // Basic date logic (DD-MM-YYYY)
    int day = stoi(d_str);
    int month = stoi(m_str);
    int year = stoi(y_str);

    if (day < 1 || day > 31 || month < 1 || month > 12)
        return false;
    if (year < 1900 || year > 2100)
        return false; // Arbitrary year range

    // More accurate day check (simplified, for robust check, proper library or logic is needed)
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;
    if (month == 2)
    {
        bool is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (is_leap && day > 29)
            return false;
        if (!is_leap && day > 28)
            return false;
    }

    return true;
}

bool is_unique_id(const string &id)
{
    ifstream appFile("applications.txt");
    if (!appFile.is_open())
    {
        return true;
    }

    string line;
    LoanApplication tempApp;
    while (getline(appFile, line))
    {
        tempApp.from_string_row(line);
        if (tempApp.applicationId == id)
        {
            appFile.close();
            return false;
        }
    }
    appFile.close();
    return true;
}

// Helper to find the maximum numeric ID currently in use
int get_max_id()
{
    ifstream appFile("applications.txt");
    if (!appFile.is_open())
    {
        return 0; // Start at 1 if file doesn't exist
    }

    string line;
    LoanApplication tempApp;
    int max_id = 0;

    // Skip header
    getline(appFile, line);

    while (getline(appFile, line))
    {
        // Find the first '#' delimiter to isolate the ID field
        size_t hash_pos = line.find('#');
        if (hash_pos != string::npos)
        {
            string id_str = line.substr(0, hash_pos);
            if (isNumber(id_str))
            {
                try
                {
                    int current_id = stoi(id_str);
                    if (current_id > max_id)
                    {
                        max_id = current_id;
                    }
                }
                catch (...)
                {
                    // Ignore conversion errors
                }
            }
        }
    }
    appFile.close();
    return max_id;
}

// Generates a new unique ID by incrementing from the highest existing ID
string generate_unique_id()
{
    // START FIX: Start search from max existing ID + 1
    int start_id = get_max_id() + 1;

    int id_counter = start_id;
    string id;

    // Max 9999 applications
    while (id_counter <= 9999)
    {
        id = to_string(id_counter);
        while (id.length() < 4)
        {
            id = "0" + id;
        }

        // Check uniqueness only if we jumped ahead (to be safe, although sequential guarantees it)
        if (id_counter == start_id || is_unique_id(id))
        {
            return id;
        }
        id_counter++;
    }

    return "FULL";
}

// --- Data Persistence ---

void save_application_to_file(const LoanApplication &app)
{
    ifstream appFileIn("applications.txt");
    vector<string> lines;
    string line;
    bool found = false;
    LoanApplication tempApp;

    const string HEADER_ROW = "ID#Category#Status#FullName#FatherName#PostalAddress#ContactNumber#EmailAddress#CNICNumber#CNICExpiryDate#EmploymentStatus#MaritalStatus#Gender#NumDependents#AnnualIncome#MonthlyAvgElectricityBill#CurrentElectricityBill#LoanActive#TotalLoanAmount#AmountReturned#LoanDue#BankName#ExistingLoanCategory#Ref1Name#Ref1Cnic#Ref1CnicIssueDate#Ref1Phone#Ref1Email#Ref2Name#Ref2Cnic#Ref2CnicIssueDate#Ref2Phone#Ref2Email#CNICFrontPath#CNICBackPath#ElectricityBillPath#SalarySlipPath";

    bool multiple_records_found = false;

    if (appFileIn.is_open())
    {
        bool header_found = false;
        while (getline(appFileIn, line))
        {
            // Check if the line is the expected header (to avoid reading data rows as header)
            if (line.rfind("ID#Category#Status#", 0) == 0 || line == HEADER_ROW)
            {
                lines.push_back(line);
                header_found = true;
                continue;
            }

            // Assuming this is a data row
            tempApp.from_string_row(line);

            // Critical check: only update if the parsed data has a non-empty ID
            if (!tempApp.applicationId.empty() && tempApp.applicationId == app.applicationId)
            {
                if (found)
                {
                    // This means a previous version of this application was already added to 'lines'
                    // This indicates corrupted file state where multiple rows exist for the same ID.
                    multiple_records_found = true;
                    // Do NOT push the old corrupted line to 'lines'. The first instance was already handled/replaced.
                }
                else
                {
                    // We found the original record (or first instance of the ID)
                    // Replace the old record with the NEW, complete application row.
                    lines.push_back(app.to_string_row());
                    found = true;
                }
            }
            else
            {
                // If the IDs don't match, or the old record was corrupted (empty ID), keep the original line
                lines.push_back(line);
            }
        }
        appFileIn.close();

        // FIX: If the file existed but the header was missing, insert it now.
        if (!header_found)
        {
            lines.insert(lines.begin(), HEADER_ROW);
        }
    }
    else
    {
        // If the file does not exist (appFileIn.is_open() failed), start with the header.
        lines.push_back(HEADER_ROW);
    }

    if (!found)
    {
        // Add new application if it wasn't an update (i.e., this is the first time this ID is saved)
        lines.push_back(app.to_string_row());
    }

    if (multiple_records_found)
    {
        // Log a warning about file corruption (only if we were updating, not creating new)
        if (found || !app.applicationId.empty())
        {
            cout << BOLD_RED << "\nWARNING: Multiple rows found for Application ID " << app.applicationId << ". File integrity restored (only the latest state saved)." << RESET << endl;
        }
    }

    // Write all lines back to file (overwriting)
    ofstream appFileOut("applications.txt");
    if (!appFileOut.is_open())
    {
        cout << BOLD_RED << "Error: Could not open applications.txt for saving." << RESET << endl;
        return;
    }
    for (const string &l : lines)
    {
        appFileOut << l << endl;
    }
    appFileOut.close();
}

// FIX: This function now iterates through the entire file and returns the LAST matching record,
// which is the most recently saved state (C2 is saved after C1, etc.).
LoanApplication load_application_by_id(const string &appId)
{
    ifstream appFile("applications.txt");
    LoanApplication app;
    LoanApplication last_found_app; // Store the latest valid version
    bool found_record = false;

    if (!appFile.is_open())
    {
        return app; // Return empty application
    }

    string line;
    // Skip header
    getline(appFile, line);

    while (getline(appFile, line))
    {
        LoanApplication current_app;
        current_app.from_string_row(line);

        if (current_app.applicationId == appId && !current_app.applicationId.empty())
        {
            last_found_app = current_app;
            found_record = true;
        }
    }
    appFile.close();

    if (found_record)
    {
        return last_found_app;
    }

    // If not found, return an empty application
    return LoanApplication();
}

// --- LoanApplication Class Methods (Implementation) ---

string LoanApplication::to_string_row() const
{
    string row = "";
    row += applicationId + "#";
    row += loanCategory + "#";
    row += status + "#";

    // C1
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

    // C2
    row += annualIncome + "#";
    row += monthlyAvgElectricityBill + "#";
    row += currentElectricityBill + "#";
    row += loanActive + "#";
    row += totalLoanAmount + "#";
    row += amountReturned + "#";
    row += loanDue + "#";
    row += bankName + "#";
    row += existingLoanCategory + "#";

    // C3
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

    // C4
    row += cnicFrontPath + "#";
    row += cnicBackPath + "#";
    row += electricityBillPath + "#";
    row += salarySlipPath;

    return row;
}

void LoanApplication::from_string_row(const string &row)
{
    stringstream ss(row);
    string field;
    vector<string> fields;
    while (getline(ss, field, '#'))
    {
        fields.push_back(field);
    }

    // Ensure minimum size for complete parsing of all fields
    if (fields.size() < 37)
    {
        // This likely means a corrupted or incomplete line was read; fail gracefully
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

bool LoanApplication::isC1Complete() const
{
    // Note: status check here is a bit tricky since status is *set* after completion.
    // We check the fields only.
    return !fullName.empty() && !fatherName.empty() && !postalAddress.empty() &&
           isNumber(contactNumber) && !emailAddress.empty() && is13DigitCnic(cnicNumber) &&
           is_valid_date(cnicExpiryDate) && !employmentStatus.empty() && !maritalStatus.empty() &&
           !gender.empty() && isNumber(numDependents);
}
bool LoanApplication::isC2Complete() const
{
    if (!isC1Complete())
        return false;

    bool loanInfoValid = true;
    if (loanActive == "Active" || loanActive == "Not Active")
    {
        loanInfoValid = isNumber(totalLoanAmount) && isNumber(amountReturned) && isNumber(loanDue) &&
                        !bankName.empty() && !existingLoanCategory.empty();
    }
    else if (loanActive != "N/A")
    {
        return false; // Invalid loanActive status if not N/A, Active, or Not Active
    }

    return isNumber(annualIncome) && isNumber(monthlyAvgElectricityBill) &&
           isNumber(currentElectricityBill) && loanInfoValid;
}
bool LoanApplication::isC3Complete() const
{
    if (!isC2Complete())
        return false;
    return !ref1Name.empty() && is13DigitCnic(ref1Cnic) && is_valid_date(ref1CnicIssueDate) &&
           !ref1Phone.empty() && !ref1Email.empty() && !ref2Name.empty() && is13DigitCnic(ref2Cnic) &&
           is_valid_date(ref2CnicIssueDate) && !ref2Phone.empty() && !ref2Email.empty();
}
bool LoanApplication::isC4Complete() const
{
    if (!isC3Complete())
        return false;
    // Note: Paths are mocked/generated in C4 collection, so checking for empty is enough
    return !cnicFrontPath.empty() && !cnicBackPath.empty() &&
           !electricityBillPath.empty() && !salarySlipPath.empty();
}

// --- Loan Option Readers (Copied/Modified from Increment 2) ---

// Car Details: Make#Model#Engine#Used#Year#Installments#Price#Down Payment
string Car_file_reader_details(string file, string user_input)
{
    int lineNumber = stoi(user_input);
    vector<string> fields = get_fields_by_line_number(file, lineNumber);

    if (fields.size() < 8)
    {
        cout << BOLD_RED << " Data corruption or invalid selection for Car." << RESET << endl;
        return "Data corruption.\n";
    }

    string Installments = fields[5];
    string Price = fields[6];
    string DownPayment = fields[7];

    double priceVal = to_double(Price);
    double downVal = to_double(DownPayment);
    int instVal = 0;
    try
    {
        instVal = stoi(Installments);
    }
    catch (...)
    {
    }

    string monthlyStr = " Invalid data for installment calculation.";
    if (instVal > 0 && priceVal >= downVal)
    {
        double monthly = (priceVal - downVal) / instVal;
        monthlyStr = to_string((long long)monthly);
    }

    int box_width = 70;
    print_border(box_width, YELLOW);
    cout << BOLD_CYAN << setw(box_width / 2 + 20) << " Selected Car Loan Details " << RESET << endl;
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

// Scooter Details: Make#Model#Distance#Time#Speed#Installments#Price#Down Payment
string Scooter_file_reader_details(string file, string user_input)
{
    int lineNumber = stoi(user_input);
    vector<string> fields = get_fields_by_line_number(file, lineNumber);

    if (fields.size() < 8)
    {
        cout << BOLD_RED << " Data corruption or invalid selection for Scooter." << RESET << endl;
        return "Data corruption.\n";
    }

    string Installments = fields[5];
    string Price = fields[6];
    string DownPayment = fields[7];

    double priceVal = to_double(Price);
    double downVal = to_double(DownPayment);
    int instVal = 0;
    try
    {
        instVal = stoi(Installments);
    }
    catch (...)
    {
    }

    string monthlyStr = " Invalid data for installment calculation.";
    if (instVal > 0 && priceVal >= downVal)
    {
        double monthly = (priceVal - downVal) / instVal;
        monthlyStr = to_string((long long)monthly);
    }

    int box_width = 70;
    print_border(box_width, YELLOW);
    cout << BOLD_CYAN << setw(box_width / 2 + 20) << " Selected Scooter Loan Details " << RESET << endl;
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
string generic_loan_file_reader_options(string file, const vector<string> &headers, const vector<int> &col_widths)
{
    ifstream out(file);
    string temp;
    string header_line;
    getline(out, header_line);
    int line_count = 0;

    int total_width = 0;
    for (int w : col_widths)
        total_width += w;

    print_border(total_width, YELLOW);

    cout << BOLD_YELLOW << setw(col_widths[0]) << left << "#";

    for (size_t i = 0; i < headers.size(); ++i)
    {
        cout << setw(col_widths[i + 1]) << headers[i];
    }
    cout << RESET << endl;
    print_border(total_width, YELLOW);

    while (getline(out, temp))
    {
        line_count++;
        stringstream ss(temp);
        string field;
        vector<string> fields;
        while (getline(ss, field, '#'))
        {
            fields.push_back(field);
        }

        // Ensure there are enough fields to fill all header columns
        if (fields.size() >= headers.size())
        {
            cout << CYAN << setw(col_widths[0]) << left << to_string(line_count);
            for (size_t i = 0; i < headers.size(); ++i)
            {
                // Ensure we don't read past the available fields
                if (i < fields.size())
                {
                    cout << setw(col_widths[i + 1]) << fields[i];
                }
                else
                {
                    cout << setw(col_widths[i + 1]) << "";
                }
            }
            cout << RESET << endl;
        }
    }
    print_border(total_width, YELLOW);
    return "";
}

// Specific reader for Scooter.txt (Routes to display options or details)
string Scooter_file_reader(string user_input, bool show_all)
{
    vector<string> headers = {"Make", "Model", "Dist(KM)", "Time(Hrs)", "Speed(KM/H)", "Installments", "Price(PKR)", "Down Payment(PKR)"};
    vector<int> col_widths = {6, 15, 12, 12, 12, 14, 15, 18, 18};
    if (show_all)
    {
        return generic_loan_file_reader_options("Scooter.txt", headers, col_widths);
    }
    else if (isNumber(user_input))
    {
        return Scooter_file_reader_details("Scooter.txt", user_input);
    }
    return "";
}

// Specific reader for Car.txt (Routes to display options or details)
string Car_file_reader(string user_input, bool show_all)
{
    vector<string> headers = {"Make", "Model", "Engine(cc)", "Used", "Year", "Installments", "Price(PKR)", "Down Payment(PKR)"};
    vector<int> col_widths = {6, 12, 12, 10, 8, 14, 15, 18, 18};
    if (show_all)
    {
        return generic_loan_file_reader_options("Car.txt", headers, col_widths);
    }
    else if (isNumber(user_input))
    {
        return Car_file_reader_details("Car.txt", user_input);
    }
    return "";
}

// reads Home.txt and prints selected details + calculates monthly installment with styling
string Home_file_reader(string file, string user_input, bool show_all)
{
    ifstream out(file);
    if (!out.is_open())
    {
        cout << BOLD_RED << " Could not open Home file." << RESET << endl;
        return " Could not open Home file.\n";
    }
    string temp;
    string header;
    getline(out, header);
    int line_count = 0;

    int col_width = 20;

    if (show_all)
    {
        print_border(6 * col_width + 5, YELLOW);
        cout << BOLD_YELLOW << setw(col_width) << left << "#"
             << setw(col_width) << "Area"
             << setw(col_width) << "Size"
             << setw(col_width) << "Installments"
             << setw(col_width) << "Price(PKR)"
             << setw(col_width) << "Down Payment(PKR)" << RESET << endl;
        print_border(6 * col_width + 5, YELLOW);

        while (getline(out, temp))
        {
            line_count++;
            stringstream ss(temp);
            string field;
            vector<string> fields;
            while (getline(ss, field, '#'))
            {
                fields.push_back(field);
            }

            if (fields.size() >= 5)
            {
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
    if (isNumber(user_input))
    {
        int lineNumber = stoi(user_input);
        vector<string> fields = get_fields_by_line_number(file, lineNumber);

        if (fields.size() < 5)
        {
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
        try
        {
            instVal = stoi(Installments);
        }
        catch (...)
        {
        }

        string monthlyStr = " Invalid data for installment calculation.";

        if (instVal > 0 && priceVal >= downVal)
        {
            double monthly = (priceVal - downVal) / instVal;
            monthlyStr = to_string((long long)monthly);
        }

        int box_width = 60;
        print_border(box_width, YELLOW);
        cout << BOLD_CYAN << setw(box_width / 2 + 15) << " Selected Home Loan Details " << RESET << endl;
        print_border(box_width, YELLOW);
        cout << CYAN << "Area: " << BOLD << Area << RESET << endl;
        cout << CYAN << "Size: " << BOLD << Size << RESET << endl;
        cout << CYAN << "Installments: " << BOLD << Installments << RESET << endl;
        cout << CYAN << "Price : " << BOLD << Price << " PKR" << RESET << endl;
        cout << CYAN << "Down Payment : " << BOLD << DownPayment << " PKR" << RESET << endl;
        cout << GREEN << "Monthly Installment : " << BOLD << monthlyStr << " PKR" << RESET << endl;
        print_border(box_width, YELLOW);

        return "";
    }
    else
    {
        return "Invalid input for Home_file_reader.\n";
    }
}

// Dummy file reader for general greetings
string file_reader(string file, string user_input, bool show_all)
{
    if (file == "Utterances.txt")
    {
        // In Increment 3, general chat mode replaces fixed Utterances.txt responses
        if (to_lower(user_input) == "hi" || to_lower(user_input) == "hello" || to_lower(user_input) == "salam" || to_lower(user_input) == "aoa")
        {
            cout << GREEN << "Hello! I am Loan-Bot. How can I assist you today?" << RESET << endl;
        }
        else if (to_lower(user_input) == "a")
        {
            cout << BOLD_GREEN << "Initiating Loan Application Process..." << RESET << endl;
        }
        return "";
    }
    else if (file == "Home.txt")
    {
        return Home_file_reader(file, user_input, show_all);
    }
    else if (file == "Car.txt")
    {
        return Car_file_reader(user_input, show_all);
    }
    else if (file == "Scooter.txt")
    {
        return Scooter_file_reader(user_input, show_all);
    }
    return "Unknown file type.";
}

// --- Application Checkpoint Collection Functions (Increment 3) ---

string collect_personal_info(LoanApplication &app)
{
    if (app.status == "Submitted" || app.status == "Approved" || app.status == "Rejected")
    {
        cout << BOLD_RED << "Application already submitted or processed. Cannot modify." << RESET << endl;
        return app.status;
    }

    string userInput;
    cout << BOLD_CYAN << "\n[C1/C4] Personal Information" << RESET << endl;
    print_border(50, CYAN);

    cout << CYAN << "Full Name (" << (app.fullName.empty() ? "N/A" : app.fullName) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.fullName = userInput;
    cout << CYAN << "Father's Name (" << (app.fatherName.empty() ? "N/A" : app.fatherName) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.fatherName = userInput;

    cout << CYAN << "Postal Address (" << (app.postalAddress.empty() ? "N/A" : app.postalAddress) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.postalAddress = userInput;

    // Contact Number (Validation)
    while (true)
    {
        cout << CYAN << "Contact Number (e.g., 03331234567) (" << (app.contactNumber.empty() ? "N/A" : app.contactNumber) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.contactNumber = userInput;
        if (isNumber(app.contactNumber) && !app.contactNumber.empty())
        {
            cout << CHECK_MARK << " Contact number accepted." << endl;
            break;
        }
        if (!app.contactNumber.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid input! Must be numeric." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    cout << CYAN << "Email Address (" << (app.emailAddress.empty() ? "N/A" : app.emailAddress) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.emailAddress = userInput;

    // CNIC Number (13-digit Validation)
    while (true)
    {
        cout << CYAN << "CNIC Number (13 digits, no dashes) (" << (app.cnicNumber.empty() ? "N/A" : app.cnicNumber) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.cnicNumber = userInput;
        if (is13DigitCnic(app.cnicNumber))
        {
            cout << CHECK_MARK << " CNIC number validated." << endl;
            break;
        }
        if (!app.cnicNumber.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid input! CNIC must be exactly 13 numeric digits." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    // CNIC Expiry Date (Validation)
    while (true)
    {
        cout << CYAN << "CNIC Expiry Date (DD-MM-YYYY) (" << (app.cnicExpiryDate.empty() ? "N/A" : app.cnicExpiryDate) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.cnicExpiryDate = userInput;
        if (is_valid_date(app.cnicExpiryDate))
        {
            cout << CHECK_MARK << " Expiry date accepted." << endl;
            break;
        }
        if (!app.cnicExpiryDate.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid format or date! Please use DD-MM-YYYY." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    // Employment Status (MENU SELECTION)
    string empOptions = "1. Self-employed\n2. Salaried\n3. Retired\n4. Unemployed";
    while (true)
    {
        string current_val = app.employmentStatus.empty() ? "N/A" : app.employmentStatus;
        cout << CYAN << "\nSelect Employment Status (" << current_val << "): \n"
             << empOptions << endl;
        cout << BOLD_GREEN << "Enter option (1-4): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
        {
            if (userInput == "1")
            {
                app.employmentStatus = "Self-employed";
                break;
            }
            if (userInput == "2")
            {
                app.employmentStatus = "Salaried";
                break;
            }
            if (userInput == "3")
            {
                app.employmentStatus = "Retired";
                break;
            }
            if (userInput == "4")
            {
                app.employmentStatus = "Unemployed";
                break;
            }
            cout << CROSS_MARK << BOLD_RED << " Invalid option! Please enter 1, 2, 3, or 4." << RESET << endl;
        }
        else if (!app.employmentStatus.empty())
        {
            // If already set and user presses Enter, keep existing value
            break;
        }
        else
        {
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
        }
    }
    cout << CHECK_MARK << " Employment status recorded: " << app.employmentStatus << endl;

    // Marital Status (MENU SELECTION)
    string maritalOptions = "1. Single\n2. Married\n3. Divorced\n4. Widowed";
    while (true)
    {
        string current_val = app.maritalStatus.empty() ? "N/A" : app.maritalStatus;
        cout << CYAN << "\nSelect Marital Status (" << current_val << "): \n"
             << maritalOptions << endl;
        cout << BOLD_GREEN << "Enter option (1-4): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
        {
            if (userInput == "1")
            {
                app.maritalStatus = "Single";
                break;
            }
            if (userInput == "2")
            {
                app.maritalStatus = "Married";
                break;
            }
            if (userInput == "3")
            {
                app.maritalStatus = "Divorced";
                break;
            }
            if (userInput == "4")
            {
                app.maritalStatus = "Widowed";
                break;
            }
            cout << CROSS_MARK << BOLD_RED << " Invalid option! Please enter 1, 2, 3, or 4." << RESET << endl;
        }
        else if (!app.maritalStatus.empty())
        {
            // If already set and user presses Enter, keep existing value
            break;
        }
        else
        {
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
        }
    }
    cout << CHECK_MARK << " Marital status recorded: " << app.maritalStatus << endl;

    // Gender (MENU SELECTION)
    string genderOptions = "1. Male\n2. Female\n3. Other";
    while (true)
    {
        string current_val = app.gender.empty() ? "N/A" : app.gender;
        cout << CYAN << "\nSelect Gender (" << current_val << "): \n"
             << genderOptions << endl;
        cout << BOLD_GREEN << "Enter option (1-3): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
        {
            if (userInput == "1")
            {
                app.gender = "Male";
                break;
            }
            if (userInput == "2")
            {
                app.gender = "Female";
                break;
            }
            if (userInput == "3")
            {
                app.gender = "Other";
                break;
            }
            cout << CROSS_MARK << BOLD_RED << " Invalid option! Please enter 1, 2, or 3." << RESET << endl;
        }
        else if (!app.gender.empty())
        {
            // If already set and user presses Enter, keep existing value
            break;
        }
        else
        {
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
        }
    }
    cout << CHECK_MARK << " Gender recorded: " << app.gender << endl;

    // Number of Dependents (Validation)
    while (true)
    {
        cout << CYAN << "Number of dependents (numeric) (" << (app.numDependents.empty() ? "N/A" : app.numDependents) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.numDependents = userInput;
        if (isNumber(app.numDependents) && !app.numDependents.empty())
        {
            cout << CHECK_MARK << " Dependents count accepted." << endl;
            break;
        }
        if (!app.numDependents.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid input! Must be numeric." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    app.status = "C1";
    app.annualIncome = "0";
    app.monthlyAvgElectricityBill = "0";
    app.currentElectricityBill = "0";
    app.loanActive = "N/A";
    app.totalLoanAmount = "0";
    app.amountReturned = "0";
    app.loanDue = "0";
    app.bankName = "N/A";
    app.existingLoanCategory = "N/A";
    app.ref1Name = "N/A";
    app.ref1Cnic = "0000000000000";
    app.ref1CnicIssueDate = " 00-00-0000";
    app.ref1Phone = "0";
    app.ref1Email = "N/A";
    app.ref2Name = "N/A";
    app.ref2Cnic = "0000000000000";
    app.ref2CnicIssueDate = "00-00-0000";
    app.ref2Phone = "0";
    app.ref2Email = "N/A";
    app.cnicFrontPath = "N/A";
    app.cnicBackPath = "N/A";
    app.electricityBillPath = "N/A";
    app.salarySlipPath = "N/A";
    save_application_to_file(app);

    return "C1";
}

string collect_financial_info(LoanApplication &app)
{
    if (app.status == "Submitted" || app.status == "Approved" || app.status == "Rejected")
    {
        cout << BOLD_RED << "Application already submitted or processed. Cannot modify." << RESET << endl;
        return app.status;
    }

    string userInput;
    cout << BOLD_CYAN << "\n[C2/C4] Financial Information" << RESET << endl;
    print_border(50, CYAN);

    // Income and Bills (Validation)
    while (true)
    {
        cout << CYAN << "Annual income in PKR (numeric, no commas) (" << (app.annualIncome.empty() ? "N/A" : app.annualIncome) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.annualIncome = userInput;
        if (isNumber(app.annualIncome) && !app.annualIncome.empty())
        {
            cout << CHECK_MARK << " Income accepted." << endl;
            break;
        }
        if (!app.annualIncome.empty())
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric without commas." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }
    while (true)
    {
        cout << CYAN << "Monthly avg electricity bill for last 12 months (PKR, numeric) (" << (app.monthlyAvgElectricityBill.empty() ? "N/A" : app.monthlyAvgElectricityBill) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.monthlyAvgElectricityBill = userInput;
        if (isNumber(app.monthlyAvgElectricityBill) && !app.monthlyAvgElectricityBill.empty())
        {
            cout << CHECK_MARK << " Average bill accepted." << endl;
            break;
        }
        if (!app.monthlyAvgElectricityBill.empty())
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric without commas." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }
    while (true)
    {
        cout << CYAN << "Exact amount of current electricity bill (PKR, numeric) (" << (app.currentElectricityBill.empty() ? "N/A" : app.currentElectricityBill) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.currentElectricityBill = userInput;
        if (isNumber(app.currentElectricityBill) && !app.currentElectricityBill.empty())
        {
            cout << CHECK_MARK << " Current bill accepted." << endl;
            break;
        }
        if (!app.currentElectricityBill.empty())
            cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric without commas." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    // Existing Loan Details
    string hasLoan;
    if (app.loanActive == "Active" || app.loanActive == "Not Active")
        hasLoan = "y";
    else if (app.loanActive == "N/A" || app.loanActive.empty())
        hasLoan = "n";
    else
        hasLoan = ""; // Invalid state

    while (true)
    {
        string current_prompt = (hasLoan == "y") ? "Y" : ((hasLoan == "n") ? "N" : "N/A");
        cout << BOLD_GREEN << "\nDo you have any existing loans (Y/N)? (" << current_prompt << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            hasLoan = to_lower(userInput);
        if (hasLoan == "y" || hasLoan == "n")
            break;
        cout << CROSS_MARK << BOLD_RED << " Invalid input! Please enter Y (Yes) or N (No)." << RESET << endl;
    }
    cout << CHECK_MARK << " Existing loan status: " << (hasLoan == "y" ? "Yes" : "No") << endl;

    if (hasLoan == "y")
    {
        string loanActiveOption;
        // Loan period Active or not (MENU SELECTION)
        while (true)
        {
            string current_val = app.loanActive.empty() ? "N/A" : app.loanActive;
            cout << CYAN << "\nLoan period Status (" << current_val << "): " << endl;
            cout << "1. Active\n2. Not Active" << endl;
            cout << BOLD_GREEN << "Enter option (1 or 2): " << RESET;
            getline(cin, loanActiveOption);

            if (!loanActiveOption.empty())
            {
                if (loanActiveOption == "1")
                {
                    app.loanActive = "Active";
                    break;
                }
                if (loanActiveOption == "2")
                {
                    app.loanActive = "Not Active";
                    break;
                }
                cout << CROSS_MARK << BOLD_RED << "Invalid input! Enter 1 or 2." << RESET << endl;
            }
            else if (!app.loanActive.empty() && app.loanActive != "N/A")
            {
                break;
            }
            else
            {
                cout << BOLD_YELLOW << "Field required." << RESET << endl;
            }
        }
        cout << CHECK_MARK << " Loan period recorded: " << app.loanActive << endl;

        while (true)
        {
            cout << CYAN << "Total Loan Amount (PKR, numeric) (" << (app.totalLoanAmount.empty() ? "N/A" : app.totalLoanAmount) << "): " << RESET;
            getline(cin, userInput);
            if (!userInput.empty())
                app.totalLoanAmount = userInput;
            if (isNumber(app.totalLoanAmount) && !app.totalLoanAmount.empty())
            {
                cout << CHECK_MARK << " Amount accepted." << endl;
                break;
            }
            if (!app.totalLoanAmount.empty())
                cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric." << RESET << endl;
            else
                cout << BOLD_YELLOW << "Field required." << RESET << endl;
        }

        while (true)
        {
            cout << CYAN << "Amount Returned (PKR, numeric) (" << (app.amountReturned.empty() ? "N/A" : app.amountReturned) << "): " << RESET;
            getline(cin, userInput);
            if (!userInput.empty())
                app.amountReturned = userInput;
            if (isNumber(app.amountReturned) && !app.amountReturned.empty())
            {
                cout << CHECK_MARK << " Amount accepted." << endl;
                break;
            }
            if (!app.amountReturned.empty())
                cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric." << RESET << endl;
            else
                cout << BOLD_YELLOW << "Field required." << RESET << endl;
        }

        while (true)
        {
            cout << CYAN << "Loan Amount Still Due (PKR, numeric) (" << (app.loanDue.empty() ? "N/A" : app.loanDue) << "): " << RESET;
            getline(cin, userInput);
            if (!userInput.empty())
                app.loanDue = userInput;
            if (isNumber(app.loanDue) && !app.loanDue.empty())
            {
                cout << CHECK_MARK << " Amount accepted." << endl;
                break;
            }
            if (!app.loanDue.empty())
                cout << CROSS_MARK << BOLD_RED << "Invalid input! Must be numeric." << RESET << endl;
            else
                cout << BOLD_YELLOW << "Field required." << RESET << endl;
        }

        cout << CYAN << "Bank Name (" << (app.bankName.empty() ? "N/A" : app.bankName) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.bankName = userInput;

        cout << CYAN << "Existing Loan Category (Car, Home, Bike, Personal) (" << (app.existingLoanCategory.empty() ? "N/A" : app.existingLoanCategory) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.existingLoanCategory = userInput;
    }
    else
    {
        app.loanActive = "N/A";
        app.totalLoanAmount = "0";
        app.amountReturned = "0";
        app.loanDue = "0";
        app.bankName = "N/A";
        app.existingLoanCategory = "N/A";
    }
  app.ref1Name = "N/A";
    app.ref1Cnic = "0000000000000";
    app.ref1CnicIssueDate = " 00-00-0000";
    app.ref1Phone = "0";
    app.ref1Email = "N/A";
    app.ref2Name = "N/A";
    app.ref2Cnic = "0000000000000";
    app.ref2CnicIssueDate = "00-00-0000";
    app.ref2Phone = "0";
    app.ref2Email = "N/A";
    app.cnicFrontPath = "N/A";
    app.cnicBackPath = "N/A";
    app.electricityBillPath = "N/A";
    app.salarySlipPath = "N/A";
    app.status = "C2";
    save_application_to_file(app);
    return "C2";
}

string collect_references(LoanApplication &app)
{
    if (app.status == "Submitted" || app.status == "Approved" || app.status == "Rejected")
    {
        cout << BOLD_RED << "Application already submitted or processed. Cannot modify." << RESET << endl;
        return app.status;
    }

    string userInput;
    cout << BOLD_CYAN << "\n[C3/C4] References" << RESET << endl;
    print_border(50, CYAN);

    // Referee 1
    cout << CYAN << "\n-- Referee 1 --" << RESET << endl;
    cout << CYAN << "Name of Referee 1 (" << (app.ref1Name.empty() ? "N/A" : app.ref1Name) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.ref1Name = userInput;

    while (true)
    {
        cout << CYAN << "CNIC of Referee 1 (13 digits) (" << (app.ref1Cnic.empty() ? "N/A" : app.ref1Cnic) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.ref1Cnic = userInput;
        if (is13DigitCnic(app.ref1Cnic))
        {
            cout << CHECK_MARK << " Referee CNIC validated." << endl;
            break;
        }
        if (!app.ref1Cnic.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid input! CNIC must be exactly 13 numeric digits." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    while (true)
    {
        cout << CYAN << "Issue date of CNIC of Referee 1 (DD-MM-YYYY) (" << (app.ref1CnicIssueDate.empty() ? "N/A" : app.ref1CnicIssueDate) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.ref1CnicIssueDate = userInput;
        if (is_valid_date(app.ref1CnicIssueDate))
        {
            cout << CHECK_MARK << " Issue date accepted." << endl;
            break;
        }
        if (!app.ref1CnicIssueDate.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid format or date! Please use DD-MM-YYYY." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }
    cout << CYAN << "Phone number of Referee 1 (" << (app.ref1Phone.empty() ? "N/A" : app.ref1Phone) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.ref1Phone = userInput;

    cout << CYAN << "Email address of Referee 1 (" << (app.ref1Email.empty() ? "N/A" : app.ref1Email) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.ref1Email = userInput;

    // Referee 2
    cout << CYAN << "\n-- Referee 2 --" << RESET << endl;
    cout << CYAN << "Name of Referee 2 (" << (app.ref2Name.empty() ? "N/A" : app.ref2Name) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.ref2Name = userInput;

    while (true)
    {
        cout << CYAN << "CNIC of Referee 2 (13 digits) (" << (app.ref2Cnic.empty() ? "N/A" : app.ref2Cnic) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.ref2Cnic = userInput;
        if (is13DigitCnic(app.ref2Cnic))
        {
            cout << CHECK_MARK << " Referee CNIC validated." << endl;
            break;
        }
        if (!app.ref2Cnic.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid input! CNIC must be exactly 13 numeric digits." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }

    while (true)
    {
        cout << CYAN << "Issue date of CNIC of Referee 2 (DD-MM-YYYY) (" << (app.ref2CnicIssueDate.empty() ? "N/A" : app.ref2CnicIssueDate) << "): " << RESET;
        getline(cin, userInput);
        if (!userInput.empty())
            app.ref2CnicIssueDate = userInput;
        if (is_valid_date(app.ref2CnicIssueDate))
        {
            cout << CHECK_MARK << " Issue date accepted." << endl;
            break;
        }
        if (!app.ref2CnicIssueDate.empty())
            cout << CROSS_MARK << BOLD_RED << " Invalid format or date! Please use DD-MM-YYYY." << RESET << endl;
        else
            cout << BOLD_YELLOW << "Field required." << RESET << endl;
    }
    cout << CYAN << "Phone number of Referee 2 (" << (app.ref2Phone.empty() ? "N/A" : app.ref2Phone) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.ref2Phone = userInput;

    cout << CYAN << "Email address of Referee 2 (" << (app.ref2Email.empty() ? "N/A" : app.ref2Email) << "): " << RESET;
    getline(cin, userInput);
    if (!userInput.empty())
        app.ref2Email = userInput;
 
    app.cnicFrontPath = "N/A";
    app.cnicBackPath = "N/A";
    app.electricityBillPath = "N/A";
    app.salarySlipPath = "N/A";
    app.status = "C3";
    save_application_to_file(app);
    return "C3";
}

string collect_documents(LoanApplication &app)
{
    if (app.status == "Submitted" || app.status == "Approved" || app.status == "Rejected")
    {
        cout << BOLD_RED << "Application already submitted or processed. Cannot modify." << RESET << endl;
        return app.status;
    }

    cout << BOLD_CYAN << "\n[C4/C4] Document Upload" << RESET << endl;
    print_border(50, CYAN);

    // Create base directory: ./data/<ApplicationID>/
    string destDir = "./data/" + app.applicationId + "/";
    try
    {
        filesystem::create_directories(destDir);
    }
    catch (const filesystem::filesystem_error &e)
    {
        cout << BOLD_RED << "Error creating directory: " << e.what() << RESET << endl;
        return app.status;
    }

    // Helper lambda to copy file
    auto copyFile = [&](const string &srcPath, const string &destFileName) -> string
    {
        if (srcPath.empty())
        {
            cout << BOLD_YELLOW << "Skipped: No path provided." << RESET << endl;
            return "N/A";
        }

        // Check if source file exists
        ifstream testFile(srcPath);
        if (!testFile)
        {
            cout << CROSS_MARK << BOLD_RED << " File not found: " << srcPath << RESET << endl;
            return "INVALID";
        }
        testFile.close();

        string destPath = destDir + destFileName;
        try
        {
            filesystem::copy_file(srcPath, destPath, filesystem::copy_options::overwrite_existing);
            cout << CHECK_MARK << " Saved as: " << destPath << RESET << endl;
            return destPath;
        }
        catch (const filesystem::filesystem_error &e)
        {
            cout << CROSS_MARK << BOLD_RED << " Copy failed: " << e.what() << RESET << endl;
            return "INVALID";
        }
    };

    // --- CNIC Front ---
    string userInput;
    cout << CYAN << "\n[1/4] Enter full path to CNIC Front image (e.g., C:\\docs\\cnic_f.jpg or /home/user/cnic_f.jpg): " << RESET;
    getline(cin, userInput);
    string frontPath = copyFile(userInput, "cnic_front.jpg");
    if (frontPath != "INVALID" && frontPath != "N/A")
        app.cnicFrontPath = frontPath;

    // --- CNIC Back ---
    cout << CYAN << "\n[2/4] Enter full path to CNIC Back image: " << RESET;
    getline(cin, userInput);
    string backPath = copyFile(userInput, "cnic_back.jpg");
    if (backPath != "INVALID" && backPath != "N/A")
        app.cnicBackPath = backPath;

    // --- Electricity Bill ---
    cout << CYAN << "\n[3/4] Enter full path to recent Electricity Bill image: " << RESET;
    getline(cin, userInput);
    string billPath = copyFile(userInput, "electricity_bill.jpg");
    if (billPath != "INVALID" && billPath != "N/A")
        app.electricityBillPath = billPath;

    // --- Salary Slip / Bank Statement ---
    cout << CYAN << "\n[4/4] Enter full path to Salary Slip or Bank Statement (page 1): " << RESET;
    getline(cin, userInput);
    string slipPath = copyFile(userInput, "salary_slip.jpg");
    if (slipPath != "INVALID" && slipPath != "N/A")
        app.salarySlipPath = slipPath;

    // Validate all documents are provided
    if (app.cnicFrontPath == "N/A" || app.cnicFrontPath == "INVALID" ||
        app.cnicBackPath == "N/A" || app.cnicBackPath == "INVALID" ||
        app.electricityBillPath == "N/A" || app.electricityBillPath == "INVALID" ||
        app.salarySlipPath == "N/A" || app.salarySlipPath == "INVALID")
    {
        cout << BOLD_RED << "\n" << CROSS_MARK << " Missing or invalid documents. Application cannot proceed to C4." << RESET << endl;
        app.status = app.status.empty() ? "C3" : app.status; // Revert or keep at C3
        save_application_to_file(app);
        return app.status;
    }

    app.status = "C4";
    save_application_to_file(app);
    cout << BOLD_GREEN << "\n" << CHECK_MARK << " All documents uploaded and saved successfully!" << RESET << endl;
    return "C4";
}

string collect_application_data(const string &category, const string &existingId, const string &existingCnic)
{
    LoanApplication app;
    string startStatus = "";

    if (!existingId.empty())
    {
        app = load_application_by_id(existingId);
        if (app.applicationId != existingId)
        {
            cout << BOLD_RED << "Error: Application ID " << existingId << " not found." << RESET << endl;
            return "";
        }
        if (app.cnicNumber != existingCnic)
        {
            cout << BOLD_RED << "Error: CNIC does not match Application ID " << existingId << "." << RESET << endl;
            return "";
        }
        if (app.status == "Submitted" || app.status == "Approved" || app.status == "Rejected")
        {
            cout << BOLD_RED << "Application is " << app.status << ". Cannot be modified." << RESET << endl;
            return "";
        }
        startStatus = app.status;
        cout << BOLD_GREEN << "\n--- Resuming Loan Application (ID: " << app.applicationId << ", Status: " << app.status << ") ---" << RESET << endl;
    }
    else
    {
        string appId = generate_unique_id();
        if (appId == "FULL")
        {
            cout << BOLD_RED << "\n"
                 << CROSS_MARK << " Error: Maximum application limit reached (9999). Cannot submit new application." << RESET << endl;
            return "";
        }
        app.loanCategory = category;
        app.applicationId = appId;
        app.status = "";
        cout << BOLD_GREEN << "\n--- Starting New Loan Application (ID: " << app.applicationId << ", Category: " << app.loanCategory << ") ---" << RESET << endl;
    }

    print_border(80, GREEN);

    string currentStatus = app.status;
    string prevStatus = "";

    // Only show Application ID for new applications at start
    if (currentStatus.empty() && existingId.empty())
    {
        cout << BOLD_YELLOW << "\n"
             << CHECK_MARK << " IMPORTANT: Your Application ID is " << app.applicationId << ". Please note this down to resume later!" << RESET << endl;
        print_border(80, YELLOW);
    }

    // 1. Personal Information (C1)
    if (currentStatus.empty())
    {
        currentStatus = collect_personal_info(app);
    }
    prevStatus = currentStatus;

    // Checkpoint 1 Save Notification
    if (currentStatus == "C1")
    {
        cout << BOLD_GREEN << "\n"
             << CHECK_MARK << " Personal Information (C1) saved successfully." << RESET << endl;
        cout << BOLD_YELLOW << "Your Application ID is: " << app.applicationId << RESET << endl;
        print_border(50, YELLOW);

        // CONFIRMATION PROMPT FOR NEXT CHECKPOINT
        string confirm;
        cout << BOLD_CYAN << "\nProceed to Financial Information (C2)? (Y/N): " << RESET;
        getline(cin, confirm);
        if (to_lower(confirm) != "y")
        {
            cout << BOLD_YELLOW << "\nApplication saved at checkpoint C1." << endl;
            cout << "Resume later using Application ID: " << app.applicationId << RESET << endl;
            return "";
        }
    }

    // 2. Financial Information (C2)
    if (prevStatus == "C1")
    {
        currentStatus = collect_financial_info(app);
    }
    prevStatus = currentStatus;

    // Checkpoint 2 Save Notification
    if (currentStatus == "C2")
    {
        cout << BOLD_GREEN << "\n"
             << CHECK_MARK << " Financial Information (C2) saved successfully." << RESET << endl;
        cout << BOLD_YELLOW << "Your Application ID is: " << app.applicationId << RESET << endl;
        print_border(50, YELLOW);

        // CONFIRMATION PROMPT FOR NEXT CHECKPOINT
        string confirm;
        cout << BOLD_CYAN << "\nProceed to References (C3)? (Y/N): " << RESET;
        getline(cin, confirm);
        if (to_lower(confirm) != "y")
        {
            cout << BOLD_YELLOW << "\nApplication saved at checkpoint C2." << endl;
            cout << "Resume later using Application ID: " << app.applicationId << RESET << endl;
            return "";
        }
    }

    // 3. References (C3)
    if (prevStatus == "C2")
    {
        currentStatus = collect_references(app);
    }
    prevStatus = currentStatus;

    // Checkpoint 3 Save Notification
    if (currentStatus == "C3")
    {
        cout << BOLD_GREEN << "\n"
             << CHECK_MARK << " References (C3) saved successfully." << RESET << endl;
        cout << BOLD_YELLOW << "Your Application ID is: " << app.applicationId << RESET << endl;
        print_border(50, YELLOW);

        // CONFIRMATION PROMPT FOR NEXT CHECKPOINT
        string confirm;
        cout << BOLD_CYAN << "\nProceed to Documents (C4)? (Y/N): " << RESET;
        getline(cin, confirm);
        if (to_lower(confirm) != "y")
        {
            cout << BOLD_YELLOW << "\nApplication saved at checkpoint C3." << endl;
            cout << "Resume later using Application ID: " << app.applicationId << RESET << endl;
            return "";
        }
    }

    // 4. Documents (C4)
    if (prevStatus == "C3")
    {
        currentStatus = collect_documents(app);
    }

    // --- Final Submission ---
    if (currentStatus == "C4" && app.isC4Complete())
    {
        print_border(80, GREEN);
        cout << BOLD_YELLOW << "\nConfirmation: Application Complete (C4)" << RESET << endl;

        string confirm;
        while (true)
        {
            cout << BOLD_GREEN << "Do you confirm and want to submit this completed application (Y/N/X)? " << RESET;
            getline(cin, confirm);

            string lower_confirm = to_lower(confirm);

            if (lower_confirm == "x")
            {
                cout << BOLD_RED << "\nSubmission postponed. Application saved as C4." << RESET << endl;
                cout << BOLD_YELLOW << "Your Application ID " << app.applicationId << " is saved at checkpoint C4. You can resume later." << RESET << endl;
                return "";
            }

            if (lower_confirm == "y")
            {
                app.status = "Submitted";
                save_application_to_file(app);
                cout << BOLD_GREEN << "\n"
                     << CHECK_MARK << " Application ID " << app.applicationId << " successfully submitted! Status: Submitted." << RESET << endl;
                return app.applicationId;
            }
            else if (lower_confirm == "n")
            {
                cout << BOLD_YELLOW << "\nApplication saved as C4, but not formally submitted. You can resume and submit later." << RESET << endl;
                cout << BOLD_YELLOW << "Your Application ID " << app.applicationId << " is saved at checkpoint C4. You can resume later." << RESET << endl;
                return "";
            }
            else
            {
                cout << CROSS_MARK << BOLD_RED << " Invalid choice. Please enter Y (Yes), N (No), or X (Exit)." << RESET << endl;
            }
        }
    }
    else
    {
        // Application interrupted or left incomplete
        cout << BOLD_YELLOW << "\nApplication saved at checkpoint " << app.status << ". You can resume later with your Application ID: " << app.applicationId << RESET << endl;
    }

    return "";

}
// --- Lender Interface Functions ---

void viewApplications()
{
    ifstream appFile("applications.txt");
    if (!appFile.is_open())
    {
        cout << BOLD_RED << "\nNo applications file found or could not open applications.txt." << RESET << endl;
        return;
    }

    string line;
    // Skip header line
    getline(appFile, line);

    int col_width = 18;
    cout << BOLD_MAGENTA << "\n--- Loan Applications List ---" << RESET << endl;
    print_border(6 * col_width + 5, MAGENTA);
    cout << BOLD_MAGENTA << setw(col_width) << left << "ID"
         << setw(col_width) << "Status"
         << setw(col_width) << "Category"
         << setw(col_width) << "Full Name"
         << setw(col_width) << "CNIC"
         << setw(col_width) << "Employment" << RESET << endl;
    print_border(6 * col_width + 5, MAGENTA);

    LoanApplication app;
    bool found_apps = false;
    while (getline(appFile, line))
    {
        app.from_string_row(line);
        if (app.applicationId.empty())
            continue; // Skip if parsing failed
        found_apps = true;

        cout << MAGENTA << setw(col_width) << left << app.applicationId
             << setw(col_width) << app.status
             << setw(col_width) << app.loanCategory
             << setw(col_width) << app.fullName
             << setw(col_width) << app.cnicNumber
             << setw(col_width) << app.employmentStatus
             << RESET << endl;
    }
    print_border(6 * col_width + 5, MAGENTA);
    appFile.close();
    if (!found_apps)
    {
        cout << BOLD_YELLOW << "No applications found." << RESET << endl;
    }
}

void viewDetails(const string &appId)
{
    // FIX: Removed 'const string&' from the function call
    LoanApplication app = load_application_by_id(appId);

    if (app.applicationId.empty())
    {
        cout << BOLD_RED << "Application ID " << appId << " not found." << RESET << endl;
        return;
    }

    int width = 70;
    cout << BOLD_CYAN << "\n-- Details for Application ID: " << appId << " (Status: " << app.status << ", Category: " << app.loanCategory << ") --" << RESET << endl;
    print_border(width, CYAN);

    // C1
    cout << BOLD_YELLOW << "Personal Info (C1):" << RESET << endl;
    cout << CYAN << setw(25) << left << "Full Name:" << MAGENTA << app.fullName << RESET << endl;
    cout << CYAN << setw(25) << left << "CNIC Number (Exp):" << MAGENTA << app.cnicNumber << " (" << app.cnicExpiryDate << ")" << RESET << endl;
    cout << CYAN << setw(25) << left << "Contact/Email:" << MAGENTA << app.contactNumber << " / " << app.emailAddress << RESET << endl;
    cout << CYAN << setw(25) << left << "Employment/Marital:" << MAGENTA << app.employmentStatus << " / " << app.maritalStatus << RESET << endl;
    cout << CYAN << setw(25) << left << "Gender/Dependents:" << MAGENTA << app.gender << " / " << app.numDependents << RESET << endl;

    // C2
    print_border(width, CYAN);
    cout << BOLD_YELLOW << "Financial Info (C2):" << RESET << endl;
    cout << CYAN << setw(35) << left << "Annual Income (PKR):" << MAGENTA << app.annualIncome << RESET << endl;
    cout << CYAN << setw(35) << left << "Monthly Avg Bill/Current (PKR):" << MAGENTA << app.monthlyAvgElectricityBill << " / " << app.currentElectricityBill << RESET << endl;
    cout << CYAN << setw(35) << left << "Existing Loan Active:" << MAGENTA << app.loanActive << RESET << endl;
    if (app.loanActive == "Active" || app.loanActive == "Not Active")
    {
        cout << CYAN << setw(35) << left << "Total/Returned/Due (PKR):" << MAGENTA << app.totalLoanAmount << " / " << app.amountReturned << " / " << app.loanDue << RESET << endl;
        cout << CYAN << setw(35) << left << "Bank Name/Category:" << MAGENTA << app.bankName << " / " << app.existingLoanCategory << RESET << endl;
    }

    // C3
    print_border(width, CYAN);
    cout << BOLD_YELLOW << "References (C3):" << RESET << endl;
    cout << CYAN << setw(25) << left << "Referee 1 Name:" << MAGENTA << app.ref1Name << RESET << endl;
    cout << CYAN << setw(25) << left << "Referee 1 CNIC (Issue):" << MAGENTA << app.ref1Cnic << " (" << app.ref1CnicIssueDate << ")" << RESET << endl;
    cout << CYAN << setw(25) << left << "Referee 2 Name:" << MAGENTA << app.ref2Name << RESET << endl;
    cout << CYAN << setw(25) << left << "Referee 2 CNIC (Issue):" << MAGENTA << app.ref2Cnic << " (" << app.ref2CnicIssueDate << ")" << RESET << endl;

    // C4
    print_border(width, CYAN);
    cout << BOLD_YELLOW << "Image Paths (C4 - data/):" << RESET << endl;
    cout << CYAN << setw(35) << left << "CNIC Front:" << MAGENTA << app.cnicFrontPath << RESET << endl;
    cout << CYAN << setw(35) << left << "CNIC Back:" << MAGENTA << app.cnicBackPath << RESET << endl;
    cout << CYAN << setw(35) << left << "Electricity Bill:" << MAGENTA << app.electricityBillPath << RESET << endl;
    cout << CYAN << setw(35) << left << "Salary Slip/Bank Statement:" << MAGENTA << app.salarySlipPath << RESET << endl;
    print_border(width, CYAN);
}

void generateMonthlyPlan(const string &appId, const string &category, const string &startingMonth)
{
    cout << BOLD_GREEN << "\n--- Monthly Payment Plan Generation ---" << RESET << endl;
    cout << GREEN << "Plan generated for Application ID: " << appId << " (" << category << ")" << RESET << endl;
    cout << GREEN << "Starting Month: " << startingMonth << RESET << endl;
    cout << "A full plan would show month-wise payments, total paid, and remaining balance." << endl;
    print_border(60, GREEN);
}

void updateApplicationStatus(const string &appId, const string &newStatus)
{
    // FIX: Removed 'const string&' from the function call
    LoanApplication app = load_application_by_id(appId);

    if (app.applicationId.empty())
    {
        cout << BOLD_RED << "Application ID " << appId << " not found." << RESET << endl;
        return;
    }
    if (app.status != "Submitted" && app.status!="Rejected" && app.status!="Approved")
    {
        cout << BOLD_RED << "Application ID " << appId << " must be 'Submitted' before it can be 'Approved' or 'Rejected'. Current status: " << app.status << RESET << endl;
        return;
    }

    app.status = newStatus;
    save_application_to_file(app);

    cout << BOLD_GREEN << "\n"
         << CHECK_MARK << " Status of Application ID " << appId << " successfully updated to: " << newStatus << RESET << endl;

    if (newStatus == "Approved")
    {
        string startingMonth;
        cout << BOLD_CYAN << "Enter the starting month for the payment plan (e.g., Nov-2025): " << RESET;
        getline(cin, startingMonth);
        generateMonthlyPlan(appId, app.loanCategory, startingMonth);
    }
}

void runLenderInterface()
{
    cout << BOLD_MAGENTA << "\n=======================================================" << RESET << endl;
    cout << BOLD_MAGENTA << "             Lender / Admin Console Interface            " << RESET << endl;
    cout << BOLD_MAGENTA << "=======================================================" << RESET << endl;
    string userInput;
    while (true)
    {
        cout << BOLD_CYAN << "\nLender Menu:\n"
             << MAGENTA << "V - View all applications\n"
             << MAGENTA << "D - View details of an application\n"
             << MAGENTA << "A - Approve a SUBMITTED application\n"
             << MAGENTA << "R - Reject a SUBMITTED application\n"
             << MAGENTA << "X - Exit" << RESET << BOLD_GREEN << "\n> " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);

        if (lower_input == "x")
        {
            cout << GREEN << "Exiting Lender Interface." << RESET << endl;
            return;
        }
        else if (lower_input == "v")
        {
            viewApplications();
        }
        else if (lower_input == "d")
        {
            string appId;
            cout << CYAN << "Enter Application ID to view details (e.g., 0001): " << RESET;
            getline(cin, appId);
            viewDetails(appId);
        }
        else if (lower_input == "a")
        {
            string appId;
            cout << CYAN << "Enter Application ID to APPROVE: " << RESET;
            getline(cin, appId);
            updateApplicationStatus(appId, "Approved");
        }
        else if (lower_input == "r")
        {
            string appId;
            cout << CYAN << "Enter Application ID to REJECT: " << RESET;
            getline(cin, appId);
            updateApplicationStatus(appId, "Rejected");
        }
        else
        {
            cout << BOLD_RED << "Invalid option." << RESET << endl;
        }
    }
}

// --- Main Chatbot Function (Applicant Interface) ---
void runChatbot()
{
    cout << BOLD_YELLOW << R"(
 _       ____    ___    _   _     ____    ____   _______ 
| |     / __ \  / _ \  | \ | |   |  _ \  / __ \ |__  __|
| |    | |  | || |_| | |  \| |   | |_) || |  | |   | |   
| |    | |  | ||  __ | | . ` |-- |  _ < | |  | |   | |   
| |____| |__| || | | | | |\  |   | |_) || |__| |   | |   
|______|\____/ |_| |_| |_| \_|   |____/  \____/    |_|   
                                                        
                                                        
Welcome to Loan-Bot. I am here to help you!
)" << RESET
         << endl;

    string userInput;

    // Greeting/Mode Selection
    while (true)
    {
        cout << BOLD_GREEN << "\nHow can I help you today? (Type 'Apply' for loans, 'Chat' for general talk, or 'X' to exit): " << RESET;
        getline(cin, userInput);
        string lower_input = to_lower(userInput);

        if (lower_input == "x" || lower_input == "exit" || lower_input == "bye")
        {
            cout << GREEN << "Thanks for coming!" << RESET << endl;
            return;
        }

        // Custom Greetings
        if (lower_input.find("hi") != string::npos || lower_input.find("hello") != string::npos ||
            lower_input.find("salam") != string::npos || lower_input.find("aoa") != string::npos)
        {
            cout << GREEN << "Hello! I am Loan-Bot. I can assist with loan applications or general conversation." << RESET << endl;
            continue;
        }

        if (lower_input == "apply")
            break;
        if (lower_input == "chat")
            break;

        cout << BOLD_RED << "Please enter a valid command ('Apply', 'Chat', or 'X')." << RESET << endl;
    }

    string mode = to_lower(userInput);

    if (mode == "chat")
    {
        cout << BOLD_YELLOW << "\n--- General Conversation Mode ---" << RESET << endl;
        cout << MAGENTA << "Ask me anything, and I will try to respond based on my chat corpus (IoU match)." << RESET << endl;
        print_border(70, YELLOW);

        while (true)
        {
            cout << BOLD_GREEN << "You: " << RESET;
            getline(cin, userInput);

            string lower_input = to_lower(userInput);
            if (lower_input == "x" || lower_input == "exit" || lower_input == "bye")
            {
                cout << GREEN << "Goodbye! Returning to main menu." << RESET << endl;
                return;
            }

            // Get response based on IoU
            cout << BOLD_CYAN << "Bot: " << RESET << IoU_response(userInput, "human_chat_corpus.txt") << endl;
        }
    }

    // --- Loan Application Flow (Apply Mode) ---
    cout << BOLD_YELLOW << "\n--- Loan Application Mode ---" << RESET << endl;

    string appMode;
    while (true)
    {
        cout << BOLD_CYAN << "Are you: N - Starting a New application, R - Resuming an incomplete one, or X - Exit? " << RESET;
        getline(cin, appMode);
        string lower_appMode = to_lower(appMode);

        if (lower_appMode == "x")
        {
            cout << GREEN << "Thanks for coming!" << RESET << endl;
            return;
        }
        if (lower_appMode == "n" || lower_appMode == "r")
            break;
        cout << BOLD_RED << "Invalid option. Please enter N, R, or X." << RESET << endl;
    }

    string existingId = "";
    string existingCnic = "";

    if (to_lower(appMode) == "r")
    {
        cout << CYAN << "Enter your Application ID: " << RESET;
        getline(cin, existingId);
        cout << CYAN << "Enter your 13-digit CNIC: " << RESET;
        getline(cin, existingCnic);

        // Skip category selection, it's loaded from the existing application
        collect_application_data("", existingId, existingCnic);
        return;
    }

    // --- New Application: Category Selection ---
    string selectedCategory = "";
    while (selectedCategory.empty())
    {
        print_border(30, CYAN);
        cout << BOLD_CYAN << "SELECT LOAN CATEGORY" << endl
             << CYAN << "H - Home Loan\nC - Car Loan\nS - Scooter Loan\nP - Personal Loan\nX - Exit" << RESET << BOLD_GREEN << "\n> " << RESET;

        getline(cin, userInput);
        string lower_input = to_lower(userInput);

        if (lower_input == "x")
        {
            cout << GREEN << "Thanks for coming" << RESET << endl;
            return;
        }

        if (lower_input == "h" || lower_input == "c" || lower_input == "s" || lower_input == "p")
        {
            selectedCategory = (lower_input == "h") ? "Home" : (lower_input == "c") ? "Car"
                                                           : (lower_input == "s")   ? "Scooter"
                                                                                    : "Personal";

            cout << BOLD_YELLOW << "\nAvailable " << selectedCategory << " Loan Options:" << RESET << endl;

            string fileName = selectedCategory + ".txt";
            bool optionsAvailable = (lower_input != "p");

            if (!optionsAvailable)
            {
                cout << BOLD_RED << "No specific options list for Personal Loan. Proceeding to application..." << RESET << endl;
            }
            else
            {
                // Show options table for H, C, or S
                file_reader(fileName, "", true);
            }

            string optionNumber;
            bool optionSelected = false;

            // Option Selection Loop (Only runs if options were displayed)
            while (optionsAvailable && !optionSelected)
            {
                cout << BOLD_GREEN << "\nEnter the OPTION NUMBER you want to select (e.g., 1, 2, ...) or R to re-view options, or X to proceed without pre-selection: " << RESET;
                getline(cin, optionNumber);
                string lower_option = to_lower(optionNumber);

                if (lower_option == "x")
                {
                    cout << BOLD_YELLOW << "Skipping specific loan option selection. Proceeding to application." << RESET << endl;
                    optionSelected = true; // Break loop
                    break;
                }

                if (lower_option == "r")
                {
                    file_reader(fileName, "", true);
                    continue;
                }

                if (isNumber(optionNumber))
                {
                    // Display detailed selection and installment calculation (Break out of the loop)
                    file_reader(fileName, optionNumber, false);
                    optionSelected = true;
                }
                else
                {
                    cout << BOLD_RED << "Invalid input! Please enter a valid OPTION NUMBER, R, or X." << RESET << endl;
                }
            }

            // Proceed to data collection
            collect_application_data(selectedCategory, "", "");

            return;
        }
        else
        {
            cout << BOLD_RED << " Invalid input! Please select H, C, S, P, or X." << RESET << endl;
        }
    }
}

// --- Main Function ---
#ifndef UNIT_TEST
int main()
{
    string mode;
    while (true)
    {
        cout << BOLD_YELLOW << "\n--- System Mode Selection ---" << RESET << endl;
        cout << BOLD_GREEN << "Enter 'B' for Bot Interface (Applicant) or 'L' for Lender Interface, or 'X' to exit: " << RESET;
        getline(cin, mode);

        string lower_mode = to_lower(mode);

        if (lower_mode == "x")
        {
            cout << GREEN << "System exiting. Goodbye!" << RESET << endl;
            break;
        }
        else if (lower_mode == "b")
        {
            runChatbot();
        }
        else if (lower_mode == "l")
        {
            runLenderInterface();
        }
        else
        {
            cout << BOLD_RED << "Invalid mode selection. Please enter B, L, or X." << RESET << endl;
        }
    }
    return 0;
}
#endif