#include "header.h"

// FIX4 --------------------------------------------------------------------------------
// FOR handling signals
extern bool stop_traversal;
void handle_signal(int signal) 
{
	if (signal == SIGINT) {
		fprintf(stderr, "\nTraversal interrupted by user (SIGINT). Exiting gracefully...\n");
	}
	if (signal == SIGQUIT) {
		fprintf(stderr, "\nTraversal interrupted by user (SIGQUIT). Exiting gracefully...\n");
	}
	if (signal == SIGSEGV) {
		fprintf(stderr, "\nError: Segmentation fault (SIGSEGV) encountered. Possible stack overflow due to deep recursion.\n");
	}
	if (signal == SIGBUS) {
		fprintf(stderr, "\nError: Bus error (SIGBUS) encountered.\n");
	}
	if (signal == SIGTERM) {
		fprintf(stderr, "\nError: Termination signal (SIGTERM) received. Exiting...\n");
	}

	stop_traversal = true;
}



void	setup_signal_handlers() {
	// SETTING UP SIGNAL HANDLERS
	signal(SIGINT, handle_signal); // handle Ctrl+C
	signal(SIGSEGV, handle_signal); // handle segmentation fault
	signal(SIGBUS, handle_signal); // handle bus error
	signal(SIGQUIT, handle_signal); // handle Ctrl+ "\"
	signal(SIGTERM, handle_signal); // handle termination signal 
	signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE to prevent termination when writing to a closed pipe
}

// ------------------------------------------------------------------------------