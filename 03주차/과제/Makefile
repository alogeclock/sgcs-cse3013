cc = g++
cflags = -W -g
target = main
objects = main.o str.o

$(target): $(objects) str.h
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.cpp str.h
	$(cc) $(cflags) -c -o $@ $<

.PHONY: clean
clean: 
	rm $(objects) $(target)