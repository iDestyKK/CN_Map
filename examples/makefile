CC = gcc
CFLAGS = --std=gnu89 -g
LIB = ../cn_map.c ../cn_cmp.c

all: int_example string_example comparison_func_example iteration_example interactive_example

int_example: int_example.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

string_example: string_example.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

comparison_func_example: comparison_func_example.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

iteration_example: iteration_example.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

interactive_example: interactive_example.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) int_example string_example comparison_func_example iteration_example interactive_example
