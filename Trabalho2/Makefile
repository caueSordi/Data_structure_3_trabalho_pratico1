CC = gcc
CFLAGS = -Iinclude
SRC = main.c $(wildcard src/*.c)
INC = $(wildcard include/*.h)
TARGET = programaTrab

all: $(TARGET)

$(TARGET): $(SRC) $(INC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

test: clean all run

zip:
	zip runcodes.zip Makefile $(SRC) $(INC)