#pragma once
#include "stdafx.h"

#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Game.h"


// SDL.h already has a main function, but it will call our main function here.
// This is why we must give it arguments.
int main(int argc, char** argv) {
    
    Game game;
    game.run();
    return 0;
}
