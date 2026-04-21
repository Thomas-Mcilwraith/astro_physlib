# ---- GLOBAL VARS [NEVER CHANGE] --------------------------------------------
CC = gcc

INCLUDES = -I. \
		   -I../../../.

CFLAGS = -Wall -Wextra -Werror -std=c11 $(INCLUDES)
LDFLAGS = -lm

BUILD_DIR = ../../../../../build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
# ----------------------------------------------------------------------------


# --- PROGRAM CONFIG [MUST UPDATE THESE] -------------------------------------
SRC = two_body_problem.c \
      ../../../utilities/logging/log/log.c \
      ../../../mathematics-library/linear-algebra/matrix-operations/matrix_operations.c \
	  ../../../mathematics-library/numerical-methods/runge-kutta-4/runge_kutta_4.c \
	  ../../../file-io/internal-products/parameter-evolution-file/parameter_evolution_file.c

TARGET = $(BIN_DIR)/two_body_problem
# ----------------------------------------------------------------------------


# --- BUILD [NEVER CHANGE] --------------------------------------------------
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
# ----------------------------------------------------------------------------
