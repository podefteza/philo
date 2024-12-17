# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/12 11:09:54 by carlos-j          #+#    #+#              #
#    Updated: 2024/12/17 12:32:40 by carlos-j         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc -Wall -Wextra -Werror -pthread

SRCS = main.c utils.c init.c

OBJS = ${SRCS:.c=.o}

all = ${NAME}

${NAME}: ${OBJS}
	$(CC) $(OBJS) -o $(NAME)
	@echo "Compilation successful."

clean:
	@rm -f ${OBJS}
	@echo "Object files removed."

fclean: clean
	@rm -f ${NAME}
	@echo "Executable removed."

re: fclean all
	@echo "Recompiled all files."

.PHONY: all clean fclean re
