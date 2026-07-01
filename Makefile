NAME := codexion
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Find all the source files
SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Compile flags
CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $^ -o $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(OBJS)
	$(CC) -g $^ -o $(NAME)

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJ)

re: fclean all

.PHONY: all clean fclean re
