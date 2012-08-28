FLAGS=-O2 -Wall -g #-pg
INCLUDES=-I/usr/include/cairo
LIBS=-lcairo

all: show_model sinr

clean:
	rm show_model
	rm *.o

show_model: show_model.cpp model.cpp uniform.cpp simulation.cpp naive.cpp
	g++ ${FLAGS} ${INCLUDES} -o show_model show_model.cpp model.cpp uniform.cpp \
		simulation.cpp naive.cpp ${LIBS}

sinr: sinr.cpp model.cpp uniform.cpp simulation.cpp naive.cpp
	g++ ${FLAGS} ${INCLUDES} -o sinr sinr.cpp model.cpp uniform.cpp simulation.cpp \
	   	naive.cpp ${LIBS}
