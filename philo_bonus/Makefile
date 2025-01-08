# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/01 11:45:58 by fmaurer           #+#    #+#              #
#    Updated: 2025/01/08 14:02:26 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= philo_bonus
SRC_IN	= ./philo_bonus.c \
					./utils_bonus.c
SRC			= $(patsubst ./%.c,%.c,$(SRC_IN))
OBJDIR	= obj
OBJS		= $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
# CFLAGS	= -Wall -Werror -Wextra -pthread
CFLAGS	= -g
HDR			= philo_bonus.h

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
