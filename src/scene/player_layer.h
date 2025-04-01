#ifndef PLAYER_LAYER_H
#define PLAYER_LAYER_H

#include <allegro5/bitmap_draw.h>
#include "layer.h"
#include "objects/player.h"

class PlayerLayer final : public ParallaxLayer {
    Player *player;
    ALLEGRO_BITMAP *texture;

public:
    explicit PlayerLayer(Player *p, ALLEGRO_BITMAP *texture)
        : player(p)
          , texture(texture) {
    }

    void update(const float cameraX) override {
    }

    void draw() const override {
        const auto spriteWidth = static_cast<float>(al_get_bitmap_width(texture));
        const auto spriteHeight = static_cast<float>(al_get_bitmap_height(texture));
        const auto playerCenterX = spriteWidth / 2.0f;
        const auto playerCenterY = spriteHeight / 2.0f;
        const auto xPos = SCREEN_WIDTH / 2.0f - spriteWidth / 2.0f;
        const auto yPos = player->getY() - spriteHeight;

        al_draw_rotated_bitmap(texture, playerCenterX, playerCenterY,
                               xPos + spriteWidth / 2.0f, yPos + spriteHeight / 2.0f,
                               fmod(player->getX() / Player::getSpeed(), 360.0f), 0);
    }
};

#endif //PLAYER_LAYER_H
