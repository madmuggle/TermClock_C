TARGET = clock
CC = gcc
#CFLAGS = -I./inc -c -Wall -DDEBUG
CFLAGS = -I./inc -c -Wall
LDFLAGS =

SRC = $(wildcard src/*.c)
#OBJ = $(patsubst %.c, %.o, $(SRC))
OBJ = $(SRC:.c=.o)
#$(warning $(SRC))
#$(warning $(OBJ))

.PHONY: all install uninstall clean test
all: depend $(TARGET)

depend:
	@echo -e "  GEN\t.depend"
	@$(CC) -MM $(CFLAGS) $(SRC) | \
	sed -e 's,\(.*:\),src/\1,' > .depend

-include .depend

install:
	cp $(TARGET) /usr/local/bin/
	cp font/number.txt /usr/local/share/number.txt
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	rm -f /usr/local/share/number.txt

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	@echo -e "  LINK\t$@"
	@$(CC) -o $@ $^ $(LDFLAGS)
%.o: %.c
	@echo -e "  CC\t$<"
	@$(CC) -o $@ $< $(CFLAGS)

lss: $(TARGET)
	@echo "disassamble the elf file..."
	@objdump -S -D $< > $(TARGET).lss
clean:
	@rm -rf $(TARGET) $(OBJ) *.lss .depend

