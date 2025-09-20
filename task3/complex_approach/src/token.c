#include "header.h"
// FIX 4: using wordexp for complex parsing like nested quotes 
// WORDEXP PARSES GIVEN LINE LIKE SHELL SO ITS GOOD UTILITY TO PARSE
char **get_cmd_args(char *line) 
{
	long	arg_max = sysconf(_SC_ARG_MAX) - env_size();	
	
	if (!line || !*line)
		return NULL;
	wordexp_t p;
	if (wordexp(line, &p, WRDE_NOCMD) != 0)  //using WRDE_NOCMD for security reasons to avoid $() command running
		return NULL;
	char **args = malloc((p.we_wordc + 1) * sizeof(char *));
	if (!args) {
		wordfree(&p);
		return NULL;
	}	
	for (size_t i = 0; i < p.we_wordc; i++) {
		args[i] = strdup(p.we_wordv[i]);
		if (arg_max < 1000) {
			free_args(args); // free previously allocated args
			wordfree(&p);
			fprintf(stderr,"Reached Argument Maximum Memory for process\n");
			return NULL;
		}
		if (!args[i]) {
			free_args(args); // free previously allocated args
			wordfree(&p);
			return NULL;
		}
		arg_max -= strlen(args[i]);
	}
	args[p.we_wordc] = NULL;
	wordfree(&p);
	return args;
}

