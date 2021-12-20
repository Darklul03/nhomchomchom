FLAGS=-Isrc/include -Isrc -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

all:
	g++ -o main src/Game/MainGame.cpp src/Game/Snake.cpp main.cpp $(FLAGS)
	mv main.exe .\bin
