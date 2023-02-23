NAME		= philo
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -pthread
RM 			= rm -rf

INCLUDES	= includes

FILES		= main cycle utils
SRCS 		= $(addsuffix .c, $(addprefix src/, $(FILES)))

$(NAME):
	@echo "\033[0;32m🥢 Setting the table...\033[0m"
	@gcc $(CFLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

all: $(NAME)

clean:
	@echo "\033[0;31m🔪 Killing off the remaining survivors...\033[0m"
	@$(RM) $(NAME)

fclean: clean

re: fclean all