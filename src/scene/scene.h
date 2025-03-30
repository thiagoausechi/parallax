#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <allegro5/bitmap.h>

struct sprite {
    ALLEGRO_BITMAP *source;
    float x, y, parallax_ratio;
    int w, h;

    sprite(ALLEGRO_BITMAP *source, const float x, const float y, const float parallax_ratio, const int w,
           const int h) : source(source), x(x), y(y), parallax_ratio(parallax_ratio), w(w), h(h) {
    }
};

class Scene {
    int screen_w, screen_h;
    std::vector<sprite> sprites;

public:
    Scene(const int screen_w, const int screen_h) : screen_w(screen_w), screen_h(screen_h) {
    };

    void add_sprite(const std::string &name, const float x, const float y, const float parallax_ratio) {
        const auto bitmap = al_load_bitmap(("../assets/" + name + ".png").c_str());
        const auto bitmap_width = al_get_bitmap_width(bitmap);
        const auto bitmap_height = al_get_bitmap_height(bitmap);

        sprites.emplace_back(bitmap, x, y, parallax_ratio, bitmap_width, bitmap_height);
    }

    void update() {
        for (auto &sprite: sprites) {
            sprite.x += sprite.parallax_ratio * -1;
            if (sprite.x + static_cast<float>(sprite.w) <= 0)
                sprite.x = 0;
        }
    }

    void draw() const {
        for (const auto &sprite: sprites) {
            al_draw_bitmap(sprite.source, sprite.x, sprite.y, 0);

            if (static_cast<int>(sprite.x) + sprite.w < screen_w)
                al_draw_bitmap(sprite.source, sprite.x + static_cast<float>(sprite.w), sprite.y, 0);
        }
    }

    ~Scene() {
        for (const auto &sprite: sprites)
            al_destroy_bitmap(sprite.source);
    }
};

#endif //SCENE_H
