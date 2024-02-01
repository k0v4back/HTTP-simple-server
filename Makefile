TARGET = http_server
CC = g++
GENERAT = $(shell find . -type f -name '*.o') $(shell find . -type f -name '*core.*') $(TARGET)

SRC = $(shell find . -type f -name '*.cpp')
OBJ = $(patsubst %.cpp, %.o, $(SRC))

CFLAGS = -Wall

all: $(TARGET)	

$(TARGET): $(OBJ)
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) -std=c++11 -c $< -o $@ $(CFLAGS)

clean:
		rm -f $(GENERAT)