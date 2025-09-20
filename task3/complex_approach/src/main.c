#include "header.h"


int		ret_status = 0;
extern char **environ;



void get_cmd_path(t_cmd *cmd_struct) 
{
	if (!cmd_struct || !(*cmd_struct->args) || !(*cmd_struct->args[0])) //Null checking
		return ;
	
	char	*cmd = cmd_struct->args[0];
	int		path_fd;
	if (is_path(cmd)) { 
		get_file_fd(cmd,cmd_struct);
		return ;
	}
	char	*env = getenv("PATH");
	if (!env) {
		cmd_struct->path_fd = -1;
		return ;
	}


	env = strdup(env); 
	if (!env)
	{
		cmd_struct->path_fd = -1;
		perror("Env strdup");
		return ;
	}

	char	*exes = strtok(env,":");
	char	path[PATH_MAX] = {0}; // the max path size that programm can handle
	int		written;
	while (exes != NULL)
	{
		written = snprintf(path, sizeof(path), "%s/%s", exes, cmd);
		if (written < 0 || written >= PATH_MAX) 
		{
			cmd_struct->path_fd = -1;
			free(env);
			fprintf(stderr, "Error: Path length for command '%s' is too long or snprintf error occurred.\n", cmd);
			return ;
		}
		// using fd s instead of access For avoid TOCTOU 
		// (If we have file descriptor we sure that we working exactly with that executable and not swapped file)
		if ((path_fd = get_file_fd(path,cmd_struct)) != -1)
		{
			free(env);
			return ;
		}
		exes = strtok(NULL,":"); // getting the path before first : 
	}
	free(env); // freeing dupped memory
}

int main(int ac, char *av[]) 
{ 
	if (ac != 5) {
		fprintf(stderr,"Error : usage is ./exec [p1] [p2] [p3] [file]\n");
		return 1;
	}

	if (!environ)
	{
		fprintf(stderr,"Error : Environ is NULL\n");
		return 1;
	}

	// FIX 4: not changing original av wordstat dont work with original strings
	t_cmd cmd1 = { .path_fd = -1, .mod_time = -1, .args = NULL };
	cmd1.args = get_cmd_args(av[1]);
	get_cmd_path(&cmd1);
	if (cmd1.path_fd == -1) 
	{
		fprintf(stderr,"p1 : invalid command\n");
		free_cmd(&cmd1);
		return 1;
	}

	t_cmd cmd2 ={ .path_fd = -1, .mod_time = -1, .args = NULL };
	cmd2.args = get_cmd_args(av[2]);
	get_cmd_path(&cmd2);	
	

	t_cmd cmd3 = { .path_fd = -1, .mod_time = -1, .args = NULL };
	cmd3.args = get_cmd_args(av[3]);
	get_cmd_path(&cmd3);
	exec_cmds(&cmd1,&cmd2,&cmd3, av[4]);
	free_cmds(&cmd1,&cmd2,&cmd3);

	return ret_status;
}