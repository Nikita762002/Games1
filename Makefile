CC = gcc
TARGET = snake

all: $(TARGET)

$(TARGET): snake.c
	$(CC) -o $(TARGET) snake.c -lncurses

clean: 
	rm -f $(TARGET)
