#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef ARG_MAX // maximum argument lenght in shell
# define ARG_MAX 32000
#endif

int main(int ac, char **av) {
	// Checking for the correct number of arguments
	if (ac != 5) {
		fprintf(stderr, "Error: ./program p1 p2 p3 file\n");
		return 1;
	}
	int total_len = strlen(av[1]) + strlen(av[2]) + strlen(av[3]) + strlen(av[4]) + strlen(" &&   |  > ") + 1; // getting the total length of the command
	char *input = (char *)malloc(total_len);
	if (!input) {
		perror("Failed to allocate memory");
		return EXIT_FAILURE;
	}

	int	bytes_written = sprintf(input, "%s && %s | %s > %s", av[1], av[2], av[3], av[4]); // constructing the command;
	if (bytes_written < 0  || bytes_written > ARG_MAX) {
		if (bytes_written < 0)
			fprintf(stderr,"Error : Sprintf failed");
		if (bytes_written > ARG_MAX)
			fprintf(stderr,"Error : Argument lenght is more than system can handle");
		free(input); // freeing the allocated memory
		return EXIT_FAILURE;
	}
	system(input); // executing the command
	free(input); // freeing the allocated memory
	return 0;
}