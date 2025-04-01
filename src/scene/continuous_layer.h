#ifndef CONTINUOUS_LAYER_H
#define CONTINUOUS_LAYER_H

#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>

#include "layer.h"

class ContinuousLayer final : public ParallaxLayer {
    ALLEGRO_BITMAP *texture;
    float textureWidth;
    float offsetX;
    float parallaxFactor;
    float yPosition;

public:
    ContinuousLayer(ALLEGRO_BITMAP *texture, const float parallaxFactor, const float yPos)
        : texture(texture)
          , offsetX(0)
          , parallaxFactor(parallaxFactor)
          , yPosition(yPos) {
        textureWidth = static_cast<float>(al_get_bitmap_width(texture));
        if (textureWidth <= 0) textureWidth = 1;
    }

    void update(const float cameraX) override {
        float baseOffset = -cameraX * parallaxFactor;
        baseOffset = fmod(baseOffset, textureWidth);
        if (baseOffset > 0) baseOffset -= textureWidth;
        offsetX = baseOffset;
    }

    void draw() const override {
        const int repeatCount = static_cast<int>(SCREEN_WIDTH / textureWidth);

        // for (int i = 0; i < repeatCount; i++)
        //     al_draw_bitmap(texture, offsetX + (i * textureWidth), yPosition, 0);

        for (int i = -1; i <= repeatCount; ++i) {
            const float xPos = offsetX + static_cast<float>(i) * textureWidth;
            al_draw_bitmap(texture, xPos, yPosition, 0);
        }
    }
};

#endif //CONTINUOUS_LAYER_H
