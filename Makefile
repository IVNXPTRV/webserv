# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvoronts <vvoronts@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/19 15:34:56 by vvoronts          #+#    #+#              #
#    Updated: 2025/12/19 15:36:07 by vvoronts         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CPP					=	c++
CPPFLAGS			=	-Wall -Wextra -Werror -std=c++98 -Wunreachable-code
DEPFLAGS			=	-MMD -MF
DEBUG				=	-g
# PEDANT				=	-Wpedantic
RM					=	rm -rf

# Name of the output library
NAME				=	webserv

# Source directories
VPATH				=	\
						./src/: \

# Include directories
INCLUDE_DIRS		=	\
						./include \

# Source files
SRC 				= \
						main.cpp \

# Object and Dependency files
OBJ					= $(SRC:%.cpp=obj/%.o)
DEP					= $(SRC:%.cpp=dep/%.d)

INCLUDE				=	$(addprefix -I, $(INCLUDE_DIRS))

# Make flags
MFLAGS				=	 --no-print-directory -C

# Build all targets
all: $(NAME)

# Link mandatory object files
$(NAME): $(OBJ)
	@echo "$(YELLOW)Building $(NAME)..."
	@$(CPP) $(OBJ) -o $@
	@echo "$(GREEN)$(NAME) has been built"

# Compile mandatory object files
obj/%.o: %.cpp | obj_dir dep_dir
	@$(CPP) $(CPPFLAGS) $(INCLUDE) $(DEBUG) $(DEPFLAGS) dep/$(@:obj/%.o=%.d) -c $< -o $@ && printf "$(GRAY)Compiling: $(RESET)$(notdir $< \n)"

# Create mandatory object directories
obj_dir:
	@mkdir -p obj

# Create mandatory dependency directories
dep_dir:
	@mkdir -p dep

# Clean build files
clean:
	@echo "$(YELLOW)Cleaning $(NAME)..."
	@$(RM) obj
	@$(RM) dep
	@echo "$(RED)$(NAME) has been cleaned$(RESET)"

# Clean build files and executables
fclean: clean
	@$(RM) $(NAME)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re
.DEFAULT_GOAL := all

# Include the dependency files
-include $(DEP)

# set colors
RED					= 	\033[0;31m
GREEN				= 	\033[0;32m
YELLOW				= 	\033[0;33m
GRAY				= 	\033[0;37m
RESET				= 	\033[0m