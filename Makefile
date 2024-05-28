NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread

MAIN = philo
#UTILS =

SRC_DIR = src
SRC = $(SRC_DIR)/philo.c
#		$(addsuffix .c, $(addprefix srcs/, $(MAIN)))
#		$(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -ldl -o $(NAME)

debug: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -ldl -g3 -fsanitize=address -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -I/usr/include -O3 -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug
