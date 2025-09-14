#ifndef HEADER_H
#define HEADER_H


// FIX IN CASE IF PATH MAX DONT DEFINED
#ifndef PATH_MAX
 #define PATH_MAX 4096
#endif

#include <stdio.h>
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <signal.h>


unsigned long	get_free_ram();
void			print_by_depth(size_t depth, char *message, char *file_name, char *parent);
bool			is_valid_name(char *d_name);
bool 			get_stack_address(uintptr_t *stack_address);
void			handle_signal(int signal);
void			setup_signal_handlers();
bool			get_stack_size();
bool			get_max_fds();
bool			check_stack_overflow();
bool			check_remained_fds();
bool			path_check(char *dirname, char *new_dname);
char			*get_path(char *dirname, char *dname) ;
void			rm_last_slash(char *path);

#endif