#include "header.h"


void print_args(t_cmd *cmd)
{
    if (!cmd || !cmd->args) return;
    printf("Command: %s\nArguments:\n", cmd->args[0]);
    for (int i = 0; cmd->args[i] != NULL; i++) {
        printf("  argv[%d] = '%s'\n", i, cmd->args[i]);
    }
}


void	error_and_exit(char *msg) 
{
	perror(msg);
	exit(EXIT_FAILURE);
}



void free_cmd(t_cmd *cmd) {
    if (!cmd) 
		return;

    // Free path
    if (cmd->path_fd != -1) {
        close(cmd->path_fd); // closing fd
    }
    // Free args
    if (cmd->args) {
       free_args(cmd->args);
	   cmd->args = NULL;
    }
}

void	free_args(char **args) 
{
	if (!args) 
		return;
	for (int i = 0; args[i] != NULL; i++)
		free(args[i]);
	free(args);
}


bool	is_path(char *p_comm) \
{
	if (strlen(p_comm) < 2)
		return false;
	if (p_comm[0] == '/' || (p_comm[0] == '.' && p_comm[1] == '/'))
		return true;
	return false;
}

void	free_cmds(t_cmd *c1, t_cmd *c2, t_cmd *c3) 
{
	free_cmd(c1);
	free_cmd(c2);
	free_cmd(c3);
}
