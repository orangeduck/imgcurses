CC=gcc
OUT=imgcurses
INCS=-I./include

C_FILES= $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

CFLAGS= $(INCS) -std=gnu99 -Wall -Werror -Wno-unused -Wno-unused-result -O3 -g
LFLAGS= -lncurses -lm -lpng -ljpeg

$(OUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LFLAGS) -o $@
	
obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@
	
obj:
	mkdir obj
	
clean:
	rm $(OUT) $(OBJ_FILES)
