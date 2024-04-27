#ifndef KEN_H_INCLUDED
#define KEN_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "Object.h"
class Ken : public Object
{
public:
    Ken() : Object()
    {
        vx = 0, vy =0;
        x = 1000, y = 500;
        w = 200,h = 200;
        hp = 1000;
        //hero = load_bitmap_resized("hero1.png",200,200);
    }
};

#endif // KEN_H_INCLUDED
