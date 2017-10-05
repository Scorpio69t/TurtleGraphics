CFLAGS = `pkg-config sdl2 --cflags` -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm -Wformat -Wunused-macros -Wparentheses -Wjump-misses-init -Wlogical-op 
INCS = extens.h 
TARGET = extens
SOURCES = tokenize.c queue.c errors.c initialiseFunc.c stack.c SDL_func.c syntax.c translate.c $(TARGET).c
LIBS =  `pkg-config sdl2 --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
