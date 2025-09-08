#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>



// Struct for command like path and args
typedef	struct cmd {
	char	*path;
	char	**args;
}	t_cmd;

void free_cmd(t_cmd *cmd);

void print_args(t_cmd *cmd);


bool	is_space(char c) 
{
	return (c >= 8 && c < 13) || c == 32;
}

bool	is_path(char *p_comm) {
	if (strlen(p_comm) < 2)
		return false;
	if (p_comm[0] == '/' || (p_comm[0] == '.' && p_comm[1] == '/'))
		return true;
	return false;
}

char	*get_cmd_path(char *cmd) 
{
	if (!cmd || !*cmd)
		return NULL;
	while (*cmd && is_space(*cmd) || (*cmd == 39 || *cmd == 34))
		cmd++;

	char	*cmd_end = strstr(cmd," ");
	char	*pure_command;
	if (cmd_end) // its not single command
	{
		*cmd_end = 0;//putting 0 temporary 
		pure_command = strdup(cmd);
		*cmd_end = ' '; //restoreing
	}
	else
		pure_command = strdup(cmd);

	if (is_path(pure_command)) // if its /bin/ls or ./a.out or ../a.out just return it
		return pure_command;
	// Getting paths of executables to find like /bin/cat 
	char	*env = getenv("PATH");

	// If path was removed from enviromental variables return NULL;
	if (!env)
		return NULL;

	env = strdup(env); // making copy because srttok changes the value of orignal

	char	*exes = strtok(env,":"); // getting the first exe path before :
	char	path[1024] = {0};
	while (exes != NULL)
	{
		sprintf(path,"%s/%s",exes,pure_command); // creating full path by command usr/bin/cmd
		if (access(path,X_OK) == 0) { // check if file executable and exist X_OK MEANS EXECUTABLE
			break;
		}
		memset(path,0,1024); // putting 0 to path
		char	*tmp_ptr_to_free = env;
		exes = strtok(NULL,":"); // getting the path before first : 
	}
	free(env); // freeing dupped memory
	free(pure_command);
	return strdup(path); // returning mem segment from heap because cant return pointer of local var stored in stack 
	// cause func stacks clear after its execution
}



int		get_args_count(char *cmd) 
{
	int i = 0;
	while (is_space(cmd[i]))
		i++; // going for first spaces
	if (!cmd[i]) // if all are spaces
		return 0;
	int	count = 1; // 1 for command name
	while (cmd[i] != 0)
	{
		if (!is_space(cmd[i] - 1) && is_space(cmd[i])) // if we got case like "a " it means its an argument we also count command name as execve needs it to
			count++;
		i++;
	}
	return count;
}

char **get_cmd_args(char *line) {
    if (!line) 
		return NULL;

    int args_c = get_args_count(line);
    char **args = malloc((args_c + 1) * sizeof(char *));
    if (!args) return NULL;

    int i = 0;
    char *token = strtok(line, " \t\n");// splitting by space
    while (token != NULL) {
		args[i++] = strdup(token); // copy each token
		token = strtok(NULL, " \t\n");
    }
    args[i] = NULL; // NULL terminate

    return args;
}


void free_args(char **args) {
    if (!args) 
		return;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

// The part for executing right prog and side res by pipe and putting it to file
void exec_cmds(t_cmd *c1, t_cmd *c2, t_cmd *c3, char *filename) {
    int pipefd[2]; // our pipe array 0 for read 1 for write
    pipe(pipefd); // create pipe regardless of c2 success

    // first process
	pid_t pid1 = fork();
    if (pid1 == 0) {
        execve(c1->path, c1->args, NULL);
        perror("execve c1 failed");
		// FIX : if c1 fails we need to free child process memory open pipe descriptors and exit
		free_cmd(c1);
		free_cmd(c2);
		free_cmd(c3);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(EXIT_FAILURE);
    }
	int status1;
    waitpid(pid1, &status1, 0); // checkinf for pid1
	if (WIFEXITED(status1) > 0) 
	{
		int exit_code = WEXITSTATUS(status1);
		if (exit_code != 0) { // if our command first ended with error we dont continue as logic wants
			close(pipefd[1]); // closing pipe inputs
			close(pipefd[0]);
			return ; // going out to free memory
		}
	}

    // if c1 ok executing second command
    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[0]);              // close read end
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipefd[1]); // closing write end
        execve(c2->path, c2->args, NULL); // executeing
        perror("execve c2 failed");
		return ; // going out to free memory
	}
    close(pipefd[1]); // parent closes write end

    // Fork c3 → always read from pipe
    pid_t pid3 = fork();
    if (pid3 == 0) {

		int	file_fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,0644);
		if (file_fd == -1) {
			perror("Open");
			return ; // going out to free memory		
		}
		dup2(file_fd,STDOUT_FILENO); // redirecting stdout to file
        dup2(pipefd[0], STDIN_FILENO); // redirect stdin from pipe
		close(pipefd[0]); // dont closeing pipefd 1 because closed in parent
        execve(c3->path, c3->args, NULL);
        perror("execve c3 failed");
		return ;      
    }
    close(pipefd[0]); // parent closes read end

    // Wait for c2 and c3
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
}

void print_args(t_cmd *cmd)
 {
    if (!cmd || !cmd->args) return;

    printf("Command: %s\nArguments:\n", cmd->path);
    for (int i = 0; cmd->args[i] != NULL; i++) {
        printf("  argv[%d] = '%s'\n", i, cmd->args[i]);
    }
}

void free_cmd(t_cmd *cmd) {
    if (!cmd) 
		return;

    // Free path
    if (cmd->path) {
        free(cmd->path);
        cmd->path = NULL;
    }

    // Free args
    if (cmd->args) {
       free_args(cmd->args);
	   cmd->args = NULL;
    }
}

int main(int ac, char *av[]) 
{ 	
	// Checking the args size
	if (ac != 5) {
		fprintf(stderr,"Error : usage is ./exec [p1] [p2] [p3] [file]\n");
		return 1;
	}
	t_cmd cmd1;
	cmd1.args = get_cmd_args(av[1]);
	cmd1.path = get_cmd_path(av[1]);

	
	t_cmd cmd2;
	cmd2.args = get_cmd_args(av[2]);
	cmd2.path = get_cmd_path(av[2]);
	
	t_cmd cmd3;
	cmd3.path = get_cmd_path(av[3]);
	cmd3.args = get_cmd_args(av[3]);

	exec_cmds(&cmd1,&cmd2,&cmd3, av[4]);
	// Even after exiting child process with return we need to free memory in each process address space
	// because its allocated in heap and not in stack
	// so each process have its own copy of heap memory
	// so we need to free it in each process
	// so we free it in parent process after all childs ended
	free_cmd(&cmd1);
	free_cmd(&cmd2);
	free_cmd(&cmd3);
}