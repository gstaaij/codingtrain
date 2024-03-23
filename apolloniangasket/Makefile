
all: main main.exe

main: src/main.c src/circle.c src/colors.c src/complex.c src/descartes.c
	mkdir -p build/
	gcc -Wall -Wextra -I./include -o build/main src/main.c src/circle.c src/colors.c src/complex.c src/descartes.c -L./lib/ -lraylib -lm

main.exe: src/main.c src/circle.c src/colors.c src/complex.c src/descartes.c
	mkdir -p build/
	x86_64-w64-mingw32-gcc -Wall -Wextra -I./include -o build/main src/main.c src/circle.c src/colors.c src/complex.c src/descartes.c -L./lib/windows/ -lraylib -lm -lwinmm -lgdi32 -static
