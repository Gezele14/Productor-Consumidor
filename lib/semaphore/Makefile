#Share memory Library 
LIBRARY = semaphore
SRC_LIBRARY = $(LIBRARY).c 
OBJ_LIBRARY= $(LIBRARY).o
LIB_LIBRARY = lib$(LIBRARY).a

#Compiler params
CC = gcc
CFLAGS = -c -Wall

#All
ALL: $(LIB_LIBRARY)

$(LIB_LIBRARY): $(OBJ_LIBRARY)
		ar rv $(LIB_LIBRARY) $(OBJ_LIBRARY)
		ranlib $(LIB_LIBRARY)

$(OBJ_LIBRARY): $(SRC_LIBRARY)
		$(CC) $(CFLAGS) $(SRC_LIBRARY)