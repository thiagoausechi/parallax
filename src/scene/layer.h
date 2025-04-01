#ifndef LAYER_H
#define LAYER_H

class ParallaxLayer : public std::enable_shared_from_this<ParallaxLayer> {
protected:
    const float SCREEN_WIDTH;
    const float SCREEN_HEIGHT;

public:
    ParallaxLayer()
        : SCREEN_WIDTH(static_cast<float>(al_get_display_width(al_get_current_display())))
          , SCREEN_HEIGHT(static_cast<float>(al_get_display_height(al_get_current_display()))) {
    }

    virtual void update(float cameraX) = 0;

    virtual void draw() const = 0;

    virtual ~ParallaxLayer() = default;
};

#endif //LAYER_H
