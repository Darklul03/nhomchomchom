FLAGS=-Isrc/include -Isrc -Lsrc/lib -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -Wl,-Bdynamic -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all:
	g++ -o main src/Game/*.cpp main.cpp $(FLAGS)
	mv main.exe .\bin
