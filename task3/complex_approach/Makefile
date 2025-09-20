NAME = exec

CC = gcc

# -Wextra -Werror  -fsanitize=address

CFLAGS = -Wall 

SRCS = $(shell find ./src -name "*.c")

DEPS = header.h Makefile

OBJS = $(SRCS:.c=.o)

%.o : %.c
	$(CC) -I. $(CFLAGS) -c $< -o $@  

all : $(NAME)

$(NAME) : $(OBJS) $(DEPS)
	$(CC) $(OBJS) $(CFLAGS) -I. -o $(NAME)

clean : 
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all re flean all