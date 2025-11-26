#define UNIT_TEST
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Agent.h"

using namespace std;

// ============== HELPER FUNCTIONS ==============
void cleanup_test_files() {
    // Remove application files
    remove("applications.txt");
    
    // Remove test data files
    remove("Home.txt");
    remove("Car.txt");
    remove("Scooter.txt");
    remove("human_chat_corpus.txt");
    
    // Remove test data directory
    try {
        filesystem::remove_all("./data");
    } catch (...) {
        // Ignore if directory doesn't exist
    }
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
    assert(isNumber("-123") == true);
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
    assert(to_double("-500") == -500.0);
    assert(to_double("invalid") == 0.0);
    cout << "✓ test_to_double passed\n";
}

void test_tokenize() {
    vector<string> tokens = tokenize("Hello World Test");
    assert(tokens.size() == 3);
    assert(tokens[0] == "hello");
    assert(tokens[1] == "world");
    assert(tokens[2] == "test");
    
    tokens = tokenize("  Multiple   Spaces  ");
    assert(tokens.size() == 2);
    assert(tokens[0] == "multiple");
    assert(tokens[1] == "spaces");
    
    tokens = tokenize("");
    assert(tokens.size() == 0);
    
    cout << "✓ test_tokenize passed\n";
}

void test_calculate_iou() {
    // Test identical strings
    assert(calculate_iou("hello world", "hello world") == 1.0);
    
    // Test completely different strings
    assert(calculate_iou("hello", "world") == 0.0);
    
    // Test partial overlap
    double iou = calculate_iou("hello world test", "hello test case");
    // Intersection: {"hello", "test"} = 2
    // Union: {"hello", "world", "test", "case"} = 4
    // IoU = 2/4 = 0.5
    assert(abs(iou - 0.5) < 0.001);
    
    // Test empty strings
    assert(calculate_iou("", "hello") == 0.0);
    assert(calculate_iou("hello", "") == 0.0);
    assert(calculate_iou("", "") == 0.0);
    
    cout << "✓ test_calculate_iou passed\n";
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
    assert(is_valid_date("29-02-2024") == true);  // leap year
    assert(is_valid_date("29-02-2023") == false); // not leap year
    assert(is_valid_date("31-04-2025") == false); // April has 30 days
    assert(is_valid_date("1-1-2025") == false);      // missing leading zeros
    assert(is_valid_date("01/01/2025") == false);    // wrong separator
    assert(is_valid_date("01-01-25") == false);      // 2-digit year
    assert(is_valid_date("2025-01-01") == false);    // wrong format
    assert(is_valid_date("") == false);
    cout << "✓ test_is_valid_date passed\n";
}

// ============== ID MANAGEMENT TESTS ==============

void test_generate_unique_id() {
    cleanup_test_files();
    
    string id1 = generate_unique_id();
    assert(id1 == "0001");
    
    // Create applications file with one entry
    ofstream appFile("applications.txt");
    appFile << "ID#Category#Status#FullName#FatherName#PostalAddress#ContactNumber#EmailAddress#CNICNumber#CNICExpiryDate#EmploymentStatus#MaritalStatus#Gender#NumDependents#AnnualIncome#MonthlyAvgElectricityBill#CurrentElectricityBill#LoanActive#TotalLoanAmount#AmountReturned#LoanDue#BankName#ExistingLoanCategory#Ref1Name#Ref1Cnic#Ref1CnicIssueDate#Ref1Phone#Ref1Email#Ref2Name#Ref2Cnic#Ref2CnicIssueDate#Ref2Phone#Ref2Email#CNICFrontPath#CNICBackPath#ElectricityBillPath#SalarySlipPath\n";
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
    appFile << "ID#Category#Status#FullName#FatherName#PostalAddress#ContactNumber#EmailAddress#CNICNumber#CNICExpiryDate#EmploymentStatus#MaritalStatus#Gender#NumDependents#AnnualIncome#MonthlyAvgElectricityBill#CurrentElectricityBill#LoanActive#TotalLoanAmount#AmountReturned#LoanDue#BankName#ExistingLoanCategory#Ref1Name#Ref1Cnic#Ref1CnicIssueDate#Ref1Phone#Ref1Email#Ref2Name#Ref2Cnic#Ref2CnicIssueDate#Ref2Phone#Ref2Email#CNICFrontPath#CNICBackPath#ElectricityBillPath#SalarySlipPath\n";
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
    app.status = "C1";
    app.fullName = "John Doe";
    app.cnicNumber = "1234567890123";
    app.employmentStatus = "Salaried";
    app.annualIncome = "500000";
    app.cnicFrontPath = "./data/0001/cnic_front.jpg";
    
    string row = app.to_string_row();
    stringstream ss(row);
    string field;
    vector<string> fields;
    while (getline(ss, field, '#')) {
        fields.push_back(field);
    }
    
    assert(fields.size() == 37);
    assert(fields[0] == "0001");
    assert(fields[1] == "Home");
    assert(fields[2] == "C1");
    assert(fields[3] == "John Doe");
    assert(fields[8] == "1234567890123");
    assert(fields[10] == "Salaried");
    assert(fields[14] == "500000");
    assert(fields[33] == "./data/0001/cnic_front.jpg");
    
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
    assert(app.ref1Name == "Ref1");
    assert(app.cnicFrontPath == "path1");
    
    cout << "✓ test_LoanApplication_from_string_row passed\n";
}

void test_LoanApplication_validation() {
    LoanApplication app;
    
    // Test incomplete C1
    assert(app.isC1Complete() == false);
    
    // Fill C1 fields
    app.fullName = "John";
    app.fatherName = "Father";
    app.postalAddress = "Address";
    app.contactNumber = "1234567890";
    app.emailAddress = "john@email.com";
    app.cnicNumber = "1234567890123";
    app.cnicExpiryDate = "01-01-2030";
    app.employmentStatus = "Salaried";
    app.maritalStatus = "Single";
    app.gender = "Male";
    app.numDependents = "0";
    
    assert(app.isC1Complete() == true);
    
    // Test C2 validation
    app.annualIncome = "100000";
    app.monthlyAvgElectricityBill = "5000";
    app.currentElectricityBill = "6000";
    app.loanActive = "N/A";
    
    assert(app.isC2Complete() == true);
    
    // Test with active loan
    app.loanActive = "Active";
    assert(app.isC2Complete() == false); // loan details missing
    
    app.totalLoanAmount = "50000";
    app.amountReturned = "10000";
    app.loanDue = "40000";
    app.bankName = "ABC Bank";
    app.existingLoanCategory = "Personal";
    
    assert(app.isC2Complete() == true);
    
    cout << "✓ test_LoanApplication_validation passed\n";
}

// ============== FILE READER TESTS ==============

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

void test_Home_file_reader() {
    ofstream out("Home.txt");
    out << "Area#Size#Installments#Price#Down Payment\n";
    out << "Bahria Town#5 Marla#24#5000000#500000\n";
    out << "DHA Phase 6#10 Marla#36#8000000#800000\n";
    out.close();
    
    // Test display mode - should not crash
    Home_file_reader("Home.txt", "", true);
    
    // Test selection mode - should not crash
    Home_file_reader("Home.txt", "1", false);
    
    remove("Home.txt");
    cout << "✓ test_Home_file_reader passed\n";
}

void test_Car_file_reader() {
    ofstream out("Car.txt");
    out << "Make#Model#Engine#Used#Year#Installments#Price#Down Payment\n";
    out << "Toyota#Corolla#1800#No#2024#36#4500000#900000\n";
    out << "Honda#Civic#1500#Yes#2022#24#3200000#640000\n";
    out.close();
    
    // Test display mode
    Car_file_reader("", true);
    
    // Test selection mode
    Car_file_reader("1", false);
    
    remove("Car.txt");
    cout << "✓ test_Car_file_reader passed\n";
}

// ============== DOCUMENT COLLECTION TEST ==============

void test_collect_documents_functionality() {
    cleanup_test_files();
    
    // Create a test image file
    ofstream testImage("test_cnic.jpg");
    testImage << "fake image content";
    testImage.close();
    
    LoanApplication app;
    app.applicationId = "0001";
    
    // Test the copyFile lambda logic by creating the directory and copying
    string destDir = "./data/" + app.applicationId + "/";
    filesystem::create_directories(destDir);
    
    string srcPath = "test_cnic.jpg";
    string destPath = destDir + "cnic_front.jpg";
    
    // Copy file using filesystem
    filesystem::copy_file(srcPath, destPath, filesystem::copy_options::overwrite_existing);
    
    // Verify file exists
    ifstream checkFile(destPath);
    assert(checkFile.good());
    checkFile.close();
    
    // Clean up test files
    remove("test_cnic.jpg");
    filesystem::remove_all("./data");
    
    cout << "✓ test_collect_documents_functionality passed\n";
}

// ============== INTEGRATION TESTS ==============

void test_save_and_load_application() {
    cleanup_test_files();
    
    LoanApplication app;
    app.applicationId = "0001";
    app.loanCategory = "Home";
    app.status = "C1";
    app.fullName = "Integration Test User";
    app.fatherName = "Father Name";
    app.postalAddress = "Test Address";
    app.contactNumber = "03001234567";
    app.emailAddress = "test@email.com";
    app.cnicNumber = "1234567890123";
    app.cnicExpiryDate = "01-01-2030";
    app.employmentStatus = "Salaried";
    app.maritalStatus = "Single";
    app.gender = "Male";
    app.numDependents = "0";
    app.annualIncome = "500000";
    app.monthlyAvgElectricityBill = "5000";
    app.currentElectricityBill = "5500";
    app.loanActive = "N/A";
    app.totalLoanAmount = "0";
    app.amountReturned = "0";
    app.loanDue = "0";
    app.bankName = "N/A";
    app.existingLoanCategory = "N/A";
    app.ref1Name = "Reference One";
    app.ref1Cnic = "1111111111111";
    app.ref1CnicIssueDate = "01-01-2020";
    app.ref1Phone = "03111111111";
    app.ref1Email = "ref1@email.com";
    app.ref2Name = "Reference Two";
    app.ref2Cnic = "2222222222222";
    app.ref2CnicIssueDate = "01-01-2020";
    app.ref2Phone = "03222222222";
    app.ref2Email = "ref2@email.com";
    app.cnicFrontPath = "./data/0001/cnic_front.jpg";
    app.cnicBackPath = "./data/0001/cnic_back.jpg";
    app.electricityBillPath = "./data/0001/electricity_bill.jpg";
    app.salarySlipPath = "./data/0001/salary_slip.jpg";
    
    save_application_to_file(app);
    
    // Load the application back
    LoanApplication loadedApp = load_application_by_id("0001");
    
    assert(loadedApp.applicationId == "0001");
    assert(loadedApp.loanCategory == "Home");
    assert(loadedApp.status == "C1");
    assert(loadedApp.fullName == "Integration Test User");
    assert(loadedApp.cnicNumber == "1234567890123");
    assert(loadedApp.employmentStatus == "Salaried");
    assert(loadedApp.ref1Name == "Reference One");
    assert(loadedApp.cnicFrontPath == "./data/0001/cnic_front.jpg");
    
    cout << "✓ test_save_and_load_application passed\n";
}

// ============== MAIN TEST RUNNER ==============

int main() {
    cout << "\n========== Running Loan Bot Test Suite ==========\n\n";
    
    cout << "--- Basic Utility Tests ---\n";
    test_to_lower();
    test_isNumber();
    test_to_double();
    test_tokenize();
    test_calculate_iou();
    
    cout << "\n--- Validation Tests ---\n";
    test_is13DigitCnic();
    test_is_valid_date();
    
    cout << "\n--- ID Management Tests ---\n";
    test_generate_unique_id();
    test_is_unique_id();
    
    cout << "\n--- LoanApplication Class Tests ---\n";
    test_LoanApplication_to_string_row();
    test_LoanApplication_from_string_row();
    test_LoanApplication_validation();
    
    cout << "\n--- File Reader Tests ---\n";
    test_get_fields_by_line_number();
    test_Home_file_reader();
    test_Car_file_reader();
    
    cout << "\n--- Document Collection Tests ---\n";
    test_collect_documents_functionality();
    
    cout << "\n--- Integration Tests ---\n";
    test_save_and_load_application();
    
    cout << "\n========================================\n";
    cout << "✓✓✓ All tests passed successfully! ✓✓✓\n";
    cout << "========================================\n\n";
    
    cleanup_test_files();
    return 0;
}