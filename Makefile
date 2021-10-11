cfiles := $(wildcard *.c)

compile: $(cfiles) $(hfiles)
	gcc -O2 -o multitape_tm $(cfiles) -lm