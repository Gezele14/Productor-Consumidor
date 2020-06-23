APP = inicializador
BIN = ../bin
INCLUDE = ../include
LIB = ../lib
SRC = $(APP).c 
LIB_UT = util/libutil.a
LIB_SM = shareMem/libshareMem.a
LIB_SEM = semaphore/libsemaphore.a

CC = gcc
CFLAGS = -Wall -lm

LIBS = $(LIB)/$(LIB_UT) $(LIB)/$(LIB_SM) $(LIB)/$(LIB_SEM)


ALL: APP Prod
 
APP: 
		$(CC) -o $(BIN)/$(APP) $(SRC) $(LIBS) -I$(INCLUDE) $(CFLAGS)

Prod:
		$(CC) -o $(BIN)/productor productor.c $(LIBS) -I$(INCLUDE) $(CFLAGS)