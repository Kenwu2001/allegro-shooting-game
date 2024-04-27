#include <iostream>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> // ANS: include the head file of font
#include <allegro5/allegro_ttf.h> // ANS: include the head file of ttf
#include <allegro5/allegro_image.h>// ANS: include the head file of image
#include "Window.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    Window *Game= new Window();
    Game->game_play(); // next to game_begin
    //Game->draw_running_map();
    Game->process_event();
    Game->game_destroy();
    return 0;
}
