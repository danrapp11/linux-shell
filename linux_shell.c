/*
project: 01
author:Daniel Rapp
email:drapp1@umbc.edu
student id:MJ95336
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

/*
In this project, you are going to implement a number of functions to
create a simple linux shell interface to perform basic linux commands
*/

/*
DEFINE THE FUNCTION PROTOTYPES
*/
int user_prompt_loop();
char* get_user_command();
char** parse_command(char* command);
void execute_command(char** parsed_command);
void proc_execute(char* filename);

int main(int argc, char **argv)
{
/*
    Write the main function that checks the number of argument passed to ensure 
    no command-line arguments are passed; if the number of argument is greater 
    than 1 then exit the shell with a message to stderr and return value of 1
    otherwise call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */

    if (argc > 1 && argv[1] != NULL) {
	fprintf(stderr, "Too many arguments!\n");
	return 1;
    }
    int exitval = user_prompt_loop();
    exit(exitval);
}
/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets command and sends it to a parser, then compares the first element to the two
different commands ("/proc", and "exit"). If it's none of the commands, 
send it to the execute_command() function. If the user decides to exit, then exit 0 or exit 
with the user given value. 
*/

int user_prompt_loop()
{
    // initialize variables

    /*
    loop:
        1. prompt the user to type command by printing >>
        2. get the user input using get_user_command() function 
        3. parse the user input using parse_command() function 
        Example: 
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]
        4. compare the first element of the parsed output to "/proc"and "exit"
        5. if the first element is "/proc" then you have the open the /proc file system 
           to read from it
            i) concat the full command:
                Ex: user input >>/proc /process_id_no/status
                    concated output: /proc/process_id_no/status
            ii) read from the file line by line. you may user fopen() and getline() functions
            iii) display the following information according to the user input from /proc
                a) Get the cpu information if the input is /proc/cpuinfo
                - Cpu Mhz
                - Cache size
                - Cpu cores
                - Address sizes
                b) Get the number of currently running processes from /proc/loadavg
                c) Get how many seconds your box has been up, and how many seconds it has been idle from /proc/uptime
                d) Get the following information from /proc/process_id_no/status
                - the vm size of the virtual memory allocated the vbox 
                - the most memory used vmpeak 
                - the process state
                - the parent pid
                - the number of threads
                - number of voluntary context switches
                - number of involuntary context switches
                e) display the list of environment variables from /proc/process_id_no/environ
                f) display the performance information if the user input is /proc/process_id_no/sched
        6. if the first element is "exit" the use the exit() function to terminate the program
        7. otherwise pass the parsed command to execute_command() function 
        8. free the allocated memory using the free() function
    */

    int is_exit = 1;
    int exitval = 0;
    printf(">> ");
    char* command = get_user_command();
    char** parsed_command = parse_command(command);
    while(is_exit != 0) {
	if (strstr(parsed_command[0], "exit") != NULL) {
	    if (parsed_command[1] != NULL)
		exitval = atoi(parsed_command[1]);
	    is_exit = 0;
	    break;
	}
	// open /proc file system and print out info from file requested by user.
	else if (strstr(parsed_command[0],"/proc") != NULL) {
	    char* filename = parsed_command[0];
	    char* source = parsed_command[1];
	    strcat(filename,source);
	    if (strstr(filename,"cpuinfo") != NULL) {
		FILE* cpuinfo = fopen(filename, "r");
		char line[64];
		int printed = 0;
		while (fgets(line, sizeof(line), cpuinfo) != NULL && printed < 4) {
		    if (strstr(line,"cpu MHz") != NULL) {
			fprintf(stdout,line);
			printed++;
		    }
		    else if (strstr(line,"cache size") != NULL) {
			fprintf(stdout,line);
			printed++;
		    }
		    else if (strstr(line,"cpu cores") != NULL) {
			fprintf(stdout,line);
			printed++;
		    }
		    else if (strstr(line,"address sizes") != NULL) {
			fprintf(stdout,line);
			printed++;
		    }
		}
		fclose(cpuinfo);
	    }
	    else if (strstr(filename,"loadavg") != NULL) {
		FILE* loadavg = fopen(filename, "r");
		char line[32];
		fgets(line, sizeof(line), loadavg);
		char** parsed_loadavg = parse_command(line);
		printf("Number of running processes: %s\n",parsed_loadavg[3]);
		fclose(loadavg);
		free(parsed_loadavg);
	    }
	    else if (strstr(filename,"uptime") != NULL) {
		FILE* uptime = fopen(filename, "r");
		char line[32];
		fgets(line, sizeof(line), uptime);
		char** parsed_uptime = parse_command(line);
		printf("Seconds the box has been up: %s\n",parsed_uptime[0]);
		printf("Seconds the box has been idle: %s\n",parsed_uptime[1]);
		fclose(uptime);
		free(parsed_uptime);
	    }
	    else if (strstr(filename,"status") != NULL) {
		FILE* status = fopen(filename, "r");
		char line[32];
		while (fgets(line, sizeof(line), status) != NULL) {
		    if (strstr(line,"VmSize") != NULL)
			fprintf(stdout,line);
		    else if (strstr(line,"VmPeak") != NULL)
                        fprintf(stdout,line);
		    else if (strstr(line,"State") != NULL)
                        fprintf(stdout,line);
		    else if (strstr(line,"PPid") != NULL)
                        fprintf(stdout,line);
		    else if (strstr(line,"Threads") != NULL)
                        fprintf(stdout,line);
		    else if (strstr(line,"voluntary_ctxt_switches") != NULL)
                        fprintf(stdout,line);
		    else if (strstr(line,"nonvoluntary_ctxt_switches") != NULL)
                        fprintf(stdout,line);
		}
		fclose(status);
	    }
	    else if (strstr(filename,"environ") != NULL) {
		FILE* env = fopen(filename, "r");
		char line[64];
		while (fgets(line, sizeof(line), env) != NULL)
		    fprintf(stdout,line);
		printf("\n");
		fclose(env);
	    }
	    else if (strstr(filename,"sched") != NULL) {
		FILE* sched = fopen(filename, "r");
		char line[128];
		while (fgets(line, sizeof(line), sched) != NULL)
		    fprintf(stdout,line);
		fclose(sched);
	    }
	    free(filename);
	}
	else {
	    execute_command(parsed_command);
	    free(command);
	}
	free(parsed_command);
	printf(">> ");
	command = get_user_command();
	parsed_command = parse_command(command);
    }
    free(command);
    free(parsed_command);
    return exitval;
}

//get_user_command():
//Take input of arbitrary size from the user and return to the user_prompt_loop()

char* get_user_command()
{
    size_t buffer = 128;
    char* command;
    getline(&command,&buffer,stdin);
    return command;
}

/*
parse_command():
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/

char** parse_command(char* command)
{

    command = unescape(command, stderr);
    // replace newline characters with null (\0)
    if (command[strlen(command) - 1] == '\n')
	command[strlen(command)-1] = '\0';
    int wordcount = 0;
    char* space = " ";
    char* word = strtok(command, space);
    char** parsed_command = malloc(wordcount * sizeof(char*));
    // parse through input, adding each word to list of strings, reallocating space for new words accordingly.
    while (word != NULL) {
	wordcount++;
	parsed_command = realloc(parsed_command, wordcount * sizeof(char*));
	parsed_command[wordcount-1] = word;
	word = strtok(NULL, space);
    }
    return parsed_command;
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

void execute_command(char** parsed_command)
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == -1)
	printf("forking failed\n");
    else if (pid == 0)
	execvp(parsed_command[0], parsed_command);
    else if (pid > 0) {
	pid_t tpid = waitpid(-1, &status, 0);
	while(tpid != pid) {
	    if (tpid != pid)
		printf("process terminated\n");
	}
    }
    return;
}
