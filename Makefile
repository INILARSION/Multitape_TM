cfiles := $(wildcard *.c)

compile: $(cfiles) $(hfiles)
	gcc -O2 -o multiband_tm $(cfiles) -lm