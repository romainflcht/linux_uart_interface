# * __ FILE NAME _______________________________________________________________
TARGET    = main
EXEC_NAME = uart

# * __ DIRECTORIES OPTIONS _____________________________________________________
SRCS_DIR  = srcs
INCS_DIR  = includes
OBJS_DIR  = objs
BIN_DIR   = bin

# * __ COMPILER OPTIONS ________________________________________________________
CC          = gcc
FLAGS       = -g -I$(INCS_DIR) -Wall -Wextra -Werror
LINK        = 

# * __ COMPILATION _____________________________________________________________
INCS = $(wildcard $(INCS_DIR)/*.h)
SRCS = main.c console.c uart.c utils.c
OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))

# * __ RUN _____________________________________________________________________
ARGS = /dev/ttyACM0 11520

# * __ FONT ____________________________________________________________________
MAGENTA  = \e[35m
CYAN     = \e[36m
WHITE    = \e[37m
YELLOW   = \e[33m
RED      = \e[31m
GREEN    = \e[32m

BOLD     = \e[1m
RST      = \e[0m
CLEAR    = \e[2J
CUR_HOME = \e[H


all: $(BIN_DIR)/$(EXEC_NAME)


$(BIN_DIR)/$(EXEC_NAME): $(OBJS) | mkdir_bin
	@echo "\n$(RED)--SOURCES FILE FOUND : $(RST)$(BOLD)$(SRCS)$(RST)"
	@echo "$(YELLOW)--OBJECTS FILE FOUND : $(RST)$(BOLD)$(OBJS)$(RST)"
	@echo "\n$(CYAN)~LINKING $(RST)$(BOLD)$<$(RST)$(CYAN) TO EXECUTABLE TARGET $(RST)$(BOLD)$@$(RST)"
	@$(CC) $^ -o $@ $(FLAGS) $(LINK)
	@echo "$(GREEN)-> FINISHED!$(RST)"


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES) | mkdir_obj
	@echo "$(MAGENTA)~COMPILING $(RST)$(BOLD)$<$(RST)$(MAGENTA) TO $(RST)$(BOLD)$@$(RST)"
	@$(CC) -c $< -o $@ $(FLAGS)


$(OBJS_DIR)/$(TARGET).o: $(TARGET).c $(INCLUDES) | mkdir_obj
	@echo "$(MAGENTA)~COMPILING $(WHITE)$(BOLD)$<$(RST)$(MAGENTA) TO $(RST)$(BOLD)$@$(RST)"
	@$(CC) -c $< -o $@ $(FLAGS)


run: all
	@echo "$(CLEAR)$(CUR_HOME)\r"
	@./$(BIN_DIR)/$(EXEC_NAME) $(ARGS)


mkdir_obj: 
	@mkdir -p $(OBJS_DIR)

mkdir_bin:
	@mkdir -p $(BIN_DIR)


clean:
	@echo "$(CLEAR)$(CUR_HOME)$(RED)$(BOLD)~CLEANING DIRECTORY... $(RST)"
	@rm -rf $(OBJS_DIR)
	@rm -rf $(BIN_DIR)/$(EXEC_NAME)
	@echo "$(GREEN)-> FINISHED!$(RST)"


.PHONY: all clean run mkdir_bin