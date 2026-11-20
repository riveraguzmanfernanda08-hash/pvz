CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = main.c visual.c logica.c config.c lectura.c
OBJ = $(SRC:.c=.o)
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = pvz

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
