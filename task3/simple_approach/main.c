#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av) {
	// Checking for the correct number of arguments
	if (ac != 5) {
		fprintf(stderr, "Error: ./program p1 p2 p3 file\n");
		return 1;
	}
	int total_len = strlen(av[1]) + strlen(av[2]) + strlen(av[3]) + strlen(av[4]) + strlen(" &&   |  > "); // getting the total length of the command
	char *input = (char *)malloc(total_len);
	if (!input) {
		perror("Failed to allocate memory");
		return EXIT_FAILURE;
	}
	sprintf(input, "%s && %s | %s > %s", av[1], av[2], av[3], av[4]); // constructing the command
	system(input); // executing the command
	free(input); // freeing the allocated memory
	return 0;
}