# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/01 11:45:58 by fmaurer           #+#    #+#              #
#    Updated: 2025/02/07 00:29:54 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= philo
SRC_IN	= ./philo.c \
					./utils.c \
					./philo_routine.c \
					./init_philo.c \
					./more_utils.c \
					./set_state_locked.c
SRC			= $(patsubst ./%.c,%.c,$(SRC_IN))
OBJDIR	= obj
OBJS		= $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
CFLAGS	= -Wall -Werror -Wextra -pthread
TSAN_CFLAGS	= -g -Wall -Werror -Wextra -pthread -fsanitize=thread
HDR			= philo.h
# CC = cc
CC = clang

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(HDR) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

philo-tsan: $(SRC)
	$(CC) $(TSAN_CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
