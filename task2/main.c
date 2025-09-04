#include <stdio.h> // for printing
#include <dirent.h> // for work with directories
#include <string.h>
#include <stdbool.h>

// By default recursive mode is false
static	bool recursive_mode = false; 
// number of files and dirs
static	int	files_count = 0; // are regular files with data
static	int dirs_count = 0; // directories that contain files dirs .. etc
static	int symlinks_count = 0; // symbolic links like shortucts that point to other dirs
static	int sockets_count = 0; // files for network communication
static	int pipes_count = 0; // named pipes for inter process comunication 
static	int block_devs_count = 0; // usb hardware devices transfering data by block (i dont know more)
static	int char_devs_count = 0; // terminals io s ...



// Function for printing by depth tabs right for better undestanding of srtuct of whole directory
void print_by_depth(int depth, char *message, char *file_name, char *parent) 
{

	printf("|");
    for (int i = 0; i < depth*2; i++)     // Print tabs by depth
        printf("-");
	printf(" ");
    if (message != NULL) // Print message (if any)
        printf("%s ", message);
	if (parent != NULL)
		printf("%s/",parent);
    printf("%s\n", file_name);
}


void	recursive_move_dirs(char *dirname, int	depth, char *parent) 
{
	DIR *dir = opendir(dirname); //opening directory
	struct dirent *dir_member;
	
	if (dir == NULL)
	{
		print_by_depth(depth,"[error opening dir]",dirname,NULL); //
		return ;
	}
	// Continiously assignin to dir_member readed member of directory
	while ((dir_member = readdir(dir)) != NULL)
	{
			char path[1024];

		// Getting ful path because opendir works with full path
		if (dir_member->d_type == DT_DIR) {  // if we got directory in other directory we calling recursive function
			sprintf(path, "%s/%s", dirname, dir_member->d_name);
			dirs_count++;
			// skipping . and .. directories to avoid infinite loop
			if (strcmp(dir_member->d_name, "..") == 0 || strcmp(dir_member->d_name, ".") == 0)
        		continue;
			print_by_depth(depth,NULL,path,NULL);
			
			if (recursive_mode) { // going to subdir if recursive mode is available
				 // add ing tou previous dirname like etc new dirname like etc/var and opneinng like that
				// in each sprintf call it will make relative call and recursively adding next call will be 
				// like etc/var/hosts ... we could use chdir to change directory and open by its simple name but chdir is disallowed
				recursive_move_dirs(path, depth + 1, dir_member->d_name);
			}
			continue;
		}
		else if (dir_member->d_type == DT_REG) 
			files_count++;
		else if (dir_member->d_type == DT_LNK) 
			symlinks_count++;
		else if (dir_member->d_type == DT_SOCK)
			sockets_count++;
		else if (dir_member->d_type == DT_FIFO) 
			pipes_count++;
		else if (dir_member->d_type == DT_BLK)
			block_devs_count++;
		else if (dir_member->d_type == DT_CHR)
			char_devs_count++;
		print_by_depth(depth,NULL,dir_member->d_name,parent);
	}	
	closedir(dir);
}

int main(int argc, char *argv[]) {
	if (argv[1] == NULL) // we must have first argument to print dir struct
	{
		fprintf(stderr,"Error : Usage ./print_dir <dir_name> -r <optional>\n"); // print in stderror
		return 1;
	}
	if (argc > 2) {
		 if (strcmp(argv[2], "-r") == 0) 
            recursive_mode = true;
		else {
            fprintf(stderr, "Error: Second argument must be -r\n"); // print in stderr
            return 1;
        }
	}
	
	recursive_move_dirs(argv[1],1,NULL);
	printf("\nTotal: %d directories, %d regular files, %d symlinks, %d sockets, %d pipes, %d block devices, %d character devices\n", 
		dirs_count, files_count, symlinks_count, sockets_count, pipes_count, block_devs_count, char_devs_count);
	return 0;
}