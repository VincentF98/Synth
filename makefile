CC=gcc
CCFLAGS=-Wall -O0 -g 
CCSTATICFLAGS=${CCFLAGS} -rdynamic
CCDYNAMICFLAGS=${CCFLAGS} -fPIC 
LDFLAGS=-lm
LDSTATICFLAGS=${LDFLAGS} -ldl -lpthread `pkg-config --cflags --libs gtk+-3.0 libpulse-simple`
LDDYNAMICFLAGS=${LDFLAGS} -shared
instruments=violin.so sine.so flute.so recorder.so violin2.so guitar.so


all: clean synth ${instruments}

%.o: %.c
	${CC} ${CCDYNAMICFLAGS} -c -o $@ $<

synth.o: synth.c
	${CC} ${CCSTATICFLAGS} -c -o $@ $<
	
wave.o: wave.c
	${CC} ${CCSTATICFLAGS} -c -o $@ $<

%.so: %.o
	${CC} ${LDDYNAMICFLAGS} -o $@ $<

synth: synth.o wave.o
	${CC} ${LDSTATICFLAGS} -o $@ synth.o wave.o 
clean:
	rm -f *.o
	rm -f *.so
	rm -f synth
