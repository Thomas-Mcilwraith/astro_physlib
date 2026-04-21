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
SRC = parameter_evolution_file.c \
      ../../../utilities/logging/log/log.c

TARGET = $(BIN_DIR)/parameter_evolution_file
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
