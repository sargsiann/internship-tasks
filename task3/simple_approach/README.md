2. System Command (Approach 2)

Constructs the command as a single string:

Checking the Lenght of each argument 
Calculating the lenght of input and formatting the command
sprintf(input, "%s && %s | %s > %s", av[1], av[2], av[3], av[4]);
system(input);


Very Simple implementation using system().