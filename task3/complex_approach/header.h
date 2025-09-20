#ifndef HEADER_H
#define HEADER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <wordexp.h>
#include <errno.h>

typedef struct s_cmd 
{
	int		path_fd; // file descriptor of command executable
	char	**args; // arguments of command
	time_t	mod_time; // modification time of executable to check if it was changed during runtime
}	t_cmd;

void 	free_cmds(t_cmd *c1, t_cmd *c2, t_cmd *c3);
void 	free_cmd(t_cmd *cmd);
void 	free_args(char **args);
bool 	is_path(char *p_comm);
char 	**get_cmd_args(char *line);
void 	get_cmd_path(t_cmd *cmd_struct);
int 	get_file_fd(char *path, t_cmd *cmd_str);
time_t 	get_mod_time(int fd);
long	env_size();
void	print_args(t_cmd *cmd);
void	error_and_exit(char *msg);
bool	exec_cmd1(t_cmd *c1, t_cmd *c2, t_cmd *c3);
bool	exec_cmd2(t_cmd *c1, t_cmd *c2, t_cmd *c3, int pipefd[]);
bool	exec_cmd3(t_cmd *c1, t_cmd *c2, t_cmd *c3, char *filename, int pipefd[]);
void	exec_cmds(t_cmd *c1, t_cmd *c2, t_cmd *c3, char *filename);
void	signal_handler() ;
void	set_signal_handlers() ;
void	block_signals(sigset_t *old_set) ;
void	unblock_signals(sigset_t *old_set) ;
bool	get_users_answer() ;

#endif