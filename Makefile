# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 15:26:00 by lsadiq            #+#    #+#              #
#    Updated: 2024/02/03 17:54:26 by lsadiq           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	Webserv

MAIN =  main.cpp 

P_SRCS	= 	$(addprefix src/parsing/,$(SRCS))

SRC = GET.cpp

M_SRCS	=	$(addprefix src/methods/,$(SRC))

OBJS = $(MAIN:.cpp=.o) $(M_SRCS:.cpp=.o)

CXX = c++

RM = rm -f

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

NONE	=	'\033[0m'
PURPLE	=	'\033[35m'
RED		=	'\033[1;91m'
GRAY	=	'\033[2;37m'
ITALIC	=	'\033[3m'
			
all : $(NAME)

$(NAME) : $(OBJS)
			@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
			@echo $(GRAY) "Compiled $(NAME)..." $(NONE)

clean :
		@$(RM) $(OBJS)
		@echo $(RED) "Removing object files..." $(NONE)

fclean : clean
			@echo $(RED) "Removing $(NAME)..." $(NONE)
			@$(RM) $(NAME)

re : fclean $(NAME)