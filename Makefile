# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 10:35:39 by cyferrei          #+#    #+#              #
#    Updated: 2025/02/13 10:54:16 by cyferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
RM = rm -rf
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

SRC_DIR = srcs
INCLUDE_DIR = includes

SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Server.cpp $(SRC_DIR)/commandHandler.cpp \
      $(SRC_DIR)/Client.cpp $(SRC_DIR)/Channel.cpp $(SRC_DIR)/debug.cpp $(SRC_DIR)/cmd_mode.cpp \
	  $(SRC_DIR)/cmd_nick.cpp $(SRC_DIR)/cmd_pass.cpp $(SRC_DIR)/cmd_privmsg.cpp \
	  $(SRC_DIR)/cmd_topic.cpp $(SRC_DIR)/cmd_user.cpp 

INCLUDES = $(INCLUDE_DIR)/Server.hpp $(INCLUDE_DIR)/Client.hpp $(INCLUDE_DIR)/Channel.hpp

OBJ = $(SRC:.cpp=.o)

BOLD    = \e[1m
GREEN   = \e[38;5;76m
RED     = \e[38;5;196m
RESET   = \e[00m

all: $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(BOLD)Linking...$(RESET)"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Executable '$(NAME)' created successfully!$(RESET)"

clean:
	@echo "$(BOLD)Cleaning object files...$(RESET)"
	$(RM) $(OBJ)
	@echo "$(GREEN)Object files cleaned successfully!$(RESET)"

fclean: clean
	@echo "$(BOLD)Cleaning executable...$(RESET)"
	$(RM) -f $(NAME)
	@echo "$(GREEN)Executable cleaned successfully!$(RESET)"

re: fclean all

.PHONY: all clean fclean re