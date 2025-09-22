#include "header.h"

extern bool stop_traversal;

unsigned long	get_free_ram()  
{
	char	buff[255]; // the buff max can be 30~40 for each line /proc/meminfo
	FILE *fp;
	unsigned long av_ram = 0;

	fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        return -1;
    }
	while (fgets(buff, sizeof(buff), fp) != NULL) {
        if (strncmp(buff, "MemAvailable:", 13) == 0) {
            sscanf(buff, "MemAvailable: %lu kB", &av_ram);
            break;
        }
		if (stop_traversal)
			break; // checking if signal interrupted
        else if (strncmp(buff, "MemFree:", 8) == 0) {
            sscanf(buff, "MemFree: %lu kB", &av_ram); 			// on old kernels memfree is used instead of MemAvailable
			break;
        }
    }
	if (fclose(fp) != 0) {
		perror("Error closing /proc/meminfo");
		return -1;
	}
	return av_ram;
}

void print_by_depth(size_t depth, char *message, char *file_name, char *parent) 
{

	printf("|");
    for (size_t i = 0; i < depth*2; i++) {
        printf("-");
	}
	printf(" ");
    if (message != NULL) {
        printf("%s ", message);
	}
	if (parent != NULL)
		printf("%s/",parent);
	if (file_name)
    	printf("%s\n", file_name);
}

bool get_path(char *path, char *dirname, char *dname) 
{
	size_t len = strlen(dirname) + strlen(dname) + 2; // +2 for '/' and '\0'
	if (len > PATH_MAX) {
		fprintf(stderr,"Path length exceeds maximum limit\n");
		return false;
	}
	int written_bytes = snprintf(path,len, "%s/%s", dirname, dname);
	if (written_bytes < 0 || (size_t)written_bytes >= len)
	{
		fprintf(stderr,"Sprintf error");
		return false;
	}
	return true; // success
}


void	rm_last_slash(char *path)  // removing trailing slashes from path to avoid filename lenght issues
{
	if (!path)
		return;
	size_t len = strlen(path);
	while (path[len - 1] == '/' && len > 1) 
	{
		path[len - 1] = '\0';
		len--;
	}
}