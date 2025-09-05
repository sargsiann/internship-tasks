Task #3 – Execute Shell Pipeline in C

This project demonstrates two approaches to implement the shell command:

prog1 && prog2 | prog3 > file

Approaches
1. Fork + execve (Approach 1)

Uses fork() and execve() to run the programs.

Conditional execution:

prog1 must succeed (exit status 0) for the pipeline to continue.

prog2 writes to a pipe; if prog2 fails, prog3 still runs.

prog3 writes output to the file specified as the 4th argument.

Fully uses system calls, no system() function.

2. System Command (Approach 2)

Constructs the command as a single string:

Calculating the lenght of input and formatting the command
sprintf(input, "%s && %s | %s > %s", av[1], av[2], av[3], av[4]);
system(input);


Simpler implementation using system().
