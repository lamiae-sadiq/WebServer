# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 15:26:00 by lsadiq            #+#    #+#              #
#    Updated: 2024/03/13 14:01:43 by lsadiq           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	Webserv

MAIN =  main.cpp 


MULT = Multiplixer.cpp

M_MULT = $(addprefix srcs/Multiplexer/,$(MULT))

SRCS =  Parse.cpp Server.cpp Utils.cpp Location.cpp  Request.cpp

P_SRCS	= 	$(addprefix srcs/Parser/,$(SRCS))

SRC = getMethod.cpp methodHelp.cpp postMethod.cpp deleteMethod.cpp response.cpp cgi.cpp

M_SRCS	=	$(addprefix srcs/methods/,$(SRC))

OBJS = $(MAIN:.cpp=.o) $(M_SRCS:.cpp=.o) $(P_SRCS:.cpp=.o) $(M_MULT:.cpp=.o)

CXX = c++ -fsanitize=address

RM = rm -f

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

NONE	=	'\033[0m'
PURPLE	=	'\033[35m'
RED		=	'\033[1;91m'
GRAY	=	'\033[2;37m'
ITALIC	=	'\033[3m'
			
all : $(NAME)

$(NAME) : $(OBJS)
			@ $(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
			@echo $(GRAY) "Compiled $(NAME)..." $(NONE)

clean :
		@$(RM) $(OBJS)
		@echo $(RED) "Removing object files..." $(NONE)

fclean : clean
			@echo $(RED) "Removing $(NAME)..." $(NONE)
			@$(RM) $(NAME)

re : fclean $(NAME)