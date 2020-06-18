APP = inicializador
NAME_UTIL = util
BIN = ../bin
INCLUDE = ../include
LIB = ../lib
SRC = $(APP).c 
LIB_UT = lib$(NAME_UTIL).a

CC = gcc
CFLAGS = -c -Wall

APP: APP_E

#Aplicacion con biblioteca estatica
APP_E: $(SRC)
		$(CC) -o $(BIN)/$(APP) $(SRC) $(LIB)/$(LIB_UT) -I$(INCLUDE)