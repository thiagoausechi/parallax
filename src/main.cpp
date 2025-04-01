#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "managers/assets-manager.h"
#include "scene/scene.h"
#include "scene/continuous_layer.h"
#include "scene/spread_layer.h"

#define SCREEN_W 1920
#define SCREEN_H 1080
#define FPS 30

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = nullptr;
    AssetManager assets("../assets");
    Scene scene;
    bool redraw = true;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
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

    if (!assets.loadAll()) {
        fprintf(stderr, "failed to load assets!\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    constexpr auto SPEED_MULTIPLIER = 10.0f;
    constexpr auto SKY_SPEED = 0.1f * SPEED_MULTIPLIER;
    constexpr auto BACKGROUND_SPEED = 0.3f * SPEED_MULTIPLIER;
    constexpr auto MIDDLE_GROUND_SPEED = 0.5f * SPEED_MULTIPLIER;
    constexpr auto FOREGROUND_SPEED = 0.8f * SPEED_MULTIPLIER;

    // Background Layers (including sky)
    scene.addLayer(std::make_unique<ContinuousLayer>(assets.getBitmap("sky"), SKY_SPEED, 0));
    scene.addLayer(std::make_unique<ContinuousLayer>(assets.getBitmap("grass"), BACKGROUND_SPEED, 0));
    scene.addLayer(std::make_unique<SpreadLayer>(
        std::vector{assets.getBitmap("tree")},
        BACKGROUND_SPEED,
        std::vector{SCREEN_W + 100.0f, SCREEN_W * 2.0f},
        0.0f,
        std::vector{
            180.0f, 190.0f, 200.0f, 210.0f, 220.0f, 230.0f, 240.0f, 250.0f, 260.0f, 270.0f, 280.0f, 290.0f,
            300.0f, 310.0f, 320.0f, 330.0f
        }
    ));

    // Middle ground Layers (where the player is)
    scene.addLayer(std::make_unique<ContinuousLayer>(assets.getBitmap("ground"), MIDDLE_GROUND_SPEED, 0));
    scene.addLayer(std::make_unique<ContinuousLayer>(assets.getBitmap("structure"), MIDDLE_GROUND_SPEED, 0));

    scene.addLayer(std::make_unique<SpreadLayer>(
        std::vector{assets.getBitmap("deco_hydrant")},
        MIDDLE_GROUND_SPEED,
        std::vector{SCREEN_W * 2.0f, SCREEN_W * 3.0f, SCREEN_W * 7.0f},
        0.0f,
        std::vector{138.0f}
    ));
    scene.addLayer(std::make_unique<SpreadLayer>(
        std::vector{assets.getBitmap("deco_trash_can")},
        MIDDLE_GROUND_SPEED,
        std::vector{SCREEN_W * 1.0f, SCREEN_W * 2.0f},
        -4.0f,
        std::vector{0.0f}
    ));
    scene.addLayer(std::make_unique<SpreadLayer>(
        std::vector{assets.getBitmap("deco_flower_pot")},
        MIDDLE_GROUND_SPEED,
        std::vector{
            SCREEN_W * 1.0f, SCREEN_W * 3.0f, SCREEN_W * 5.0f
        },
        905.0f,
        std::vector{0.0f}
    ));

    // Foreground Layers (some visual blocks)
    scene.addLayer(std::make_unique<SpreadLayer>(
        std::vector{assets.getBitmap("deco_high_grass")},
        FOREGROUND_SPEED,
        std::vector{100.0f, 200.0f, SCREEN_W * 2.0f, SCREEN_W + 100.0f},
        0.0f,
        std::vector{0.0f}
    ));

    while (true) {
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            scene.draw();
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
                scene.update();
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
