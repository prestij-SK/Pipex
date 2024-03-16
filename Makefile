SRC =	pipex.c \
		cmd_execute.c \
		libft_funcs.c \

OBJS = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = pipex
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)

$(OBJS): pipex.h Makefile

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re sanitize bonus