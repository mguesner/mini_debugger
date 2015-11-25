NAME = line_edition.a

OBJ = putonterm.o \
	  line_edition_init.o \
	  line_edition_end.o \
	  line_edition_getline.o

CC = clang -g -O3 -Wall -Wextra -Werror -I include

all: $(NAME)

$(NAME): $(OBJ)
	ar rs $(NAME) $(OBJ)
	ranlib $(NAME)

%.o: %.c
	$(CC) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all