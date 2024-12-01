# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/01 11:45:58 by fmaurer           #+#    #+#              #
#    Updated: 2024/12/01 11:57:14 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= philo
SRC_IN	= ./philo.c
SRC			= $(patsubst ./%.c,%.c,$(SRC_IN))
OBJDIR	= obj
OBJS		= $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
CFLAGS	= -pthread
HDR			=

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRC) $(HDR) | $(OBJDIR)
	clang -c -o $@ $^

$(NAME): $(OBJS)
	clang $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
