# C := gcc  
#     GXX := g++  
#     LD  := ld  
#     AR  := ar  
      
#     LDFLAGS := -shared -fpic  
#     ARFLAGS := -rc  
#     #CFLAGS  := -g -Wall -fpic  
#     CFLAGS  := -fpic

#     DIR_INC := ./inc  
#     DIR_SRC := ./src
#     DIR_OBJ := ./obj  
#     DIR_LIB := ./lib
      
#     LIB_NAME := main
#   #  $(warning "$(DIR_SRC)")
#     SRC := $(wildcard ${DIR_SRC}/*.c)  
#     OBJ := $(patsubst %.c, ${DIR_OBJ}/%.o, $(notdir ${SRC}))  
      
#     SHARE_LIB   := $(LIB_NAME).so  
#     DYNAMIC_LIB := $(LIB_NAME).a  
      
#     INC := -I${DIR_INC}  
#            $(warning "$(OBJ)")

# all:$(SRC)  
# 	#$(LD) $(LDFLAGS) -o $(DIR_LIB)/$(SHARE_LIB) $(SRC)  
# 	#${AR} ${ARFLAGS} $(DIR_LIB)/$(DYNAMIC_LIB) $(SRC)  
# 	${DIR_OBJ}/%.o:${DIR_SRC}/%.c  
# 	@echo Compiling $(OBJ) ...  
# 	$(C) $(CFLAGS) -o $@ -c $< ${INC} -g  
# 	##	clean:  
# 	#	rm -rf $(DIR_OBJ)/*.o $(DIR_LIB)/*.so $(DIR_LIB)/*.a  




main:src/main.c
	gcc -o lib/main src/main.c -ldl

