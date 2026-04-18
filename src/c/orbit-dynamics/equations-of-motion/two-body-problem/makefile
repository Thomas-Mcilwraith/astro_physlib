# Compiler
CC = gcc

# Compiler Inlclude flags
INCLUDES = -I. \
		   -I../../../.

# Flags
CFLAGS = -Wall -Wextra -std=c11 $(INCLUDES)
LDFLAGS = -lm

# Source files
SRC = two_body_problem.c \
      ../../../utilities/logging/log/log.c \
      ../../../mathematics-library/linear-algebra/matrix-operations/matrix_operations.c

# Object files
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = two_body_problem

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)
