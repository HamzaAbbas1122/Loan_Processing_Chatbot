#define UNIT_TEST
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Agent.h"

using namespace std;

// ============== HELPER FUNCTIONS ==============
void cleanup_test_files() {
    remove("Home.txt");
    remove("Car.txt");
    remove("Scooter.txt");
    remove("Utterances.txt");
    remove("applications.txt");
}

// ============== BASIC UTILITY TESTS ==============

void test_to_lower() {
    assert(to_lower("HELLO") == "hello");
    assert(to_lower("HeLLo") == "hello");
    assert(to_lower("hello") == "hello");
    assert(to_lower("") == "");
    cout << "✓ test_to_lower passed\n";
}

void test_isNumber() {
    assert(isNumber("123") == true);
    assert(isNumber("0") == true);
    assert(isNumber("12a") == false);
    assert(isNumber("") == false);
    assert(isNumber(" 123") == false);
    assert(isNumber("123 ") == false);
    cout << "✓ test_isNumber passed\n";
}

void test_to_double() {
    assert(to_double("1000") == 1000.0);
    assert(to_double("1,000") == 1000.0);
    assert(to_double("1,234,567") == 1234567.0);
    assert(to_double("0") == 0.0);
    assert(to_double("invalid") == 0.0);
    cout << "✓ test_to_double passed\n";
}

void test_compare_responses() {
    bool result = compare_responses("Hi", "Hello there!", "Hi");
    assert(result == true);
    
    result = compare_responses("Hi", "Hello there!", "hi");
    assert(result == true);
    
    result = compare_responses("Hi", "Hello there!", "Bye");
    assert(result == false);
    
    cout << "✓ test_compare_responses passed\n";
}

void test_string_parser() {
    bool res = false;
    string response = string_parser("Hi#Hello there!", res, "Hi");
    assert(res == true);
    assert(response == "Hello there!");
    
    res = false;
    response = string_parser("Hello#Welcome!", res, "Goodbye");
    assert(res == false);
    
    res = false;
    response = string_parser("*#Default response", res, "anything");
    assert(res == false);
    assert(response == "Default response");
    
    cout << "✓ test_string_parser passed\n";
}

// ============== VALIDATION TESTS ==============

void test_is13DigitCnic() {
    assert(is13DigitCnic("1234567890123") == true);
    assert(is13DigitCnic("123456789012") == false);  // 12 digits
    assert(is13DigitCnic("12345678901234") == false); // 14 digits
    assert(is13DigitCnic("12345-6789012") == false);  // contains dash
    assert(is13DigitCnic("123456789012a") == false);  // contains letter
    assert(is13DigitCnic("") == false);
    cout << "✓ test_is13DigitCnic passed\n";
}

void test_is_valid_date() {
    assert(is_valid_date("01-01-2025") == true);
    assert(is_valid_date("31-12-2024") == true);
    assert(is_valid_date("1-1-2025") == false);      // missing leading zeros
    assert(is_valid_date("01/01/2025") == false);    // wrong separator
    assert(is_valid_date("01-01-25") == false);      // 2-digit year
    assert(is_valid_date("2025-01-01") == false);    // wrong format
    assert(is_valid_date("") == false);
    cout << "✓ test_is_valid_date passed\n";
}

void test_is_valid_loan_status() {
    assert(is_valid_loan_status("Active") == true);
    assert(is_valid_loan_status("active") == true);
    assert(is_valid_loan_status("ACTIVE") == true);
    assert(is_valid_loan_status("Not Active") == true);
    assert(is_valid_loan_status("not active") == true);
    assert(is_valid_loan_status("Inactive") == false);
    assert(is_valid_loan_status("") == false);
    cout << "✓ test_is_valid_loan_status passed\n";
}

// ============== ID MANAGEMENT TESTS ==============

void test_generate_unique_id() {
    cleanup_test_files();
    
    string id1 = generate_unique_id();
    assert(id1 == "0001");
    
    // Create applications file with one entry
    ofstream appFile("applications.txt");
    appFile << "0001#Home#Submitted#John#Father#Address#123#email#1234567890123#01-01-2030#Salaried#Single#Male#0#100000#5000#2000#N/A#0#0#0#N/A#N/A#Ref1#1234567890123#01-01-2020#123#ref1@email.com#Ref2#1234567890123#01-01-2020#456#ref2@email.com#path1#path2#path3#path4\n";
    appFile.close();
    
    string id2 = generate_unique_id();
    assert(id2 == "0002");
    
    cout << "✓ test_generate_unique_id passed\n";
}

void test_is_unique_id() {
    cleanup_test_files();
    
    // No file exists, all IDs should be unique
    assert(is_unique_id("0001") == true);
    assert(is_unique_id("0099") == true);
    
    // Create applications file with specific IDs
    ofstream appFile("applications.txt");
    appFile << "0001#Home#Submitted#John#Father#Address#123#email#1234567890123#01-01-2030#Salaried#Single#Male#0#100000#5000#2000#N/A#0#0#0#N/A#N/A#Ref1#1234567890123#01-01-2020#123#ref1@email.com#Ref2#1234567890123#01-01-2020#456#ref2@email.com#path1#path2#path3#path4\n";
    appFile << "0005#Car#Submitted#Jane#Father#Address#456#email#9876543210987#01-01-2030#Self-employed#Married#Female#2#200000#6000#3000#Active#50000#10000#40000#Bank#Car#Ref1#1234567890123#01-01-2020#789#ref1@email.com#Ref2#1234567890123#01-01-2020#012#ref2@email.com#path1#path2#path3#path4\n";
    appFile.close();
    
    assert(is_unique_id("0001") == false);
    assert(is_unique_id("0005") == false);
    assert(is_unique_id("0002") == true);
    assert(is_unique_id("0099") == true);
    
    cout << "✓ test_is_unique_id passed\n";
}

// ============== LOANAPPLICATION CLASS TESTS ==============

void test_LoanApplication_to_string_row() {
    LoanApplication app;
    app.applicationId = "0001";
    app.loanCategory = "Home";
    app.status = "Submitted";
    app.fullName = "John Doe";
    app.cnicNumber = "1234567890123";
    
    string row = app.to_string_row();
    assert(row.find("0001") != string::npos);
    assert(row.find("Home") != string::npos);
    assert(row.find("Submitted") != string::npos);
    assert(row.find("John Doe") != string::npos);
    
    cout << "✓ test_LoanApplication_to_string_row passed\n";
}

void test_LoanApplication_from_string_row() {
    string testRow = "0001#Home#Approved#John Doe#Father Name#Address#1234567890#john@email.com#1234567890123#01-01-2030#Salaried#Married#Male#2#150000#5000#3000#Active#100000#20000#80000#ABC Bank#Car#Ref1#1234567890123#01-01-2020#111#ref1@email.com#Ref2#9876543210987#01-01-2020#222#ref2@email.com#path1#path2#path3#path4";
    
    LoanApplication app;
    app.from_string_row(testRow);
    
    assert(app.applicationId == "0001");
    assert(app.loanCategory == "Home");
    assert(app.status == "Approved");
    assert(app.fullName == "John Doe");
    assert(app.cnicNumber == "1234567890123");
    assert(app.employmentStatus == "Salaried");
    assert(app.loanActive == "Active");
    
    cout << "✓ test_LoanApplication_from_string_row passed\n";
}

// ============== FILE READER TESTS ==============

void test_Home_file_reader_display() {
    ofstream out("Home.txt");
    out << "Area#Size#Installments#Price#Down Payment\n";
    out << "Bahria Town#5 Marla#24#5000000#500000\n";
    out << "DHA Phase 6#10 Marla#36#8000000#800000\n";
    out << "Gulberg#7 Marla#30#6500000#650000\n";
    out.close();
    
    string response = Home_file_reader("Home.txt", "", true);
    assert(response == "");  // Function prints but returns empty for display mode
    
    cout << "✓ test_Home_file_reader_display passed\n";
}

void test_Home_file_reader_selection() {
    ofstream out("Home.txt");
    out << "Area#Size#Installments#Price#Down Payment\n";
    out << "Bahria Town#5 Marla#24#5000000#500000\n";
    out << "DHA Phase 6#10 Marla#36#8000000#800000\n";
    out.close();
    
    // Test selecting option 1
    string response = Home_file_reader("Home.txt", "1", false);
    assert(response == "");  // Function prints details but returns empty string
    
    cout << "✓ test_Home_file_reader_selection passed\n";
}

void test_Car_file_reader() {
    ofstream out("Car.txt");
    out << "Make#Model#Engine#Used#Year#Installments#Price#Down Payment\n";
    out << "Toyota#Corolla#1800#No#2024#36#4500000#900000\n";
    out << "Honda#Civic#1500#Yes#2022#24#3200000#640000\n";
    out.close();
    
    // Test display mode
    string response = Car_file_reader("", true);
    assert(response == "");
    
    // Test selection mode
    response = Car_file_reader("1", false);
    assert(response == "");
    
    cout << "✓ test_Car_file_reader passed\n";
}

void test_Scooter_file_reader() {
    ofstream out("Scooter.txt");
    out << "Make#Model#Distance#Time#Speed#Installments#Price#Down Payment\n";
    out << "Jolta#JS-70#70#4#45#12#185000#37000\n";
    out << "Super Power#SP-125#100#6#50#18#225000#45000\n";
    out.close();
    
    // Test display mode
    string response = Scooter_file_reader("", true);
    assert(response == "");
    
    // Test selection mode
    response = Scooter_file_reader("1", false);
    assert(response == "");
    
    cout << "✓ test_Scooter_file_reader passed\n";
}

void test_get_fields_by_line_number() {
    ofstream out("test_fields.txt");
    out << "Header1#Header2#Header3\n";
    out << "Data1#Data2#Data3\n";
    out << "Data4#Data5#Data6\n";
    out << "Data7#Data8#Data9\n";
    out.close();
    
    vector<string> fields = get_fields_by_line_number("test_fields.txt", 2);
    assert(fields.size() == 3);
    assert(fields[0] == "Data4");
    assert(fields[1] == "Data5");
    assert(fields[2] == "Data6");
    
    remove("test_fields.txt");
    cout << "✓ test_get_fields_by_line_number passed\n";
}

// ============== INTEGRATION TESTS ==============

void test_save_and_read_application() {
    cleanup_test_files();
    
    LoanApplication app;
    app.applicationId = "0001";
    app.loanCategory = "Home";
    app.status = "Submitted";
    app.fullName = "Test User";
    app.cnicNumber = "1234567890123";
    
    save_application_to_file(app);
    
    // Read back the application
    ifstream appFile("applications.txt");
    assert(appFile.is_open());
    
    string line;
    getline(appFile, line);
    appFile.close();
    
    LoanApplication readApp;
    readApp.from_string_row(line);
    
    assert(readApp.applicationId == "0001");
    assert(readApp.loanCategory == "Home");
    assert(readApp.fullName == "Test User");
    
    cout << "✓ test_save_and_read_application passed\n";
}

// ============== MAIN TEST RUNNER ==============

int main() {
    cout << "\n========== Running Loan Bot Test Suite ==========\n\n";
    
    cout << "--- Basic Utility Tests ---\n";
    test_to_lower();
    test_isNumber();
    test_to_double();
    test_compare_responses();
    test_string_parser();
    
    cout << "\n--- Validation Tests ---\n";
    test_is13DigitCnic();
    test_is_valid_date();
    test_is_valid_loan_status();
    
    cout << "\n--- ID Management Tests ---\n";
    test_generate_unique_id();
    test_is_unique_id();
    
    cout << "\n--- LoanApplication Class Tests ---\n";
    test_LoanApplication_to_string_row();
    test_LoanApplication_from_string_row();
    
    cout << "\n--- File Reader Tests ---\n";
    test_Home_file_reader_display();
    test_Home_file_reader_selection();
    test_Car_file_reader();
    test_Scooter_file_reader();
    test_get_fields_by_line_number();
    
    cout << "\n--- Integration Tests ---\n";
    test_save_and_read_application();
    
    cout << "\n========================================\n";
    cout << "✓✓✓ All tests passed successfully! ✓✓✓\n";
    cout << "========================================\n\n";
    
    cleanup_test_files();
    return 0;
}