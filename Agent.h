#ifndef AGENT_H
#define AGENT_H

#include <string>
using namespace std;

// ANSI color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_RED "\033[1;31m"

// Function Declarations
string to_lower(string s);
bool compare_responses(string user_greet, string response,string user_input);
string string_parser(string line, bool &res, string user_input);
string file_reader(string file, string user_input, bool show_all );
bool isNumber(const string &s);
string Home_file_reader(string file,string user_input,bool show_all );
void runChatbot();

// UI Helper Declarations
void typeWriter(string text, int speed=25);
void loadingEffect(string msg, int dots=3);
void printBox(string title, string content, string color=CYAN);
void showWelcome();
void exitAnimation();

#endif
