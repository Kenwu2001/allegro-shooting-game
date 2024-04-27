#include "Object.h"
ALLEGRO_BITMAP* Object::load_bitmap_resized(const char* filename,int w ,int h)
{
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    //if (!loaded_bmp)
        //game_abort("failed to load image: %s", filename);
    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    //if (!resized_bmp)
        //game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);
    //game_log("resized image: %s", filename);
    return resized_bmp;
}