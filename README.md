# Metadata

**File**: README

**Authors**: Manuel Loera,Brandon Altamirano Ortega,Shane Moro

**Account**: cssc3006,cssc3740,cssc3711

**Class**: cs530 Spring 2021

**Assignment**: Project 2, LED

# File Manifest

- a2.cpp
- a2.h
- README.md
- SICXE_Parser.cpp
- SICXE_Source.cpp
- SICXE_Instruction.cpp
- SICXE_Dictionary.cpp
- led

# Compiler Instructions
**From makefile...**
```CC = g++
EXECNAME = led
CFLAGS = -g -Wall -Wextra -pedantic -std=c++11
OBJS = SICXE_Parser.o SICXE_Source.o SICXE_Instruction.o SICXE_Dictionary.o
MAIN = a2

compile: ${OBJS}
	${CC} ${CFLAGS} -o ${EXECNAME} ${MAIN}.cpp ${OBJS}

SICXE_Parser.o: SICXE_Parser.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Parser.cpp

SICXE_Source.o: SICXE_Source.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Source.cpp

SICXE_Instruction.o: SICXE_Instruction.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Instruction.cpp

SICXE_Dictionary.o: SICXE_Dictionary.cpp ${MAIN}.h
	${CC} ${CFLAGS} -c SICXE_Dictionary.cpp

clean:
	rm -f *.o ${EXECNAME} *.exe
```
***These commands work for any regular file as long as the file path is valid***

# Design Decisions
- Used a seperate .h file to keep declarations and definitions seperate from source code space
- Broke down the program into 2 main functions where one handles reading in of file, and the other processes file (more detail within .cpp source file)

- Definitions (**#define**) used for important paramaters used in IO calls, important branching conditionals, or for important array access indecies
- Some variables global for simpler function calls (making code look clean), also made sense the functions should be able to see these variables within their scope along with **main()**
# Features, Algorithms, Functionality
- Used **iostream** along with **bitset** for compatability between the two when it comes to IO
# Bugs
**N/A**
# Lessons Learned
- Bitset was a new type that I had never used and made it easy to interpret data into binary.