#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
class Object
{
public:
    Object() {}
    //~Object() {}
    ALLEGRO_BITMAP* load_bitmap_resized(const char*, int, int);
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x, y axes.
    float vx, vy;
    // Should we draw this object on the screen.
    bool hidden;

    int hp;

    int attack_num;
    ALLEGRO_BITMAP* hero;
    ALLEGRO_BITMAP* attack;
    ALLEGRO_BITMAP* big;
    ALLEGRO_BITMAP* heal_normal;
    ALLEGRO_BITMAP* pussy;
};

#endif // OBJECT_H_INCLUDED
