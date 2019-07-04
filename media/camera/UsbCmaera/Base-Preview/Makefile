OS = linux
CC = gcc
CXX = g++
compiler=$(CC)
EXEC_OBJECTS = main.o
SRC_DIR = src
BUILD_DIR = build
RELEASE_DIR = release/linux
SRCS_NOEXT = $(addprefix $(BUILD_DIR)/, $(notdir $(basename $(SRCS))))
OBJS = $(addsuffix .o, $(SRCS_NOEXT))

SRCS = main.c

BIN = $(RELEASE_DIR)/main


build:$(BIN)
	@echo "$(SRCS_NOEXT)"

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@echo "build $@"
	$(compiler) -c $< -o $@ 

$(BIN):$(OBJS)
	@echo "build $@"
	$(compiler) -o $@ $(OBJS) 
