#define UNIT_TEST


#include <cassert>
#include <iostream>
#include<fstream>
#include "Agent.h"
using namespace std;

void test_general_case(){
    bool res = false;
    string response = string_parser("*#How can i help you dear?!", res,"Assalam-o-Alaikum");
    assert(res == false);
    assert(response == "How can i help you dear?!");
    cout << " General -> test_string_parser passed\n";
    
}


void test_home_display() {
    
    ofstream out("Home.txt");
    out << "Area#Size#Installments#Price#Down Payment\n";
    out << "A1#5 Marla#24#5000000#500000\n";
    out << "A2#10 Marla#36#8000000#800000\n";
    out.close();

    string response = Home_file_reader("Home.txt", "",true);
    cout << "Output:\n" << response << endl;


    assert(response.find("Area: A2") != string::npos);
    assert(response.find("Price: 8000000") != string::npos);
    cout << " test_home_display passed\n";
}


void test_isNumber() {
    assert(isNumber("123"));
    assert(!isNumber("12a"));
    assert(!isNumber(""));
    cout << " test_isNumber passed\n";
}

void test_string_parser() {
    bool res = false;
    string response = string_parser("Hi#Hello there!", res,"Hi");
    assert(res == true);
    assert(response == "Hello there!");
    cout << " test_string_parser passed\n";
}

void test_compare_responses() {
    bool result = compare_responses("Hi", "Hello there!", "Hi");
    assert(result == true);
    result = compare_responses("Hi", "Hello there!", "Bye");
    assert(result == false);
    cout << " test_compare_responses passed\n";
}

int main() {
    test_isNumber();
    test_string_parser();
    test_compare_responses();
    test_general_case();
    test_home_display();
    cout << " All tests passed successfully!" << endl;
    return 0;
}
