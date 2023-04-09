
## CPDP Assignment - 2

The following assignment includes the following files
-  toolkit.c
-  mytimeout.c
-  makefile
-  testcase.tc
-  readme.c

The *makefile* file is used to compile the given programs 
1. To run the make file
`$ make`
2. To test with sample testcases
`$ make sample_run`
3. To clean the executables
`$ make clean`

The program *toolkit.c*, written in C programming language, is an implementation of UNIX system toolkit for process and file management. Although, it is simslar to UNIX shells, it supports the following 
- [x] mycd -- User defined cd command
- [x] mypwd -- user defined pwd command
- [x] myexit -- user defined exit command
- [x] all other unix commands, including standalone programs
- [x] mytimeout.x command -- a timelimit command
- [x] Piping (can extend to infinite piping)
- [x] IO Redirection
- [x] If taken commands as input, the toolkit terminates when EOF is encountered

Note: This mytoolkit.c is based on the assumption *Pipes ans IO redirection will not occur together*.

The program *mytieout.c*, a C program, will run a command within the given time limit *snds* (user input) seconds. 
Basic implementation of mytimeout.x (the executable file of mytimeout.c) is :
```
$ gcc mytimeout.c -o mytimeout.x 
$ ./mytimeout.x 5 sleep 10
OUTPUT: Command terminated by timeout
```
The file *testcase.tc*, include some of the sample test cases to run the program

  Some of the commands while executing toolkit.x *(executable of toolkit.c)* are:
  1. mycd -- works same as cd command -- changes the directory
  `$ mycd [dir_name]`
  2. mypwd -- works same as pwd command -- outputs current directory
  `$ mypwd`
  3. myexit -- exits from the toolkit
  `$ myexit`
  4. Any external unix commands (for example: ls, cat, sort, etc. and standalone programs)
  `$ cmd [args]` -- the syntax for the commands are similar of the UNIX shell
  5. mytimeout,x -- a time limiting command
  `$ ./mytimeout.x snds cmd [args]` -- here *cmd* is the command to be time limited and *snds* is the time limit
  6. Piping -- can extend up to as many as pipes as possible
  `$ cmd1 | cmd2 | cmd3 | ...`
  7.  IO Redirection
  `$ cmd [args] < input_file > output_file // both IO redirection`
  `$ cmd [args] < input_file // input redirection`
  `$ cmd [args] > outputfile // output redirection`

### Execution steps
1. run makefile - `$ make`
2. run toolkit.x - 
`$ ./toolkit.x`
`$ make sample_run // to run sample testcases`
3. execute commands in toolkit.x - `$ cmd [args]`
4. use myexit command to exit the toolkit  - `$ myexit`
5. clean the executables - `$ make clean`
