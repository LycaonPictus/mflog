SRC = mflog.c
OBJ = $(SRC:.c=.o)

all: $(OBJ)

%.o: %.c
	@gcc -Wall -Werror -Wextra -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean

re: clean all

.PHONY: all clean fclean re