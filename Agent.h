#ifndef AGENT_H
#define AGENT_H

#include <string>
using namespace std;

// Function declarations
string to_lower(string s);
bool compare_responses(string user_greet, string response,string user_input);
string string_parser(string line, bool &res, string user_input);
string file_reader(string file, string user_input);
bool isNumber(const string &s);
void runChatbot();
string Home_file_reader(string file,string user_input);
#endif
