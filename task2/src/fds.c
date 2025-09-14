#include "header.h"

extern int max_open_files; // maximum number of open files

bool	get_max_fds() 
{
	struct rlimit fd_limit;
	if (getrlimit(RLIMIT_NOFILE, &fd_limit) == -1) {
		perror("getrlimit");
		return false;
	}
	printf("Maximum number of open files: %ld\n", fd_limit.rlim_cur);
	max_open_files = fd_limit.rlim_cur;
	if (max_open_files < 16) { // if max open files is less than 16
		fprintf(stderr, "Error: Its Dangerous to run the program, Maximum open files limit is too small\n");
		return false;
	}
	return true;
}