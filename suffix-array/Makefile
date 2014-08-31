all: suffix-array

suffix-array: suffix-array.o
	gcc -o $@ $^

%.o: %.c
	gcc -c $^

clean:
	rm -rf *.o suffix-array
