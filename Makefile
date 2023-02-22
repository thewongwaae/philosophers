NAME		= philo
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -pthread -fsanitize=address -I includes/
RM 			= rm -rf

FILES		= main cycle utils
SRCS 		= $(addsuffix .c, $(addprefix src/, $(FILES)))

$(NAME):
	@echo "\033[0;32mSetting the table...\033[0m"
	@gcc $(CFLAGS) $(SRCS) -o $(NAME)

all: $(NAME)

clean:
	@echo "\033[0;31mKilling off the remaining survivors...\033[0m"
	@$(RM) $(NAME)

fclean: clean

re: fclean all