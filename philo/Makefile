# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/01 11:45:58 by fmaurer           #+#    #+#              #
#    Updated: 2025/01/24 12:13:43 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= philo
SRC_IN	= ./philo.c \
					./utils.c \
					./philo_routine.c \
					./init_philo.c
SRC			= $(patsubst ./%.c,%.c,$(SRC_IN))
OBJDIR	= obj
OBJS		= $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
CFLAGS	= -Wall -Werror -Wextra -pthread
HDR			= philo.h

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(HDR) | $(OBJDIR)
	clang $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	clang $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
