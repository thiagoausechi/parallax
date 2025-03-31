#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "layer.h"

class Scene {
    std::vector<std::unique_ptr<ParallaxLayer>> layers;
    float lastTime;

public:
    Scene()
        : lastTime(0) { ; }

    void addLayer(std::unique_ptr<ParallaxLayer> layer) {
        layers.push_back(std::move(layer));
    }

    void update() const {
        const auto delta_time = static_cast<float>(al_get_time()) - lastTime;
        for (auto &layer: layers)
            layer->update(delta_time);
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
