OS = linux
CC = gcc
CXX = g++
compiler=$(CXX)
EXEC_OBJECTS = main.o
SRC_DIR = src
BUILD_DIR = build
RELEASE_DIR = release/linux

FFMPEG_LIB = -static -Llib -lavcodec  -lavformat -lavcodec -lavutil -lavdevice -lavfilter -lswresample  -Wl,-Bdynamic  -lm -lz -lpthread  -lbz2

OPENGL_LIB =   -Wl,-Bdynamic -lglut -lGL -DGL_GLEXT_PROTOTYPES -lpthread 


INCLUDE = -Iinclude -Isrc

DEPENDENCE = $(OPENGL_LIB) $(FFMPEG_LIB) $(INCLUDE)

SRCS = main.cpp \
		decoder.cpp \
		mediaplayer.cpp \
		opengl.cpp

SRCS_NOEXT = $(addprefix $(BUILD_DIR)/, $(notdir $(basename $(SRCS))))
OBJS = $(addsuffix .o, $(SRCS_NOEXT))


BIN = $(RELEASE_DIR)/main


build:$(BIN)
	@echo "$(SRCS_NOEXT)"

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	@echo "build .o $@"
	$(compiler) -c $< -o $@ $(DEPENDENCE)

$(BIN):$(OBJS)
	@echo "build exe $@"
	$(compiler) -o $@ $(OBJS) $(DEPENDENCE)
