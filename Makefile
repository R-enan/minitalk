# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rleite-s < rleite-s@student.42sp.org.br    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/30 21:59:30 by rleite-s          #+#    #+#              #
#    Updated: 2022/12/13 19:08:24 by rleite-s         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	client server

FILES		=	client.c server.c

MAN_PATH	=	mandatory/
MANDATORY	=	$(addprefix $(MAN_PATH), $(FILES:.c=.o))
CHECK_MAN	=	$(wildcard $(MANDATORY))

BON_PATH	=	bonus/
BONUS		=	$(addprefix $(BON_PATH), $(FILES:.c=.o))
CHECK_BON	=	$(wildcard $(BONUS))

CFLAGS		=	-Wall -Werror -Wextra

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

ifeq ($(CHECK_BON),)
all: $(NAME)
else
all:
endif

$(NAME): $(MANDATORY)
	cc $(CFLAGS) $(MAN_PATH)$(@:=.c) -o $@

clean:
	rm -f $(MANDATORY) $(BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make

ifneq ($(CHECK_MAN),)
bonus: fclean $(BONUS)
	make $(NAME) MANDATORY="$(BONUS)" --no-print-directory
else
bonus: $(BONUS)
	make $(NAME) MANDATORY="$(BONUS)" --no-print-directory
endif

.PHONY: all clean fclean re bonus