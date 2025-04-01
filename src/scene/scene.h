#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "layer.h"
#include "objects/player.h"

class Scene {
protected:
    const float SCREEN_WIDTH;
    const float SCREEN_HEIGHT;

    std::vector<std::unique_ptr<ParallaxLayer>> layers;
    Player *player;

public:
    explicit Scene(Player *p)
        : SCREEN_WIDTH(static_cast<float>(al_get_display_width(al_get_current_display())))
          , SCREEN_HEIGHT(static_cast<float>(al_get_display_height(al_get_current_display())))
          , player(p) {
    }

    void addLayer(std::unique_ptr<ParallaxLayer> layer) {
        layers.push_back(std::move(layer));
    }

    void update() const {
        const auto cameraX = player->getX() - SCREEN_WIDTH / 2;

        for (const auto &layer: layers)
            layer->update(cameraX);
    }

    void draw() const {
        for (const auto &layer: layers)
            layer->draw();
    }

    ~Scene() {
        layers.clear();
    }
};

#endif //SCENE_H
