CC = g++
EXECNAME = led
CFLAGS = -g -Wall -Wextra -pedantic -std=c++11
OBJS = SICXE_Parser.o SICXE_Source.o SICXE_Instruction.o
MAIN = a2

compile: ${OBJS}
	${CC} ${CFLAGS} -o ${EXECNAME} ${MAIN}.cpp ${OBJS}

SICXE_Parser.o: SICXE_Parser.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Parser.cpp

SICXE_Source.o: SICXE_Source.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Source.cpp

SICXE_Instruction.o: SICXE_Instruction.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Instruction.cpp

clean:
	rm -f *.o ${EXECNAME} *.exe