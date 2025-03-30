#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "scene/scene.h"

#define SCREEN_W 1920
#define SCREEN_H 1080
#define FPS 30

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = nullptr;
    Scene *scene = nullptr;
    bool redraw = true;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "failed to install mouse!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize allegro image addon!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    scene = new Scene(SCREEN_W, SCREEN_H);

    scene->add_sprite("sky", 0, 0, 1);
    scene->add_sprite("grass", 0, 0, 2);
    scene->add_sprite("ground", 0, 0, 2);
    scene->add_sprite("tree", 0, 0, 2);

    scene->add_sprite("structure", 0, 0, 3);
    scene->add_sprite("deco_hydrant", 0, 0, 3);
    scene->add_sprite("deco_flower_pot", 0, 0, 3);
    scene->add_sprite("deco_trash_can", 0, 0, 3);

    scene->add_sprite("deco_high_grass", 0, 0, 3.5);

    while (true) {
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            scene->draw();
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }

        al_wait_for_event(event_queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        goto end;
                    default: ;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                scene->update();
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                goto end;
            default:
                break;
        }
    }

end:
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
