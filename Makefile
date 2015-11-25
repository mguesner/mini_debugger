NAME = mini_debugger

OBJ = main.o \
	  mini_debugger.o

LIB = line_edition/line_edition.a

CC = clang -g -O3 -Wall -Wextra -Werror -I line_edition

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -ltermcap -o $(NAME) $(OBJ) $(LIB)

%.o: %.c
	$(CC) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all