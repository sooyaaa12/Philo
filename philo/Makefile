NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = sources/main.c sources/dining_table.c sources/philo.c sources/philo_utils.c sources/philo_utils2.c sources/dining_table_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
