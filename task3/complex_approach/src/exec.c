#include "header.h"

extern int ret_status;
extern	char **environ;

pid_t pid2;
pid_t pid3;

volatile sig_atomic_t interrupted = 0;




bool	exec_cmd1(t_cmd *c1, t_cmd *c2, t_cmd *c3) 
{
	// In parent process
	set_signal_handlers();

	sigset_t old_set;
	block_signals(&old_set);
	// Fix 3
	pid_t pid1 = fork();
	if (pid1 == -1) { // FIX5 : checking system calls return values
		perror("fork");
		ret_status = 1;
		unblock_signals(&old_set);
		return false;
	}
    if (pid1 == 0) {
		signal(SIGINT,SIG_DFL);
		signal(SIGTERM,SIG_DFL);
		unblock_signals(&old_set);
		if (c1->mod_time != get_mod_time(c1->path_fd) || c1->mod_time < 0 ) // if file was changed
			fprintf(stderr,"Error: file was changed during runtime\n");
		else {
        	fexecve(c1->path_fd, c1->args, environ);
        	perror("execve c1 failed");
		}
		free_cmds(c1, c2, c3);
		exit(EXIT_FAILURE);
    }
	
	int status1;

	if (waitpid(pid1,&status1,0) == -1 || interrupted) 
	{
		if (!interrupted)
			perror("Programm 1 Failed");
		else
			printf("INTERUPTED YEAHH");
		unblock_signals(&old_set);
		kill(SIGTERM,pid1);
		return false;
	}
	if ((WIFEXITED(status1) && WEXITSTATUS(status1) > 0) || WIFSIGNALED(status1))
	{
		fprintf(stderr, "Prog 1 %s\n", WIFSIGNALED(status1) ? "terminated by signal" : "exited with error");
		sigprocmask(SIG_SETMASK, &old_set, NULL);
		return false;
	}
	return true;
}

bool	exec_cmd2(t_cmd *c1, t_cmd *c2, t_cmd *c3, int pipefd[]) 
{
	sigset_t old_set;

	block_signals(&old_set);
	
	pid2 = fork();
    if (pid2 == -1) {// FIX5 : checking system calls return values
		unblock_signals(&old_set);
		perror("fork");
		ret_status = 1;
		close(pipefd[0]);
		close(pipefd[1]);
		return false;
	}
	if (pid2 == 0) {
		signal(SIGINT,SIG_DFL);
		signal(SIGTERM,SIG_DFL);
		sigprocmask(SIG_SETMASK,&old_set,NULL);
        close(pipefd[0]);    
		if (dup2(pipefd[1], STDOUT_FILENO) == -1) {// FIX5 : checking system calls return values  
			perror("dup2");
			close(pipefd[1]);
			free_cmds(c1, c2, c3);
			exit(EXIT_FAILURE);
		}
		if (c2->mod_time != get_mod_time(c2->path_fd) || c2->mod_time < 0) // if file was changed
			fprintf(stderr,"Error: file was changed during runtime\n");
		else
        	fexecve(c2->path_fd, c2->args, environ);
        perror("execve c2 failed");
		close(pipefd[1]);
		// FIX 3: exiting in child processes
		free_cmds(c1, c2, c3);
		exit(EXIT_FAILURE);
	}
    close(pipefd[1]); 
	unblock_signals(&old_set);
	return true;
}

bool	exec_cmd3(t_cmd *c1, t_cmd *c2, t_cmd *c3, char *filename, int pipefd[]) 
{
	sigset_t old_set;
	block_signals(&old_set);
	pid3 = fork();
	if (pid3 == -1) {
		unblock_signals(&old_set);
		perror("fork");
		ret_status = 1;
		close(pipefd[0]);
		return false;
	}
    if (pid3 == 0) {
		signal(SIGINT,SIG_DFL);
		signal(SIGTERM,SIG_DFL);
		unblock_signals(&old_set);
		int	file_fd = open(filename,O_WRONLY | O_CREAT | O_EXCL | O_TRUNC,0644); // not overwriting existing files
		if (file_fd == -1) {
			close(pipefd[0]);
			perror("Open");
			// FIX 3: exiting in child processes
			free_cmds(c1, c2, c3);
			exit(EXIT_FAILURE);
		}
		if (dup2(file_fd, STDOUT_FILENO) == -1) {// FIX5 : checking system calls return values
			close(pipefd[0]);
			close(file_fd);
			free_cmds(c1, c2, c3);
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		if (dup2(pipefd[0], STDIN_FILENO) == -1) {// FIX5 : checking system calls return values
			close(pipefd[0]);
			close(file_fd);
			free_cmds(c1, c2, c3);
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		if (c3->mod_time != get_mod_time(c3->path_fd) || c3->mod_time < 0) // if file was changed
			fprintf(stderr,"Error: file was changed during runtime\n");
		else
        	fexecve(c3->path_fd, c3->args, environ);
        perror("execve c3 failed");
		close(pipefd[0]);
		// FIX 3: exiting in child processes
		close(file_fd);
		free_cmds(c1, c2, c3);
		exit(EXIT_FAILURE);
    }
	close(pipefd[0]);
	unblock_signals(&old_set);
	return true;
}

// The part for executing right prog and side res by pipe and putting it to file
void exec_cmds(t_cmd *c1, t_cmd *c2, t_cmd *c3, char *filename)
 {

	if (exec_cmd1(c1,c2,c3) == false)
	{
		ret_status = 1;
		return ;
	}
	
	int pipefd[2];
 	if (pipe(pipefd) == -1) {// FIX5 : checking system calls return values
		perror("pipe");
		ret_status = 1;
		return ;
	}
	
    exec_cmd2(c1,c2,c3,pipefd);
	exec_cmd3(c1,c2,c3,filename,pipefd);


	int status3;
	if (waitpid(pid3,&status3,0) == -1 || interrupted) 
	{
		if (interrupted) {
			fprintf(stderr,"Interrupted pid3");
			kill(pid3,SIGTERM);
		}
		else
			perror("Error : Waiting for prog3");
		ret_status = 1;
	}
	else if ((WIFEXITED(status3) && WEXITSTATUS(status3) > 0) || WIFSIGNALED(status3)) 
	{
		fprintf(stderr, "Prog 3 %s\n", WIFSIGNALED(status3) ? "terminated by signal" : "exited with error");
		ret_status = 1;
	}
	else if (WIFEXITED(status3) && WEXITSTATUS(status3) == 0) {
		printf("Prog 3 exited successfully\n");
		ret_status = 0;
	}

	int status2;
	if (waitpid(pid2,&status2,0) == -1 || interrupted) {
		if (interrupted) {
			fprintf(stderr,"Interrupted pid2");
			kill(pid2,SIGTERM);
		}
		else
			perror("Error : Waiting for prog2");
		ret_status = 1;
	}
	else if ((WIFEXITED(status2) && WEXITSTATUS(status2) > 0) || WIFSIGNALED(status2)) 
	{
		fprintf(stderr, "Prog 2 %s\n", WIFSIGNALED(status2) ? "terminated by signal" : "exited with error");
		ret_status = 1;
	}
	else if (WIFEXITED(status2) && WEXITSTATUS(status2) == 0) {
		printf("Prog 2 exited successfully\n");
		ret_status = 0;
	}
}
