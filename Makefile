# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 15:26:00 by lsadiq            #+#    #+#              #
#    Updated: 2024/01/25 15:26:02 by lsadiq           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME	=	Webserv

MAIN	=	./src/main.cpp

SRCS	=	config.cpp

SRC		=	test.cpp

P_SRCS	= 	$(addprefix src/parsing/,$(SRCS))

M_SRCS	=	$(addprefix src/methods/,$(SRC))


OBJS	=	$(P_SRCS:.c=.o) $(M_SRCS:.c=.o) $(MAIN:.c=.o)


CXXFLAGS	=	-Wall -Wextra -Werror  #-fsanitize=address -g3 


CXX		=	c++

AR 		=	ar rc


NONE	=	'\033[0m'
GREEN	=	'\033[32m'
RED		=	'\033[1;91m'
GRAY	=	'\033[2;37m'
ITALIC	=	'\033[3m'

.c.o	:
			@echo $(ITALIC)$(GRAY) "    Compiling $<" $(NONE)
			@$(CXX) $(CXXFLAGS) -c $< -o $(<:.c=.o)

all		:	$(NAME)

$(NAME)	:  	$(OBJS)
			@echo $(GREEN) "Compiling $(NAME)..." $(NONE)
			@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
			@echo $(GREEN) "\n ~ Compiled !!" $(NONE)


clean	:
			@echo $(RED) "Removing object files..." $(NONE)
			@rm -rf $(OBJS)

fclean	:	clean
			@echo $(RED) "Removing $(NAME)..." $(NONE)
			@rm -f $(NAME)

re		:	fclean $(NAME)

