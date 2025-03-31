#ifndef SPREAD_LAYER_H
#define SPREAD_LAYER_H

#include <random>
#include <vector>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/display.h>

#include "layer.h"

struct Element {
    ALLEGRO_BITMAP *texture;
    float x;
    float y;
};

class SpreadLayer final : public ParallaxLayer {
    const float SPAWN_BUFFER = 200.0f; // Spawn X pixels beyond the screen
    const float SCREEN_WIDTH;
    const float SCREEN_HEIGHT;

    std::vector<ALLEGRO_BITMAP *> sprites;
    std::vector<Element> elements;
    std::vector<float> xPositions;
    std::vector<float> yPositions;
    float xRenderOffset;
    float speed;

    std::mt19937 rng;

public:
    SpreadLayer(const std::vector<ALLEGRO_BITMAP *> &sprites, const float speed,
                const std::vector<float> &xPositionPool,
                const float xOffset,
                const std::vector<float> &yPositionPool)
        : SCREEN_WIDTH(al_get_display_width(al_get_current_display()))
          , SCREEN_HEIGHT(al_get_display_height(al_get_current_display()))
          , sprites(sprites)
          , xPositions(xPositionPool)
          , yPositions(yPositionPool)
          , speed(speed)
          , xRenderOffset(xOffset)
          , rng(std::random_device{}()) {
    }

    void update(const float deltaTime) override {
        moveElements();
        removeOutOfBoundsElements();
        spawnNewElements();
    }

    void draw() const override {
        for (const auto &[texture, x, y]: elements)
            al_draw_bitmap(texture, x + xRenderOffset, y, 0);
    }

private:
    void moveElements() {
        for (auto &element: elements)
            element.x -= speed;
    }

    void removeOutOfBoundsElements() {
        elements.erase(
            std::ranges::remove_if(
                elements,
                [this](const Element &e) {
                    return e.x + SCREEN_WIDTH < 0;
                }).begin(),
            elements.end());
    }

    void spawnNewElements() {
        float farthestX = elements.empty()
                              ? 0
                              : std::ranges::max_element(
                                  elements,
                                  [](const Element &a, const Element &b) { return a.x < b.x; })->x;

        while (farthestX < SCREEN_WIDTH + SPAWN_BUFFER) {
            std::uniform_int_distribution<> spriteDist(0, sprites.size() - 1);
            std::uniform_int_distribution<> xDist(0, xPositions.size() - 1);
            std::uniform_int_distribution<> yDist(0, yPositions.size() - 1);

            const auto sprite = sprites[spriteDist(rng)];
            const auto spriteHeight = al_get_bitmap_height(sprite);

            const auto xPos = farthestX + xPositions[xDist(rng)];
            const auto yPos = SCREEN_HEIGHT - spriteHeight - yPositions[yDist(rng)];
            Element newElement{sprite, xPos, yPos};

            elements.push_back(newElement);
            farthestX = SCREEN_WIDTH + xPos;
        }
    }
};

#endif //SPREAD_LAYER_H
