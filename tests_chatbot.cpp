#define UNIT_TEST


#include <cassert>
#include <iostream>
#include "Agent.h"
using namespace std;

void test_isNumber() {
    assert(isNumber("123"));
    assert(!isNumber("12a"));
    assert(!isNumber(""));
    cout << "✅ test_isNumber passed\n";
}

void test_string_parser() {
    bool res = false;
    string response = string_parser("Hi#Hello there!", res,"Hi");
    assert(res == true);
    assert(response == "Hello there!");
    cout << "✅ test_string_parser passed\n";
}

void test_compare_responses() {
    bool result = compare_responses("Hi", "Hello there!", "Hi");
    assert(result == true);
    result = compare_responses("Hi", "Hello there!", "Bye");
    assert(result == false);
    cout << "✅ test_compare_responses passed\n";
}

int main() {
    test_isNumber();
    test_string_parser();
    test_compare_responses();
    cout << "🎉 All tests passed successfully!" << endl;
    return 0;
}
