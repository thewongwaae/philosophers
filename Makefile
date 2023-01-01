NAME = philo

CFLAGS = -Wall -Wextra -Werror -pthread

RM = rm -rf

SRCS = 	srcs/main.c\
		srcs/init.c\
		srcs/philo.c\
		srcs/philo_util.c\
		srcs/utils.c\

$(NAME) :
	gcc $(CFLAGS) $(SRCS) -o $(NAME)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all
