all: test-suffix-array

test-suffix-array: suffix-array.o test-suffix-array.o
	gcc -o $@ $^

%.o: %.c
	gcc -c $^

clean:
	rm -rf *.o test-suffix-array
