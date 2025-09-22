#include "header.h"

extern size_t stack_size;

bool get_stack_address(uintptr_t *stack_address) 
{
    if (!stack_address) {
        fprintf(stderr, "Error: NULL pointer for stack_address\n");
        return false;
    }
	#if defined(__x86_64__) // 64 bit architecture
	    __asm__("mov %%rsp, %0" : "=r"(*stack_address));
	#elif defined(__i386__) // 32 bit architecture
	    __asm__("mov %%esp, %0" : "=r"(*stack_address));
	#elif defined(__arm__) // ARM 32 bit
	    __asm__("mov %0, sp" : "=r"(*stack_address));
	#elif defined(__aarch64__) // ARM 64 bit
	    __asm__("mov %0, sp" : "=r"(*stack_address));
	#elif defined(__riscv) // RISC-V
	    __asm__("mv %0, sp" : "=r"(*stack_address));
	#else
	    fprintf(stderr, "Error: Unsupported architecture for stack address\n");
	    *stack_address = 0;
	    return false;
	#endif
    return true;
}

bool	get_stack_size() 
{
	struct rlimit limit; // struct for getting info about stack size	
	if (getrlimit (RLIMIT_STACK, &limit) == -1) { // get stack size
		perror ("getrlimit");
		return false;
  	}
	stack_size = limit.rlim_cur;
	if (stack_size == RLIM_INFINITY) { // if stack size is unlimited set to /150 of free ram
		fprintf(stderr, "Warning: Stack size is unlimited setting to /150 of avail ram to prevent overflow.\n"); // should not happen in most cases
		long ram_size = get_free_ram();
		if (ram_size == -1) {
			fprintf(stderr, "Error: Cannot determine free RAM size.\n");
			return false;
		}
		stack_size = ram_size/150; //getting 1/150 of free ram for stack size
	}
	if (stack_size < 2*1024) { // if stack size is less than 2KB
		fprintf(stderr, "Error: Its Dangerous to run the program, Stack size is too small\n");
		return false;
	}
	return true;
}