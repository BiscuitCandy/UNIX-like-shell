#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

char* trim_spaces(char *str);
void myexit();
void mycd(char *dir_path);
void mypwd();
void execute_external(char **args);
void execution_draw(char input[]);
void execute_pipes(char *input[], int num_args);


// time function
// trims out the leading and trailing white spaces
char* trim_spaces(char *string) {
    
    int cmd_len;
    
    while (*string == ' ' || *string == '\t') {
        string++;
    }
    
    cmd_len = strlen(string);
    
    while (cmd_len > 0 && (string[cmd_len-1] == ' ' || string[cmd_len-1] == '\t')) {
        cmd_len--;
    }
    string[cmd_len] = '\0';
    
    return string;
}

// implementation of the command execution
// contains executions for myexit
// mycd, mypwd, and external commands
void execution_draw(char input[]) {

	char *args[225];
	int num_args = 0;
	int i;

	
	args[num_args] = strtok(input, " ");
    	while (args[num_args] != NULL) {
        	num_args++;
        	args[num_args] = strtok(NULL, " ");
    	}
    	
    	for(i = 0; i < num_args; i++) {
		    
		    	args[i] = trim_spaces(args[i]);
		    }
    	
	if (args[0] == NULL) {
	
	// no command
    	    return;

	} 
    	    	
    	else if (strcmp(args[0], "mycd") == 0) {

    	    if (num_args != 2) {
    	        printf("Usage: mycd dir\n");
    	        return;
    	    }

    	    mycd(args[1]);
    	} 
    	
    	else if (strcmp(args[0], "mypwd") == 0) {
    	    mypwd();
    	} 

    	else if (strcmp(args[0], "myexit") == 0) {
        	myexit();
    	}
    	
    	else {
    	    execute_external(args);
    	}

}

// pipes execution
// no limit specfied
// tested with around 5 pipes and works prpoerly
void execute_pipes(char *input[], int num_args) {

    int pipefd[2];
    int i, j;
    int stat;
    int in_fd = STDIN_FILENO;
    char *args[225];
    
    pid_t pid;

    for (i = 0; i < num_args; i++) {

	// pipe
        pipe(pipefd);
	
	// fork a new process
        pid = fork();

        if (pid == 0) {
            dup2(in_fd, STDIN_FILENO);
            if (i < num_args - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
            }
            close(pipefd[0]);

            j = 0;
            args[j] = strtok(input[i], " \n");
            while (args[j] != NULL) {
                j++;
                args[j] = strtok(NULL, " \n");
            }
            args[j] = NULL;

            // command execution
            if (execvp(args[0], args) < 0) {
                fprintf(stderr, "ERROR: Check COMMAND");
                exit(0);
            }	
           	
        } 
        
        else if (pid < 0) {
            fprintf(stderr, "ERROR: FORK");
            exit(0);
        } 
        
        else {
        
            wait(&stat);
            close(pipefd[1]);
            in_fd = pipefd[0];
        }
    }
}

// myexit command
void myexit() {

    printf("Exiting ...\n");
    exit(0);
    
}

// mycd command
void mycd(char *dir_path) {

    if (chdir(dir_path) == -1) {
    
        fprintf(stderr, "ERROR: mycd");
    }
}

// mypwd command
void mypwd() {

    char buf[4096];
    if (getcwd(buf, sizeof(buf)) == NULL) {
    
        fprintf(stderr, "ERROR: mypwd");
    } 
    else {
    
        printf("%s\n", buf);
    }
}

//external command execution
void execute_external(char **cmd_args) {

	int stat;

    pid_t pid = fork();
    
    if (pid == -1) {
    
        fprintf(stderr, "ERROR: FORK");
        return;
    } 
    else if (pid == 0) {
    
        execvp(cmd_args[0], cmd_args);
        fprintf(stderr, "ERROR: arguments");
        exit(1);
        
    } 
    else {
    
        wait(&stat);
        
    }
}


int main(int argc, char* argv[]) {

	char input[500];
	char *args[225];
	int num_args = 0;
	char *input_file = NULL;
    char *output_file = NULL;
    int input_fd = -1;
    int output_fd = -1;
    int i;
    int stat;
    
    pid_t pid;
	
	while (1) {
	
        printf("$ ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            myexit();
        }
        
        input[strcspn(input, "\n")] = '\0';

        num_args = 0;
        
        if (strstr(input, "|") != NULL) {
			
			// printf("Has pipe in it \n");
			
			// tokenization by "|"
			
			args[num_args] = strtok(input, "|");
			
			while (args[num_args] != NULL) {
			
		        num_args++;

		        args[num_args] = strtok(NULL, "|");
		    }
		    
		    for(i = 0; i < num_args; i++) {
		    
		    	args[i] = trim_spaces(args[i]);		    
		    }
		    
		    execute_pipes(args, num_args);
		    
			
		}
		
		else if ((strstr(input, "<") != NULL) || (strstr(input, ">"))) {
		
			input_file = NULL;
    		output_file = NULL;
			
			// printf("Has IO Redirection \n");
			
			// IO REDIRECTION
						
			args[num_args++] = strtok(input, " ");
			
			while ((args[num_args++] = strtok(NULL, " ")) != NULL);

			for (i = 0; i < num_args - 1; i++) {
			
				// Look for input redirection operators
				if (strcmp(args[i], "<") == 0) {
				
				    input_file = args[i + 1];
				    args[i] = NULL;
				    
				} 
				
				// check for output redirection
				else if (strcmp(args[i], ">") == 0) {
				
				    output_file = args[i + 1];
				    args[i] = NULL;
				}
			}
						
			
			pid = fork();
			
			if (pid == 0) {
			
				// child process
				
				if (input_file != NULL) {
				
					// opn input file
					input_fd = open(input_file, O_RDONLY);
					
					if (input_fd == -1) {
					
						fprintf(stderr, "ERROR: INPUT FILE - %s\n", input_file);
						exit(1);
					
					}
					
					dup2(input_fd, STDIN_FILENO);
				}

				if (output_file != NULL) {
				
					// open output file
					output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
					if (output_fd == -1) {
					
						fprintf(stderr, "ERROR: OUTPUT FILE - %s\n", output_file);
						exit(1);
					}
					
					dup2(output_fd, STDOUT_FILENO);
				}

				// Execute the command with the remaining arguments
				execvp(args[0], args);

				// cleanup
				if (input_fd != -1) {
					close(input_fd);
				}
				
				// cleanup
				if (output_fd != -1) {
					close(output_fd);
				}
				
				exit(0);
			
			}
			
			else if (pid > 0) {
				
				// parent
				
				// wait for child to complete
				wait(&stat);
			
			}
			
			else {
			
				fprintf(stderr, "ERROR: FORK");
			
			}
		
		}
		
		else {
		
		// Execution Function -- Seperate
		
		execution_draw(input);
		
		}
    }
	
	return 0;

}
