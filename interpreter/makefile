CFLAGS = `pkg-config sdl2 --cflags` -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm -Wformat -Wunused-macros -Wconversion -Wparentheses -Wjump-misses-init -Wlogical-op 
INCS = stack.h interp.h
TARGET = interp
SOURCES = stack.c interp_init_func.c SDL_func.c $(TARGET).c
LIBS =  `pkg-config sdl2 --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
