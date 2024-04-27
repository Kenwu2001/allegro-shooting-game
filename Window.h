#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED
#include "Object.h"
#include "Ken.h"
#include "Deck.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#define GAME_INIT 5
#define GAME_SETTING 6
#define GAME_SELECT 7
#define GAME_BEGIN 8
#define GAME_CONTINUE 9
#define GAME_FAIL 10
#define GAME_TERMINATE 11
#define GAME_NEXT_LEVEL 12
#define GAME_EXIT 13

class Window
{
public:
    Window();//constructor
    void game_init();
    void game_reset();
    void game_play();
    void game_begin();
    int game_run();
    void game_update();
    void show_err_msg(int msg);
    void game_destroy();
    void draw_running_map(); // each drawing scene function
    void draw_movable_object(Object*);
    void draw_movable_bullet(Object);
    void process_event(); // process of updated event
    void change_scene(int);
    void on_key_down(int keycode);
    Object* create_hero(int);
    void on_mouse_down(int btn,int x,int y);
    bool pnt_in_rect(int px,int py, int x, int y, int w,int h);
    ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
    void draw_movable_big(Object*);
    void draw_movable_heal(Object);
    void draw_movable_pussy(Object* obj);

private:
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *menu_scene;
    ALLEGRO_BITMAP *start_scene;
    ALLEGRO_BITMAP *start1_scene;
    ALLEGRO_BITMAP *start2_scene;
    ALLEGRO_BITMAP *start_scene_s;
    ALLEGRO_BITMAP *start2_scene_s;
    ALLEGRO_BITMAP *select_scene;
    ALLEGRO_BITMAP *select_character_scene;
    ALLEGRO_BITMAP *win_scene;
    ALLEGRO_BITMAP *t1;
    ALLEGRO_BITMAP *t2;

    ALLEGRO_BITMAP *hero1_img;
    ALLEGRO_BITMAP *hero1_a_img;
    ALLEGRO_BITMAP *hero1_big_img;

    ALLEGRO_BITMAP *Heal_normal_img;
    ALLEGRO_BITMAP *Pussy_img;
    ALLEGRO_BITMAP *hero2_img;
    ALLEGRO_BITMAP *hero2_a_img;
    ALLEGRO_BITMAP *hero2_big_img;

    ALLEGRO_FONT* font_pirulen_54;
    ALLEGRO_FONT* font_pirulen_24;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER* game_update_timer;
    ALLEGRO_SAMPLE* bang;
    ALLEGRO_SAMPLE_ID bang_id;
    ALLEGRO_SAMPLE* winnn;
    ALLEGRO_SAMPLE_ID winnn_id;
    ALLEGRO_SAMPLE* biggg;
    ALLEGRO_SAMPLE_ID biggg_id;
    ALLEGRO_SAMPLE* smalll;
    ALLEGRO_SAMPLE_ID smalll_id;
    ALLEGRO_SAMPLE* menu_music;
    ALLEGRO_SAMPLE_ID menu_music_id;
    ALLEGRO_SAMPLE* start_music;
    ALLEGRO_SAMPLE_ID start_music_id;
    bool key_state[ALLEGRO_KEY_MAX];
    bool *mouse_state;
    int mouse_x,mouse_y;
    const float FPS = 60;
    //bool redraw = false;
    int active_scene;
};

#endif // WINDOW_H_INCLUDED
