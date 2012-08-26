FLAGS=-O2 -Wall -g
INCLUDES=-I/usr/include/cairo
LIBS=-lcairo

all: show_model

clean:
	rm show_model
	rm *.o

show_model: show_model.cpp model.cpp uniform.cpp simulation.cpp
	g++ ${FLAGS} ${INCLUDES} -o show_model show_model.cpp model.cpp uniform.cpp simulation.cpp \
		${LIBS}
