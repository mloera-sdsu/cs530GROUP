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

# Running the Program...
**Build the program**

- make sure to be in the directory of the project and run make in console:

```$> make```

**Run the program**

- use sample files in the ./samples folder

```
$> led samples/P2sampleAdder.lis samples/P2sampleWriter.lis
```

- output from program shows memory reference errors on terminal, and outputs .obj and .st files regardless of invalid references for purposes of demonstration (though we could've halted at invalid memory)

# Design Decisions
- Used a seperate .h file to keep declarations and definitions seperate from source code space
- Broke down the program into 2 main functions where one handles reading in of file, and the other processes file (more detail within .cpp source file)
- Definitions (**#define**) used for important paramaters used in IO calls, important branching conditionals, or for important array access indecies
- Used different classes to store information of the source currently analyzing. Stored each source into vectors of its corresponding type of data
# Features, Algorithms, Functionality
- Parsing and storing data into vectors and objects. Also used stringstreams and file streams to write text into a new file.
- Handles starting, extdef, and extref lines before the rest of the instructions
- Prints ESTAB (name of sections, extdefs per section, lengths, and starting and ending addresses)
# Bugs
**N/A**
# Lessons Learned
Parsing methods from each member can be combined to receive desired ouput. It is more efficient to store all data neceesary prior to outputting results