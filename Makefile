##
# Makefile for mkcmain 

CC= gcc
STD= -std=c99
OPT= -O3
DEBUG= -ggdb3
WARN= -Wall -Werror -Wextra -pedantic
CFLAGS= $(STD) $(OPT) $(WARN)

SRCS= mkcmain.c
OBJS= $(SRCS:.c=.o)
TARGET= mkcmain
INSTALL_DIR= /usr/local/bin

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o: $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all debug clean install

all: $(TARGET)


debug: OPT= $(DEBUG)
debug: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)

install: $(TARGET)
	strip --strip-unneeded $(TARGET)
	cp $(TARGET) $(INSTALL_DIR)/$(TARGET)
