#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMANDS 256
#define MAX_ARGS 15

using namespace std;

int main() { // Main entry point of the shell
    
    char command[MAX_COMMANDS]; // C-string variable to store command input
    char *args[MAX_ARGS]; // Array of C-string to store parsed tokens
    
    char delim[] = " \n"; // Delimeter to be parsed
    char *tokens; // Char pointer where parsed token position is stored
    
    pid_t pid; // variable to store process ID

    do {
        cout << "Heesang's Shell # "; // Show # to waiting for user command input
        fgets(command, sizeof command, stdin);
        // cout << command << endl; // print input command for check
        
        /* 
            Parses String into d
                                   */
        int i = 0;  // Index integer for argument array
        tokens = strtok(command, delim); // pointer to first token
        while (tokens != NULL) { // Loop through each token
            args[i++] = tokens; 
            tokens = strtok(NULL, delim);
        }
        args[i] = NULL; // Set the last token as NULL to point end

        // Output parsed command
        cout << "======================" << endl;
        int j = 0; // Index integer for argument array (printing purpose)
        while (j < i) {
            cout << "token [" << j << "]: " << args[j] << endl;
            j++;
        }
        cout << "======================" << endl;
        
        // Run execvp() to run command, fork() a child when running exec()
        switch(pid = fork()) {
            case 0: // Child Process
                execvp(args[0], args);
                perror("ERROR: execvp() failed");
                break;

            case -1: // Fork Failed
                perror("ERROR: fork() failed");
                break;

            default: // Parent Process
                wait(NULL); // waits until child terminates exec()
                break;
        }
    } while (1);

    return 0;
}
