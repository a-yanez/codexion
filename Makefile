NAME := codexion

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread -I$(INC_DIR)
# Debug flags
DBGFLAGS := $(CFLAGS) -g3 -O0 \
			-fsanitize=address,undefined \
			-fno-omit-frame-pointer

# Automatically generate header dependencies
DEPFLAGS := -MMD -MP

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

debug: CFLAGS := $(DBGFLAGS)
debug: fclean $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Include generated dependency files if they exist
-include $(DEPS)

.PHONY: all clean fclean re debug
