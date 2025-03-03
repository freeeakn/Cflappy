CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = $(shell sdl2-config --libs)
CPPFLAGS = $(shell sdl2-config --cflags)

TARGET = flappy
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean 