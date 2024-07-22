CC=gcc
AS=as
LD=ld
OBJDUMP=objdump
OBJCOPY=objcopy
SIZE=size
LINK=$(CC)

BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
# BIN_DIR=bin
BIN_DIR=.

LIBS :=
DEFS := -DVERSION=\"0.1\"
WARN_LEVEL = -Wall -Wextra -pedantic

PRG = brightness
INCLUDES := -Iinc
CFLAGS := $(INCLUDES) $(DEFS) $(WARN_LEVEL) -pipe -O0 -g3 -std=c11

debug: CFLAGS += -O0 -g3
debug: all

release: CFLAGS += -O2
release: all
LDFLAGS = $(LIBS) -ffunction-sections -Wl,--gc-sections

SRC_C := $(wildcard *.c) $(wildcard src/*.c)
SRC_A := $(wildcard src/*.s)

OBJECTS := $(SRC_C:%.c=$(OBJ_DIR)/%.o)
OBJECTS += $(SRC_A:%.s=$(OBJ_DIR)/%.o)

all: directories $(PRG)

$(PRG): $(BIN_DIR)/$(PRG)

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(@D)
	$(AS) $(INCLUDES) $(MMCU) -g -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN_DIR)/$(PRG): $(OBJECTS)
	@mkdir -p $(@D)
	$(LINK) -o $(BIN_DIR)/$(PRG) $^ $(LDFLAGS) $(LIBS)

.PHONY: directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(BIN_DIR)/*

.PHONY: mrproper
mrproper:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
