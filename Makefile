CC ?= gcc
CFLAGS ?= -Wall -Wextra -pthread -Iinclude
LDFLAGS ?= -pthread

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = queue_example

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
