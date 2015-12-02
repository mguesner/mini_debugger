NAME = mini_debugger

OBJ = main.o \
	  mini_debugger.o \
	  init_debugger.o \
	  load_symbol.o \
	  explode.o\
	  wait_event.o \
	  get_current_sym.o \
	  delete_node.o \
	  db_run.o \
	  db_quit.o \
	  db_info.o \
	  db_cont.o \
	  db_backtrace.o \
	  db_breakpoint.o \
	  db_enable.o \
	  function_info.o \
	  breakpoint_info.o \
	  enable_count.o \
	  enable_breakpoint.o

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