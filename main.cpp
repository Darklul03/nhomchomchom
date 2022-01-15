#include <Game/MainGame.hpp>
int main (int argc, char* argv[]) {
    MainGame Game;
    Game.run();
    Game.cleanup();
    return 0;
}
