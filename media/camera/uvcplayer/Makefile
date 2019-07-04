OS = linux
CC = gcc
CXX = g++
compiler=$(CXX)
EXEC_OBJECTS = main.o
SRC_DIR = src
BUILD_DIR = build
RELEASE_DIR = release/linux
SRCS_NOEXT = $(addprefix $(BUILD_DIR)/, $(notdir $(basename $(SRCS))))
OBJS = $(addsuffix .o, $(SRCS_NOEXT))


INCLUDE_DIR = src

SRCS = main.cpp \
	Camera.cpp \
	OpenGL.cpp

#BIN = $(RELEASE_DIR)/main

BIN=main

build:$(BIN)
	@echo "$(SRCS_NOEXT)"

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	@echo "build $@"
	$(compiler) -c $< -o $@ -I$(INCLUDE_DIR) -lpthread -lglut -lGL -DGL_GLEXT_PROTOTYPES
$(BIN):$(OBJS)
	@echo "build $@"
	$(compiler) -o $@ $(OBJS) -lpthread -lglut -lGL -DGL_GLEXT_PROTOTYPES
