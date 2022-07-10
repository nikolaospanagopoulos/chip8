FLAGS= -Wall -std=c++17 
LIB= -lSDL2


OBJECTS=./build/Chip8Memory.o ./build/Chip8Stack.o ./build/Chip8Keyboard.o ./build/Chip8.o ./build/Chip8Screen.o


all: ${OBJECTS}
	g++ ${FLAGS} -o ./bin/main ./src/main.cpp ${OBJECTS} ${LIB}

./build/Chip8Memory.o: src/Chip8Memory.cpp
	g++ ${FLAGS} ./src/Chip8Memory.cpp -c -o ./build/Chip8Memory.o
./build/Chip8Stack.o: src/Chip8Stack.cpp
	g++ ${FLAGS} ./src/Chip8Stack.cpp -c -o ./build/Chip8Stack.o
./build/Chip8Keyboard.o: src/Chip8Keyboard.cpp 
	g++ ${FLAGS} ./src/Chip8Keyboard.cpp -c -o ./build/Chip8Keyboard.o
./build/Chip8.o: src/Chip8.cpp
	g++ ${FLAGS} ./src/Chip8.cpp -c -o ./build/Chip8.o
./build/Chip8Screen.o: src/Chip8Screen.cpp
	g++ ${FLAGS} ./src/Chip8Screen.cpp -c -o ./build/Chip8Screen.o
