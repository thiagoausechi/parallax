#ifndef CONTINUOUS_LAYER_H
#define CONTINUOUS_LAYER_H

#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/display.h>

#include "layer.h"

class ContinuousLayer final : public ParallaxLayer {
    ALLEGRO_BITMAP *texture;
    float textureWidth;
    float offsetX;
    float speed;
    float yPosition;

public:
    ContinuousLayer(ALLEGRO_BITMAP *texture, const float speed, const float yPos)
        : texture(texture)
          , offsetX(0)
          , speed(speed)
          , yPosition(yPos) {
        textureWidth = al_get_bitmap_width(texture);
    }

    void update(const float deltaTime) override {
        offsetX -= speed;
        if (offsetX <= -textureWidth) offsetX += textureWidth;
    }

    void draw() const override {
        const auto repeatCount = 2 + al_get_display_width(al_get_current_display()) / textureWidth;
        for (int i = 0; i < repeatCount; i++)
            al_draw_bitmap(texture, offsetX + (i * textureWidth), yPosition, 0);
    }
};

#endif //CONTINUOUS_LAYER_H
