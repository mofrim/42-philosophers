# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/01 11:45:58 by fmaurer           #+#    #+#              #
#    Updated: 2025/02/07 16:07:50 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= philo_bonus
SRC_IN	= ./philo_bonus.c \
					./utils_bonus.c \
					./philo_dinner.c \
					./philo_eat.c \
					./any_dead_all_fed.c \
					./make_proto_philo.c \
					./semas.c \
					./death_thread.c \
					./set_state_locked.c
SRC			= $(patsubst ./%.c,%.c,$(SRC_IN))
OBJDIR	= obj
OBJS		= $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
HDR			= philo_bonus.h

CFLAGS	= -Wall -Werror -Wextra
TSAN_CFLAGS	= -g -Wall -Werror -Wextra -fsanitize=thread

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(HDR) | $(OBJDIR)
	clang $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	clang $(CFLAGS) -o $@ $^

philo-tsan: $(SRC)
	$(CC) $(TSAN_CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
