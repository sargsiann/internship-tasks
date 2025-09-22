#include "header.h"

extern size_t used_stack_bytes;
extern size_t stack_size;
extern int available_fds; 

bool	check_stack_overflow() 
{
	if (stack_size - used_stack_bytes <= 4096) { // left some memory for function calls etc
		fprintf(stderr, "Error: Stack overflow protection triggered. Recursion depth too high.\n");
		return false; // do not allow to continue
	}
	return true; // allow to continue
}

bool	check_remained_fds() 
{
	if (available_fds <= 8) { // should not happen because of check in main
		fprintf(stderr, "Error: Its Dangerous to run the program, Maximum open file descpritors limit is too small\n");
		return false;
	}
	return true;
}

bool	path_check(char *dirname, char *new_dname) 
{
	if (strcmp(new_dname, "..") == 0 || strcmp(new_dname, ".") == 0) //passing same or parent dir for avoid infinite recursion
        return false;
	if (is_valid_name(new_dname) == false) // checking dir name to be less than 4096
	{
		fprintf(stderr,"Error : Directory name is too big %s/%s\n",dirname,new_dname);
		return false;
	}
	size_t dir_len = strlen(dirname);
	size_t new_dirlen = strlen(new_dname);

	if (PATH_MAX - 1 - new_dirlen < dir_len || SIZE_MAX - 1 - dir_len < new_dirlen) // avoiding size_t overflow
	{
		fprintf(stderr,"Error : Directory name is too big %s/%s\n",dirname,new_dname);
		return false;
	}
	return true; // all checks passed
}


bool	is_valid_name(char *d_name)
{
	if (!d_name)
		return false;
	size_t size = 0;
	while (d_name[size])
	{
		if (size >= PATH_MAX)
			return false;
		size++;	
	}
	return true;
}
