#Util Library
UTIL = util
SRC_UTIL = $(UTIL).c 
OBJ_UTIL= $(UTIL).o
LIB_UTIL = lib$(UTIL).a

#Compiler params
CC = gcc
CFLAGS = -c -Wall

#All
ALL: $(LIB_UTIL)

#LibUtil make
$(LIB_UTIL): $(OBJ_UTIL)
		ar rv $(LIB_UTIL) $(OBJ_UTIL)
		ranlib $(LIB_UTIL)

$(OBJ_UTIL): $(SRC_UTIL)
		$(CC) $(CFLAGS) $(SRC_UTIL)