# linux-shell
Linux shell interface which allowa a user to input basic commands
1. main function
	checks if there are any command-line arguments and return an error if any are found. calls the user_prompt_loop
	function, which returns an int value for the exit call.
2. user_prompt_loop
	prompts the user for command, which is obtained using the get_user_command function, the command is then parsed
	with parse_command. continues asking for input commands, does the required action and loops until the exit flag is 1.
	after the command is obtained and parsed, first checks if the command contain the word "exit", if so, sets exit flat
	to 1 and sets the exit value to be returned to the value given in the command, converting string to int using atoi.
	if the command contains the "/proc" keyword, and concat the "/proc" with the rest of the directory given. Prints
	differently depending on the directory inputed, but each function opens the file at the given directory, reads said
	file line by line using fgets and prints the desired information using fprintf. If the command given is not "exit" or
	"/proc", call the execute_command function. after the loop is exited, free the memory allocated to command and
	parsed_command and return the exit value.
3. get_user_command
	calls the getline() function to get user input for the command and returns said command.
4. parse_command
	uses unescape to allocate memory for the command passed in and removing any escape sequences. Replaces the endline
	char at the end of the string with '\0', otherwise execute_command and "/proc" will have issues. Uses the strtok
	function to read through each token of the command separated by a space, and add each token to a list of strings,
	reallocating space for each new token, after returns the list of strings.
5. execute_command
	forks the current process to execute the command without exiting the program. If the fork returns -1, it failed, if
	it returns 0, it is in the child process, so execvp() is called to run the command inputed by the user. if the fork
	returns anything > 0, wait until the child process is finished and once the child is done, return to user_prompt_loop
