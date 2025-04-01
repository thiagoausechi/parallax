#ifndef SPREAD_LAYER_H
#define SPREAD_LAYER_H

#include <random>
#include <vector>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>

#include "layer.h"

struct Element {
    ALLEGRO_BITMAP *texture;
    float worldX;
    float worldY;
};

class SpreadLayer final : public ParallaxLayer {
    float MAX_SPRITE_WIDTH;

    std::vector<ALLEGRO_BITMAP *> sprites;
    std::vector<Element> elements;
    std::vector<float> xPositions;
    std::vector<float> yPositions;
    float parallaxFactor;
    float xRenderOffset;

    std::mt19937 rng;
    float offsetX;
    float farthestX = 0.0f;

public:
    SpreadLayer(const std::vector<ALLEGRO_BITMAP *> &sprites, const float parallaxFactor,
                const std::vector<float> &xPositionPool,
                const float xOffset,
                const std::vector<float> &yPositionPool)
        : sprites(sprites)
          , xPositions(xPositionPool)
          , yPositions(yPositionPool)
          , parallaxFactor(parallaxFactor)
          , xRenderOffset(xOffset)
          , rng(std::random_device{}())
          , offsetX(0) {
        const auto largestSprite = std::ranges::max_element(
            sprites,
            [](ALLEGRO_BITMAP *a, ALLEGRO_BITMAP *b) {
                return al_get_bitmap_width(a) < al_get_bitmap_width(b);
            }).base();

        MAX_SPRITE_WIDTH = static_cast<float>(al_get_bitmap_width(*largestSprite));
    }

    void update(const float cameraX) override {
        offsetX = -cameraX * parallaxFactor;
        checkSpawn();
    }

    void draw() const override {
        for (const auto &[texture, worldX, worldY]: elements) {
            const float renderX = worldX + xRenderOffset + offsetX;

            if (const auto elementWidth = static_cast<float>(al_get_bitmap_width(texture));
                renderX + elementWidth < 0 || renderX > SCREEN_WIDTH)
                continue;

            al_draw_bitmap(texture, renderX, worldY, 0);
        }
    }

private:
    void checkSpawn() {
        const float spawnThreshold = -offsetX + SCREEN_WIDTH + MAX_SPRITE_WIDTH;

        while (farthestX < spawnThreshold)
            spawnNewElement();
    }

    void spawnNewElement() {
        std::uniform_int_distribution spriteDist(0, static_cast<int>(sprites.size()) - 1);
        std::uniform_int_distribution xDist(0, static_cast<int>(xPositions.size()) - 1);
        std::uniform_int_distribution yDist(0, static_cast<int>(yPositions.size()) - 1);

        const auto sprite = sprites[spriteDist(rng)];
        const auto spriteHeight = static_cast<float>(al_get_bitmap_height(sprite));

        const auto xPos = farthestX + xPositions[xDist(rng)];
        const auto yPos = SCREEN_HEIGHT - spriteHeight - yPositions[yDist(rng)];

        elements.emplace_back(Element{sprite, xPos, yPos});

        farthestX = SCREEN_WIDTH + xPos;
    }
};

#endif //SPREAD_LAYER_H
