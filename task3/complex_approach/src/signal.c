#include "header.h"

extern volatile sig_atomic_t interrupted;

void signal_handler() // if process got singal sigint or sigterm changind the interupted to true
{
    interrupted = 1;
}

void	set_signal_handlers() 
{
	struct sigaction sa = {.sa_handler = signal_handler, .sa_flags = 0}; // setting up the struct for signlar handler
	sigemptyset(&sa.sa_mask); // setting sig handler to only our defined signals
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGTERM,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);
}

void	block_signals(sigset_t *old_set) 
{
	sigset_t block_set;
	sigemptyset(&block_set); // clearing blocked signals data structure
	sigaddset(&block_set,SIGINT);
	sigaddset(&block_set,SIGTERM);
	sigaddset(&block_set,SIGCHLD);
	sigprocmask(SIG_BLOCK,&block_set,old_set);
}

void	unblock_signals(sigset_t *old_set) 
{
	sigprocmask(SIG_SETMASK,old_set,NULL);
}