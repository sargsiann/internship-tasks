#include "header.h"

extern size_t recursion_size_bytes;
extern size_t used_stack_bytes;
extern size_t stack_size;
extern int max_open_files; 

bool	check_stack_overflow() 
{
	if (used_stack_bytes + 2 * recursion_size_bytes >= stack_size - 256) { // left some memory for function calls etc
		fprintf(stderr, "Error: Stack overflow protection triggered. Recursion depth too high.\n");
		return false; // do not allow to continue
	}
	return true; // allow to continue
}

bool	check_remained_fds() 
{
	if (max_open_files <= 16) { // should not happen because of check in main
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
	if (strlen(dirname) + strlen(new_dname) > PATH_MAX - 1 || strlen(dirname) + strlen(new_dname) + 1 > SIZE_MAX) // avoiding size_t overflow
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
