#include "Window.h"
#include <cmath>
#include <iostream>
#define window_width 1300
#define window_height 800
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);

enum
{
    START = 1,
    MENU = 2,
    INTRO1 = 3,
    WIN = 4,
    SELECT_SCENE = 5,
};

int flag;
Object *hero1;
Object *hero2;
Object *ken_bullets = new Object[5];
Object *ken_big = new Object;
Object *Deck_bullets = new Object[5];
Object *Deck_big = new Object;
Object *Heal = new Object[6];
//Object *Pussy = new Object;

const float MAX_COOLDOWN = 0.2f;
const float MAX_COOL = 10.0f;
const float MAX_COOL1 = 8.0f;
double last_shoot_timestamp;

Window::Window()
{
    if (!al_init())
        show_err_msg(-1);
    display = al_create_display(window_width, window_height);
    printf("Game Initializing...\n");
    event_queue = al_create_event_queue();
    game_update_timer = al_create_timer(1.0/FPS);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon
    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event
    al_reserve_samples(10);
    const unsigned m_buttons = al_get_mouse_num_buttons();
    mouse_state = new bool[m_buttons + 1];
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(game_update_timer);
    game_init();
}

Object* Window::create_hero(int type)
{
    Object *h = NULL;
    switch(type)
    {
    case 1:
        h = new Ken();
        break;
    case 2:
        h = new Deck();
        break;
    default:
        break;
    }
    return h;
}

void Window::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
       fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);
    game_destroy();
    exit(9);
}

void Window::game_init()
{
    menu_music = al_load_sample("menu_music.ogg");
    winnn = al_load_sample("win_img.ogg");
    biggg = al_load_sample("big_sound.ogg");
    smalll = al_load_sample("attack_sound.ogg");
    start_music = al_load_sample("start_music.ogg");
    bang = al_load_sample("bomb.ogg");
    font_pirulen_54 = al_load_font("pirulen.ttf", 54, 0);
    font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
    t1 = load_bitmap_resized("t1.png",100,150);
    t2 = load_bitmap_resized("t2.png",100,150);

    menu_scene = load_bitmap_resized("menu.png",1300,800);
    win_scene = load_bitmap_resized("win.png",1300,800);
    //start_scene=load_bitmap_resized("start.png",1300,800);
    start1_scene = load_bitmap_resized("start.png",1300,800);
    start2_scene = load_bitmap_resized("start2.png",1300,800);
    start_scene_s = load_bitmap_resized("start.png",400,300);
    start2_scene_s = load_bitmap_resized("start2.png",400,350);
    //Heal
    Heal_normal_img = load_bitmap_resized("heal_normal.png",50,50);
    for(int i=0;i<6;i++)
        Heal[i].heal_normal = Heal_normal_img;
    //hero_one
    hero1 = create_hero(1);
    hero1_img = load_bitmap_resized("hero1.png",300,200);
    hero1->hero= hero1_img;
    hero1_a_img=load_bitmap_resized("hero1_a.png",100,50);
    hero1_big_img=load_bitmap_resized("hero1_big.png",200,200);
    for(int i=0;i<5;i++)
        ken_bullets[i].attack = hero1_a_img;
    ken_big->big = hero1_big_img;

    //hero_two
    hero2 = create_hero(2);
    hero2_img = load_bitmap_resized("hero2.png",200,200);
    hero2->hero= hero2_img;
    hero2_a_img=load_bitmap_resized("hero2_a.png",100,50);
    hero2_big_img=load_bitmap_resized("hero2_big.png",150,150);
    for(int i=0;i<5;i++)
        Deck_bullets[i].attack = hero2_a_img;
    Deck_big->big = hero2_big_img;
    //Pussy_img = load_bitmap_resized("pussy.png",100,100);
    //Pussy->pussy = Pussy_img;
    change_scene(MENU); //????????????????????????????
}
//void Window::game_reset();
void Window::game_play()
{
    draw_running_map();
}

int Window::game_run()
{
    int error = GAME_CONTINUE;
    al_is_event_queue_empty(event_queue);
    return error;
}

void Window::process_event()
{
    std::cout<<"jjjj"<<std::endl;
    bool done = false;
    ALLEGRO_EVENT event;
    int redraws = 0;
     while (!done) {
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Event for redrawing the display.
            if (event.timer.source == game_update_timer)
                // The redraw timer has ticked.
                redraws++;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Event for keyboard key down.
            //game_log("Key with keycode %d down", event.keyboard.keycode);
            std::cout<<"ggggggggggg"<<std::endl;
            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Event for keyboard key up.
            //game_log("Key with keycode %d up", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = false;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Event for mouse key down.
            //game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Event for mouse key up.
            //game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if (event.mouse.dz != 0) {
                // Event for mouse scroll.
                //game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
            }
        }
        if(redraws > 0 && al_is_event_queue_empty(event_queue)) {
            game_update();
            draw_running_map();
            redraws = 0;
        }
    }
}

void Window::game_destroy()
{
    al_destroy_display(display);
    al_destroy_bitmap(t1);
}

void Window::game_update()
{
    if(active_scene == START)
    {
        hero1->vx = hero1->vy = 0;
        if (key_state[ALLEGRO_KEY_UP])
            hero1->vy -= 1;
        if (key_state[ALLEGRO_KEY_DOWN])
            hero1->vy += 1;
        if (key_state[ALLEGRO_KEY_LEFT])
            hero1->vx -= 1;
        if (key_state[ALLEGRO_KEY_RIGHT])
            hero1->vx += 1;
        // 0.71 is (1/sqrt(2)).
        hero1->y += hero1->vy * 4 * (hero1->vx ? 0.71f : 1);
        hero1->x += hero1->vx * 4 * (hero1->vy ? 0.71f : 1);

        hero2->vx = hero2->vy = 0;
        if (key_state[ALLEGRO_KEY_W])
            hero2->vy -= 1;
        if (key_state[ALLEGRO_KEY_S])
            hero2->vy += 1;
        if (key_state[ALLEGRO_KEY_A])
            hero2->vx -= 1;
        if (key_state[ALLEGRO_KEY_D])
            hero2->vx += 1;
        // 0.71 is (1/sqrt(2)).
        hero2->y += hero2->vy * 4 * (hero2->vx ? 0.71f : 1);
        hero2->x += hero2->vx * 4 * (hero2->vy ? 0.71f : 1);

        // out of edge
        if (hero1->x-hero1->w/2< 0)
           hero1->x =hero1->w/2 ;
        else if (hero1->x+hero1->w/2 > window_width  )
           hero1->x = window_width -hero1->w/2;
        else if(hero1->x-hero1->w/2 < window_width/2)
           hero1->x = window_width/2 +hero1->w/2;
        if (hero1->y-hero1->h/2 < 0)
            hero1->y = hero1->h/2;
        else if (hero1->y+hero1->h/2 > window_height)
            hero1->y = window_height-hero1->h/2;

        if (hero2->x-hero2->w/2< 0)
           hero2->x =hero2->w/2 ;
        else if (hero2->x+hero2->w/2 > window_width/2 )
           hero2->x = window_width/2 -hero2->w/2;
        if (hero2->y-hero2->h/2 < 0)
            hero2->y = hero2->h/2;
        else if (hero2->y+hero2->h/2 > window_height)
            hero2->y = window_height-hero2->h/2;
    }

    // whether the bullet hits enemy
    for(int i=0;i<5;i++)
    {
        if(ken_bullets[i].hidden == true) continue;
        ken_bullets[i].x += ken_bullets[i].vx;
        ken_bullets[i].y += ken_bullets[i].vy;
        if(ken_bullets[i].x - ken_bullets[i].w/2 < 0){ ken_bullets[i].hidden = true;}
        if(ken_bullets[i].hidden==false && hero2->x-hero2->w/2<ken_bullets[i].x&&
            hero2->x+hero2->w/2>ken_bullets[i].x&&
            ken_bullets[i].y<=hero2->y+hero2->h/2&&
            ken_bullets[i].y>=hero2->y-hero2->h/2)
                {
                    al_play_sample(bang,1,0,1,ALLEGRO_PLAYMODE_ONCE,&bang_id);
                    ken_bullets[i].hidden = true;
                    hero2->hp -= 50;
                    std::cout<<"hero2 hp: "<<hero2->hp<<std::endl;
                }
    }

    for(int i=0;i<5;i++)
    {
        if(Deck_bullets[i].hidden == true) continue;
        Deck_bullets[i].x += Deck_bullets[i].vx;
        Deck_bullets[i].y += Deck_bullets[i].vy;
        if(Deck_bullets[i].x + Deck_bullets[i].w/2 > 1300){ Deck_bullets[i].hidden = true;}
        if(Deck_bullets[i].hidden==false&&hero1->x-hero1->w/2<Deck_bullets[i].x&&
            hero1->x+hero1->w/2>Deck_bullets[i].x&&
            Deck_bullets[i].y<=hero1->y+hero1->h/2&&
            Deck_bullets[i].y>=hero1->y-hero1->h/2)
                {
                    al_play_sample(bang,1,0,1,ALLEGRO_PLAYMODE_ONCE,&bang_id);
                    Deck_bullets[i].hidden = true;
                    hero1->hp -= 50;
                    std::cout<<"hero1 hp: "<<hero1->hp<<std::endl;
                }
    }
    //double now = al_get_time();
    for(int i=0;i<6;i++)
    {
        if(Heal[i].hidden == true) continue;
        Heal[i].y += Heal[i].vy;
        if(Heal[i].y  > 800)
        {
            Heal[i].hidden = true;
        }
        if(hero1->x+20>=Heal[i].x-Heal[i].w/2 &&
            hero1->x-20<=Heal[i].x+Heal[i].w/2 &&
           hero1->y+hero1->h/2>=Heal[i].y-Heal[i].h/2 &&
            hero1->y<=Heal[i].y+Heal[i].h/2)
                {
                    Heal[i].hidden = true;
                    hero1->hp += 20;
                    std::cout<<"hero1 hp: "<<hero1->hp<<std::endl;
                }

         if(hero2->x+20>=Heal[i].x-Heal[i].w/2 &&
            hero2->x-20<=Heal[i].x+Heal[i].w/2 &&
           hero2->y+hero2->h/2>=Heal[i].y-Heal[i].h/2 &&
            hero2->y<=Heal[i].y+Heal[i].h/2)
                {
                    Heal[i].hidden = true;
                    hero2->hp += 20;
                    std::cout<<"hero2 hp: "<<hero1->hp<<std::endl;
                }
    }

    /*if(Pussy->hidden!=true)
    {
        Pussy->y -= Pussy->vy;
        if(Pussy->y  < 0)
        {
            Pussy->hidden = true;
        }
        if(hero1->x+20>=Pussy->x-Pussy->w/2 &&
            hero1->x-20<=Pussy->x+Pussy->w/2 &&
           hero1->y+hero1->h/2>=Pussy->y-Pussy->h/2 &&
            hero1->y<=Pussy->y+Pussy->h/2)
                {
                    Pussy->hidden = true;
                    hero1->hp += 200;
                }

         if(hero2->x+20>=Pussy->x-Pussy->w/2 &&
            hero2->x-20<=Pussy->x+Pussy->w/2 &&
           hero2->y+hero2->h/2>=Pussy->y-Pussy->h/2 &&
            hero2->y<=Pussy->y+Pussy->h/2)
                {
                    Pussy->hidden = true;
                    hero2->hp += 200;
                }
    }*/

    if(!ken_big->hidden)
    {
        ken_big->x += ken_big->vx;
        ken_big->y += ken_big->vy;
        if(ken_big->x < 0){ ken_big->hidden = true;}
        if(ken_big->hidden==false &&
           hero2->x+hero2->w/2>=ken_big->x+100/*-ken_big->w/50*/ &&
            hero2->x-hero2->w/2<=ken_big->x/*+ken_big->w/50*/ &&
           hero2->y+50>=ken_big->y/*-ken_big->h/2*/ &&
            hero2->y-hero2->h<=ken_big->y/*+ken_big->h/2*/)
                {
                    al_play_sample(bang,1,0,1,ALLEGRO_PLAYMODE_ONCE,&bang_id);
                    ken_big->hidden = true;
                    hero2->hp -= 200;
                    std::cout<<"x: "<<ken_big->x<<"y: "<<ken_big->y<<std::endl;
                }
    }



    if(!Deck_big->hidden)
    {
        Deck_big->x += Deck_big->vx;
        Deck_big->y += Deck_big->vy;
        if(Deck_big->x > 1300){ Deck_big->hidden = true;}
        if(Deck_big->hidden==false &&  hero1->x+hero1->w/2>=Deck_big->x/*-ken_big->w/50*/ &&
            hero1->x-hero1->w/2<=Deck_big->x-50/*+ken_big->w/50*/ &&
           hero1->y+50>=Deck_big->y/*-ken_big->h/2*/ &&
            hero1->y-hero1->h<=Deck_big->y/*+ken_big->h/2*/)
                {
                    al_play_sample(bang,1,0,1,ALLEGRO_PLAYMODE_ONCE,&bang_id);
                    Deck_big->hidden = true;
                    hero1->hp -= 200;
                    std::cout<<"hero1 hp: "<<hero1->hp<<std::endl;
                }
    }

    // press button to attack
    double now = al_get_time();
    if(key_state[ALLEGRO_KEY_O] && now - last_shoot_timestamp >= MAX_COOLDOWN)
    {
        for(int j=0;j<5;j++)
        {
            if(ken_bullets[j].hidden)
            {
                std::cout<<"+++++"<<std::endl;
                al_play_sample(smalll,1,0,1,ALLEGRO_PLAYMODE_ONCE,&smalll_id);
                last_shoot_timestamp = now;
                ken_bullets[j].hidden = false;
                ken_bullets[j].x = hero1->x;
                ken_bullets[j].y = hero1->y;
                break;
            }
        }
        std::cout<<"daaagggg"<<std::endl;
    }
    if(now - last_shoot_timestamp >= MAX_COOL)
    {
        for(int j=0;j<6;j++)
        {
            if(Heal[j].hidden)
            {
                //std::cout<<"+++++"<<std::endl;
                last_shoot_timestamp = now;
                Heal[j].hidden = false;
                //ken_big->x = hero1->x;
                //ken_big->y = hero1->y;
                Heal[j].x = Heal[j].w / 2 + (float)rand() / RAND_MAX * (1300 - Heal[j].w);
                Heal[j].y = 0;
            }
        }

    }

    /*if(now - last_shoot_timestamp >= MAX_COOL1)
    {
            if(Pussy->hidden)
            {
                //std::cout<<"+++++"<<std::endl;
                last_shoot_timestamp = now;
                Pussy->hidden = false;
                //ken_big->x = hero1->x;
                //ken_big->y = hero1->y;
                Pussy->x = Pussy->w / 2 + (float)rand() / RAND_MAX * (1300 - Pussy->w);
                Pussy->y = 800;
            }
    }*/

    if(key_state[ALLEGRO_KEY_P] && now - last_shoot_timestamp >= MAX_COOLDOWN)
    {
        if(ken_big->hidden)
        {
            al_play_sample(biggg,1,0,1,ALLEGRO_PLAYMODE_ONCE,&biggg_id);
            std::cout<<"+++++"<<std::endl;
            last_shoot_timestamp = now;
            ken_big->hidden = false;
            ken_big->x = hero1->x;
            ken_big->y = hero1->y;
        }
    }
    if(key_state[ALLEGRO_KEY_C] && now - last_shoot_timestamp >= MAX_COOLDOWN)
    {
        for(int k=0;k<5;k++)
        {
            if(Deck_bullets[k].hidden)
            {
                al_play_sample(smalll,1,0,1,ALLEGRO_PLAYMODE_ONCE,&smalll_id);
                //std::cout<<"+++++"<<std::endl;
                last_shoot_timestamp = now;
                Deck_bullets[k].hidden = false;
                Deck_bullets[k].x = hero2->x;
                Deck_bullets[k].y = hero2->y;
                break;
            }
        }
        std::cout<<"daaagggg"<<std::endl;
    }
    if(key_state[ALLEGRO_KEY_V] && now - last_shoot_timestamp >= MAX_COOLDOWN)
    {
        if(Deck_big->hidden)
        {
            al_play_sample(biggg,1,0,1,ALLEGRO_PLAYMODE_ONCE,&biggg_id);
            std::cout<<"+++++"<<std::endl;
            last_shoot_timestamp = now;
            Deck_big->hidden = false;
            Deck_big->x = hero2->x;
            Deck_big->y = hero2->y;
        }
    }
    if(hero1->hp<=0 || hero2->hp <= 0)
    {
        change_scene(4);
        hero1->hp = 1000;
        hero2->hp = 1000;
    }
}

void Window::draw_running_map()
{
    if(active_scene == START)
    {
        //al_clear_to_color(al_map_rgb(149,128,255));
        if(flag == 1) al_draw_bitmap(start1_scene,0, 0, 0);
        else if(flag == 2) al_draw_bitmap(start2_scene,0, 0, 0);
           //al_draw_bitmap(start_scene,0, 0, 0);
        al_draw_bitmap(t1,1200, 630, 0);
        al_draw_bitmap(t2,0, 630, 0);
        //al_draw_bitmap(hero2_a_img,300, 400, 0);

        //al_draw_bitmap(hero1_img,1100,200,0);
        draw_movable_object(hero1);
        draw_movable_object(hero2);
        for(int i=0;i<5;i++)
            draw_movable_bullet(ken_bullets[i]);
        draw_movable_big(ken_big);
        for(int i=0;i<5;i++)
            draw_movable_bullet(Deck_bullets[i]);
        for(int i=0;i<6;i++)
            draw_movable_heal(Heal[i]);
        draw_movable_big(Deck_big);
        //draw_movable_pussy(Pussy);
        //std::cout<<"aaaaaaa"<<std::endl;
        al_draw_filled_rectangle(900,50,
            900+hero1->hp/3,60,al_map_rgb(0xd8,0x00,0x00));
        al_draw_filled_rectangle(100,50,
            100+hero2->hp/3,60,al_map_rgb(0xd8,0x00,0x00));

        al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), 1060,
                       70,ALLEGRO_ALIGN_CENTER,"%d",hero1->hp);
        al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), 270,
                       70,ALLEGRO_ALIGN_CENTER,"%d",hero2->hp);
    }
    else if(active_scene == MENU)
    {
        //al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(menu_scene,0, 0, 0);
        al_draw_text(font_pirulen_54, al_map_rgb(0, 0, 0),
                     670, 400, ALLEGRO_ALIGN_CENTER, "K D G");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255),
                      20, 800 - 50, 0, "Press enter key to start");
        //std::cout<<"9999999999"<<std::endl;
    }
    else if(active_scene == WIN)
    {
        al_draw_bitmap(win_scene,0, 0, 0);
        al_draw_text(font_pirulen_54, al_map_rgb(255, 255, 255),
                     660, 360, ALLEGRO_ALIGN_CENTER, "VICTORY");
    }
    else if(active_scene == SELECT_SCENE)
    {
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(start_scene_s,165, 170, 0);
        al_draw_bitmap(start2_scene_s,715, 145, 0);
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255),
                      300, 500, 0, "SCENE 1");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255),
                        850, 500, 0, "SCENE 2");
    }

    //std::cout<<"eeeeeeeeeeee"<<std::endl;
    al_flip_display();
}

/*void Window::draw_movable_pussy(Object* obj)
{
    if (obj->hidden) return;
    al_draw_bitmap(obj->pussy,round(obj->x - obj->w / 2),round(obj->y - obj->h / 2),0);
}*/

void Window::draw_movable_object(Object* obj)
{
   //if (obj->hidden)
        //return;
    al_draw_bitmap(obj->hero, round(obj->x - obj->w / 2), round(obj->y - obj->h / 2), 0);
}

void Window::draw_movable_bullet(Object obj)
{
    if (obj.hidden) return;
    al_draw_bitmap(obj.attack,round(obj.x - obj.w / 2),round(obj.y - obj.h / 2),0);
}

void Window::draw_movable_heal(Object obj)
{
    if (obj.hidden) return;
    al_draw_bitmap(obj.heal_normal,round(obj.x - obj.w / 2),round(obj.y - obj.h / 2),0);
}
void Window::draw_movable_big(Object* obj)
{
    if (obj->hidden) return;
    al_draw_bitmap(obj->big,round(obj->x - obj->w / 2),round(obj->y - obj->h / 2),0);
}

ALLEGRO_BITMAP *Window::load_bitmap_resized(const char* filename, int w, int h) {
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


void Window::on_mouse_down(int btn,int x,int y)
{
    if(active_scene==SELECT_SCENE)
    {
        if(btn==1){
            if(pnt_in_rect(x,y,165,170,400,300))
            {
                flag = 1;
                change_scene(START);
            }
            else if(pnt_in_rect(x,y,715,170,400,300))
            {
                flag = 2;
                change_scene(START);
            }
        }
    }
}
bool Window::pnt_in_rect(int px,int py, int x, int y, int w,int h)
{
     if (px <= x + w && px >= x && py <= y + h && py >= y)
        return true;
    else
        return false;
}

void Window::change_scene(int next_scene)
{
    if (active_scene == MENU) { //1
        al_stop_sample(&menu_music_id);
    }
    else if (active_scene == START) { //2
        al_stop_sample(&start_music_id);
    }
    else if(active_scene == WIN)
    {
        al_stop_sample(&winnn_id);
    }
    //Pussy->hidden = false;
    active_scene = next_scene;
    if(active_scene == START)
    {
        /*if(pnt_in_rect(mouse_x,mouse_y,165,170,400,300))
        {
            start_scene=start1_scene;
        }
        else if(pnt_in_rect(mouse_x,mouse_y,715,170,400,300))
        {
            start_scene=start2_scene;
        }*/
        std::cout<<"start1111111"<<std::endl;
        al_play_sample(start_music, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_music_id);
        for(int i = 0;i<5;i++)
        {
            std::cout<<"ghghghghg"<<std::endl;
            ken_bullets[i].w = 100;
            ken_bullets[i].h = 50;
            //ken_bullets[i]->attack = hero1_a_img;
            ken_bullets[i].vx = -7;
            ken_bullets[i].vy = 0;
            ken_bullets[i].hidden = true;
        }
         for(int i = 0;i<6;i++)
        {
            std::cout<<"ghghghghg"<<std::endl;
            Heal[i].w = 100;
            Heal[i].h = 100;
            Heal[i].vy = 7;
            //Heal[i].x = 500;
            Heal[i].x = Heal[i].w / 2 + (float)rand() / RAND_MAX * (1300 - Heal[i].w);
            Heal[i].y = 0;
            //Heal[i].vx=rand()%4+2;
            //Heal[i].vy=rand()%4+1;
            Heal[i].hidden = false;
        }

        /*Pussy->w = 100;
        Pussy->h = 100;
        Pussy->vy = 7;
        Pussy->x = Pussy->w / 2 + (float)rand() / RAND_MAX * (1300 - Pussy->w);
        Pussy->y = 800;
        Pussy->hidden = false;*/

        ken_big->w = 100;
        ken_big->h = 50;
        ken_big->vx = -10;
        ken_big->vy = 0;
        ken_big->hidden = true;
        for(int j = 0;j<5;j++)
        {
            std::cout<<"ghghghghg"<<std::endl;
            Deck_bullets[j].w = 100;
            Deck_bullets[j].h = 50;
            //ken_bullets[i]->attack = hero1_a_img;
            Deck_bullets[j].vx = 7;
            Deck_bullets[j].vy = 0;
            Deck_bullets[j].hidden = true;
        }
        Deck_big->w = 100;
        Deck_big->h = 50;
        //ken_bullets[i]->attack = hero1_a_img;
        Deck_big->vx = 10;
        Deck_big->vy = 0;
        Deck_big->hidden = true;

    }
    else if(active_scene == MENU)
    {
        std::cout<<"menuuuuuuu"<<std::endl;
        al_play_sample(menu_music, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menu_music_id);
    }
    else if(active_scene == WIN)
    {
        al_play_sample(winnn, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &winnn_id);
    }
}

void Window::on_key_down(int keycode) {
    if (active_scene == MENU) {
        std::cout<<"bbbb"<<std::endl;
        if (keycode == ALLEGRO_KEY_ENTER)
            change_scene(SELECT_SCENE);
    }
    else if (active_scene == START) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            change_scene(MENU);
    }
    else if (active_scene == WIN) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            change_scene(MENU);
    }
}
