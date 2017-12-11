# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xxxxxxxx <marvin@student.42.fr>.           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/23 15:22:13 by cdittric          #+#    #+#              #
#    Updated: 2017/11/23 15:30:31 by cdittric         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fillit

SRC = fillit.c

OBJ = fillit.o

FLAGS = -Wall -Wextra -Werror

$(NAME): $(OBJ)
	gcc $(FLAGS) -o $(NAME) $(OBJ)

all: $(NAME)

%.o : %.c
	gcc $(FLAGS) -o $@ -c $<

clean:
	/bin/rm -f *.o

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
