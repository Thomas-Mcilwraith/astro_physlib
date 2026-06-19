# ---- GLOBAL VARS [NEVER CHANGE] --------------------------------------------
CC = gcc

INCLUDES = -I. \
           -I../../../.

CFLAGS = -Wall -Wextra -Werror -std=c11 $(INCLUDES)

BUILD_DIR = ../../../../../build
OBJ_DIR = $(BUILD_DIR)/obj
# ----------------------------------------------------------------------------


# --- MODULE CONFIG [MUST UPDATE THESE] --------------------------------------
SRC = log.c
# ----------------------------------------------------------------------------


# --- BUILD [NEVER CHANGE] ---------------------------------------------------
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(OBJ)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
# ----------------------------------------------------------------------------
