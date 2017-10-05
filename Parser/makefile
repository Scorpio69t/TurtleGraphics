CFLAGS = -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -Wformat -Wunused-macros -Wconversion -Wparentheses -Wjump-misses-init -Wlogical-op 

INCS = parser.h
TARGET = parser
SOURCES =  parser_init_func.c $(TARGET).c
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
