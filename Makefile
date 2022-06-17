# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/20 12:28:43 by okinnune          #+#    #+#              #
#    Updated: 2022/06/17 10:47:59 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractol

SRCS = main.c threading.c sample_image.c evaluate_arg.c \
		fractals.c loops.c thread_helpers.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a
CC = gcc -Ilibft
CFLAGS = -pthread -Wall -Werror -Wextra -O2 #-mavx -fsanitize=address -g
UNAME= $(shell uname)
ifeq ($(UNAME), Darwin)
MLXFLAGS = libmlx.dylib
MLIB =  libmlx.dylib
else ifeq ($(UNAME), Linux)
override CFLAGS += -Imlx/Linux
MLXFLAGS =  -Imlx/Linux/ -I/usr/include -I/usr/lib/ mlx/Linux/libmlx.a -lm -lX11 -lXext
MLIB = mlx/Linux/libmlx.a
else
warning:
	@echo "Compilation for platform $(UNAME) not supported."
	exit 1
endif

all:
	@echo "Compiling for platform: $(UNAME)."
	$(MAKE) $(NAME)

$(NAME): $(OBJS) $(MLIB) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(MLIB) $(MLXFLAGS)

$(LIBFT):
	make -C libft

libmlx.dylib:
	make -C mlx/OS_X
	mv mlx/OS_X/libmlx.dylib libmlx.dylib

mlx/Linux/libmlx.a:
	make -C mlx/Linux

clean	:
	make -C libft clean
	#make -C mlx/OS_X clean
	#make -C mlx/Linux clean
	rm -f $(OBJS) *~ core *.core

fclean	: clean
	rm -f libmlx.dylib
	rm -rf *.dSYM
	rm -f $(NAME)

re:	fclean all
