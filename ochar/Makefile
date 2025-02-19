CC = gcc
CFLAGS = -Wall -pthread -I./include
SRC_DIR = sources
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXEC = philo

# Rule to build the final executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Create obj directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the object files and executable
clean:
	rm -rf $(OBJ_DIR) $(EXEC)

.PHONY: clean
