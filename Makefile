FLAGS=-Isrc/include -Isrc -Lsrc/lib -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -Wl,-Bdynamic -lmingw32 -lSDL2main -lSDL2

all:
	g++ -o main src/Game/MainGame.cpp src/Game/Snake.cpp main.cpp $(FLAGS)
	mv main.exe .\bin
