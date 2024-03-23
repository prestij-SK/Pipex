SRC =	pipex.c \
		cmd_execute.c \
		libft_funcs.c \
		utils.c \

BONUS_SRC = pipex_bonus.c \
			cmd_execute.c \
			libft_funcs.c \
			utils.c \
			get_next_line.c \
			get_next_line_utils.c \

OBJS = $(SRC:.c=.o)
BONUS_OBJS = $(BONUS_SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = pipex
BONUS_NAME = pipex_bonus
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)

$(OBJS): pipex.h Makefile
$(BONUS_OBJS): pipex.h Makefile

bonus: $(BONUS_OBJS) $(BONUS_NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME)

re: fclean all

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME)

.PHONY: all clean fclean re bonus