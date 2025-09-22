#include "header.h"

volatile bool stop_traversal = false;
static	bool recursive_mode = false;
uintptr_t stack_size = 0;
int available_fds = 0;
uintptr_t stack_address = 0; 
size_t	 used_stack_bytes = 0;

// The maximum file size is 4,294,967,295 bytes (4 GiB - 1) on Linux systems. so ive chosen uint32_t 
static uint32_t		files_count = 0;
static uint32_t 	dirs_count = 0; 
static uint32_t 	symlinks_count = 0; 
static uint32_t 	sockets_count = 0; 
static uint32_t 	pipes_count = 0;  
static uint32_t 	block_devs_count = 0; 
static uint32_t 	char_devs_count = 0;


void	count_file_types(char d_type) 
{
	if (d_type == DT_REG) 
		files_count++;
	else if (d_type == DT_LNK) 
		symlinks_count++;
	else if (d_type == DT_SOCK)
		sockets_count++;
	else if (d_type == DT_FIFO) 
		pipes_count++;
	else if (d_type == DT_BLK)
		block_devs_count++;
	else if (d_type == DT_CHR)
		char_devs_count++;
}

void	recursive_move_dirs(char *dirname, int	depth) 
{
	char path[PATH_MAX] = {0};
	DIR *dir = opendir(dirname);
	struct dirent *dir_member;

	// FIX 2: ------------------------------------------------------------------------------
	// STACK OVERFLOW PROTECTION, RECURSION LIMIT	
	uintptr_t func_addr_in_stack; // get current stack value from cpu register for current architecture
	get_stack_address(&func_addr_in_stack);
	used_stack_bytes = stack_address - func_addr_in_stack; // how much stack used currently
	// ------------------------------------------------------------------------------	
	if (is_valid_name(dirname) == false) { // checking in any case
		print_by_depth(depth,"Error : Path is too big",dirname,NULL); 
		return;
	}
	if (dir == NULL) {
		print_by_depth(depth,strerror(errno),NULL,NULL);
		return;
	}
	available_fds--; // opening directory costs one fd
	while ((dir_member = readdir(dir)) != NULL)
	{
		if (stop_traversal) { // handling signal interruption
				break;
		}
		if (dir_member->d_type == DT_DIR) {
			// FIX 1: ------------------------------------------------------------------------------
			// PATH BUFFER OVERFLOW, DIR NAME HANDLING, LONGER PATHS,DYNAMIC ALLOC FOR PATH NAME Long pathnames for size_t overflow
			if (path_check(dirname, dir_member->d_name) == false)
				continue;
			//------------------------------------------------------------------------------
			if (!get_path(path, dirname, dir_member->d_name)) // getting full path of directory entry
				continue;
			dirs_count++;
			
			print_by_depth(depth,NULL,dir_member->d_name,NULL);
			if (recursive_mode) {
				// FIX 2-3: ------------------------------------------------------------------------------
				if (!check_remained_fds() || !check_stack_overflow()) {  // remainging stack and fds check
					continue;
				}
				// ------------------------------------------------------------------------------
				recursive_move_dirs(path, depth + 1); // traversing into directory
			}
			continue;
		}
		else if (is_valid_name(dir_member->d_name) == false) {
			print_by_depth(depth,"Error : File name is too big",dir_member->d_name,NULL); 
			continue;
		}
		print_by_depth(depth,NULL,dir_member->d_name,NULL);
		count_file_types(dir_member->d_type);
	}
	if (errno != 0) // check if error occurred during readdir
		fprintf(stderr,"Error reading directory %s: %s\n", dirname, strerror(errno));
	if (closedir(dir) == -1) // check if error occurred during closedir
		fprintf(stderr,"Error closing directory %s: %s\n", dirname, strerror(errno));
	available_fds++; // closing directory frees one file descriptor
}



int main(int argc, char *argv[])
{
	setup_signal_handlers();
	// FIX 2: ------------------------------------------------------------------------------
	if (!get_stack_address(&stack_address) || !get_stack_size()) // GETTING STACK SIZE AND ADDRESS FOR STACK OVERFLOW PROTECTION
		return 1;
	// ------------------------------------------------------------------------------
	// FIX 3: ------------------------------------------------------------------------------
	if (!get_max_fds()) // GETTING MAXIMUM NUMBER OF OPEN FILES FOR PROCESS
		return 1;
	// ------------------------------------------------------------------------------
	if (argv[1] == NULL || argc > 3)
	{
		fprintf(stderr,"Error : Usage ./print_dir <dir_name> -r <optional>\n");
		return 1;
	}
	if (argc > 2) {
		if (strcmp(argv[2], "-r") == 0) 
           recursive_mode = true;
		else {
            fprintf(stderr, "Error: Second argument must be -r\n");
            return 1;
        }
	}
	rm_last_slash(argv[1]); // removing trailing slashes from path to avoid filename lenght issues
	recursive_move_dirs(argv[1],1);
	printf("\nTotal: %d directories, %d regular files, %d symlinks, %d sockets, %d pipes, %d block devices, %d character devices\n", 
		dirs_count, files_count, symlinks_count, sockets_count, pipes_count, block_devs_count, char_devs_count);
	return 0;
}