# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/12 11:09:54 by carlos-j          #+#    #+#              #
#    Updated: 2025/01/15 09:08:37 by carlos-j         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c utils.c init.c
OBJS = ${SRCS:.c=.o}

NAME = philo

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	@echo "Compilation successful."

debug: ${SRCS}
	${CC} ${CFLAGS} -DDEBUG ${SRCS} -o ${NAME}
	@echo "Compilation with DEBUG flag successful."

tsanitize: fclean
	${CC} ${CFLAGS} -fsanitize=thread ${SRCS} -o ${NAME}
	@echo "Compilation with ThreadSanitizer successful."

clean:
	@rm -f ${OBJS}
	@echo "Object files removed."

fclean: clean
	@rm -f ${NAME}
	@echo "Executable removed."

re: fclean all
	@echo "Recompiled all files."

.PHONY: all clean fclean re tsanitize debug
