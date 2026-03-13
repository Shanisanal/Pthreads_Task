# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(INC_DIR) -D_REENTRANT
DEBUGFLAGS = -Wall -Wextra -g
LDFLAGS = -lpthread -lrt -lm

# Project structure
SRC_DIR = Source
INC_DIR = Include
RELEASE_DIR = release

# Source files
SRCS = main.c Source/PossixHandler.c Source/ThreadHandler.c

# Flatten object names
OBJS_RELEASE = $(addprefix $(RELEASE_DIR)/, $(notdir $(SRCS:.c=.o)))

vpath %.c $(SRC_DIR) 

# Executable name
TARGET = TaskThreads

# Default rule
all: release

# Build release version
release: $(RELEASE_DIR)/$(TARGET)

$(RELEASE_DIR)/$(TARGET): $(OBJS_RELEASE)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(RELEASE_DIR)/%.o: %.c
	@mkdir -p $(RELEASE_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(RELEASE_DIR) 

.PHONY: all release debug clean
