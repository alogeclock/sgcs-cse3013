main : main.o print.o solve.o
	gcc -o main main.o print.o solve.o

main.o : main.c
	gcc -c -o main.o main.c

print.o : print.c
	gcc -c -o print.o print.c

solve.o : solve.c
	gcc -c -o solve.o solve.c

main.o calculate.o print.o : header.h

.PHONY: clean
clean:
	rm main main.o print.o solve.o