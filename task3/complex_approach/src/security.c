#include "header.h"

extern char **environ;

// FIX2: using fd s instead of access For avoid TOCTOU
int		get_file_fd(char *path, t_cmd *cmd_str) 
{
	if (path == NULL || cmd_str == NULL)
		return -1;
	int file_fd = open(path,__O_PATH);
	if (file_fd == -1)
		return -1;
	struct stat file_stat;
	if (fstat(file_fd,&file_stat) == 0 && file_stat.st_mode & (S_IXUSR| S_IXGRP | S_IXOTH)) { // getting file
		cmd_str->path_fd = file_fd; 
		cmd_str->mod_time = file_stat.st_mtime; // getting last modification time to prevent toctou
		return 1;
	}
	cmd_str->path_fd = -1;
	cmd_str->mod_time = -1;
	close(file_fd);
	return -1;
}

// Getting modification time of file to understand whether file changes during runtime
time_t	get_mod_time(int fd)  
{
	struct stat file_stat;

	if (fstat(fd,&file_stat) == 0 && file_stat.st_mode & (S_IXUSR| S_IXGRP | S_IXOTH))  //getting last modification time to prevent toctou
		return file_stat.st_mtime;
	return -1;
}


long env_size() 
{
	long size = 0;
	int	i = 0;
	while (environ[i])
	{
		size += strlen(environ[i]);
		i++;
	}	
	return size;
}