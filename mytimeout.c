#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {

pid_t pid, result;
int stat;
int i;
int snds;
char *cmd;

	// not required number of arguments 
    if (argc < 3) {
        printf("Usage: %s snds cmd [cmd-args]\n", argv[0]);
        exit(1);
    
	}
	
	// snds mentioned in the command
    snds = atoi(argv[1]);
    cmd = argv[2];
	
	
    char *args[argc-1];
    
    
    for (i = 2; i < argc; i++) {
        args[i-2] = argv[i];
    }
    
    args[argc-2] = NULL;

	// create a child process
	// to run the given command
	
    pid = fork();
    
    if (pid < 0) {
    	
    	// cannot create a fork
    	    	
        printf("Failed to fork a child process\n");
        exit(1);
        
    } 
    
    else if (pid == 0) {
		
		// Child
		
		// execte the command
        execvp(cmd, args);
        printf("Failed to execute the command\n");
        exit(1);
        
    } 
    
	else {
        // Parent 
        
        // Wait for the snds seconds
        sleep(snds);

	result = waitpid(pid, &stat, WNOHANG);

	if (result == 0) {
        
        // process recieves signal - terminated 
        if (kill(pid, SIGTERM) == 0) {
        
            printf("Command terminated by timeout\n");
        } 
        
        // Failed to recieve signal - terminated
        else {
        
            printf("Failed to terminate the command\n");
        } }

	else {
	
	printf("command executed before timeout!\n");
	
	}
        
        exit(0);
    }
    
    return 0;
}



/* 

$ gcc mytimeout.c -o mytimeout.x
$ ./mytimeout.x 5 sleep 10
Command terminated by timeout


*/
