#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring> // For strerror
#include <errno.h>   // For errno

using namespace std;

// Function to display the prompt
void display_prompt() {
    cout << "myshell> ";
}

// Function to read a line of input
string read_input() {
    string input_line;
    getline(cin, input_line);
    return input_line;
}

// Function to split the input into commands and arguments
vector<string> parse_input(const string& input_line) {
    vector<string> tokens;
    stringstream ss(input_line);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to execute a command
int execute_command(vector<string>& args) {
    if (args.empty()) {
        return 0; // No command entered
    }

    string command = args[0];

    // Built-in commands
    if (command == "exit") {
        return 1; // Signal to exit the shell
    } else if (command == "cd") {
        if (args.size() > 1) {
            if (chdir(args[1].c_str()) != 0) {
                cerr << "myshell: cd: " << strerror(errno) << endl;
            }
        } else {
            cerr << "myshell: cd: missing argument" << endl;
        }
        return 0;
    } else if (command == "help") {
        cout << "myshell: Available commands: exit, cd, help" << endl;
        return 0;
    }

    // External commands
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        // Convert vector<string> to char* array for execvp
        vector<char*> c_args;
        for (size_t i = 0; i < args.size(); ++i) {
            c_args.push_back(const_cast<char*>(args[i].c_str()));
        }
        c_args.push_back(nullptr); // execvp needs a null-terminated array

        execvp(c_args[0], c_args.data());

        // If execvp returns, there was an error
        cerr << "myshell: " << command << ": " << strerror(errno) << endl;
        exit(1);
    } else if (pid < 0) {
        // Fork error
        cerr << "myshell: fork: " << strerror(errno) << endl;
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}

// Signal handler for Ctrl+C
void signal_handler(int signal) {
    if (signal == SIGINT) {
        cout << endl; // Move to a new line after "^C" is displayed
        display_prompt(); // Re-display the prompt
        fflush(stdout);    // Flush the output buffer
    }
}

int main() {
    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler);

    int exit_status = 0;
    while (!exit_status) {
        display_prompt();
        string input_line = read_input();
        vector<string> args = parse_input(input_line);
        exit_status = execute_command(args);
    }

    cout << "Exiting myshell." << endl;
    return 0;
}

