NAME = print_dir

CFLAGS = -Wall -Wextra -Werror --pedantic -g3 -fsanitize=address

SRCS = $(shell find src -type f -name "*.c")

HEADER = header.h

DEPS = Makefile $(HEADER)

OBJ = $(SRCS:.c=.o)

%.o : %.c
	gcc -I . $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) $(DEPS) 
	gcc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re